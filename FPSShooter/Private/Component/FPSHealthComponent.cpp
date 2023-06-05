// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/FPSHealthComponent.h"

#include "Classes/FPSUnitsBase.h"

#include "FPSHUD.h"

// Конструктор компонента здоровья.
UFPSHealthComponent::UFPSHealthComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Тик отключен.
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	_healthMax = 100.0f;
	_healthAmmount = 100.0f;
	_recoveryHealth = 1.0f;

	// ...
}

// Пре инцилизация компонента здоровья.
void UFPSHealthComponent::PostInitProperties()
{
	Super::PostInitProperties();
}

// Called when the game starts
void UFPSHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// Устанавливаем в начале ХП юниту, равнаю максимального здоровья.
	SetHealthAmmount(_healthMax);

	// ...
	
}

// Установить максимальное здоровье юниту.
void UFPSHealthComponent::SetHealthMax(float NewMax)
{
	_healthMax = abs(NewMax);
}

// Установить текущее здоровье юниту.
float UFPSHealthComponent::SetHealthAmmount(float NewHealth)
{
	// Новое значение текущего здоровье, 
	// Зажато между значениями ноль и максимальным кол-во ХП.
	_healthAmmount = FMath::Clamp(NewHealth,
		0.0f, GetHealthMax());

	// Проверяем что владелец валиден.
	if (GetPlayerComponent())
	{
		// Ссылка на HUD.
		const AFPSHUD* _hudLocal =
			GetPlayerComponent()->GetPlayerHUD();

		// Проверяем ссылку на HUD.
		if (_hudLocal)
		{
			// Вызываем делегат изменения здоровья.
			_hudLocal->OnHealthChanged.Broadcast(
				_healthAmmount, // Передаем текущее значение.
				_healthMax); // Передаем максимальное значение.
		}
	}

	// Проверяем, что ХП равно или меньше нуля.
	// Также проверяем, что делегат смерти привязан к функции.
	if (GetHealthCount() <= 0) {
		OnDead.Broadcast(); // Вызываем делегат смерти.
	}

	// Возращаем текущее здоровье.
	return _healthAmmount;
}

// Получение урона.
float UFPSHealthComponent::AnyDammage(float DamageAmount)
{
	// Высчитываем новое значение ХП.
	// От текущего ХП отнимаем урон оружия.
	const float _newHealth = GetHealthCount() - DamageAmount;
	
	// Устанавливаем новое значение ХП.
	SetHealthAmmount(_newHealth);

	// Возращаем нанесенный урон.
	return DamageAmount;
}

