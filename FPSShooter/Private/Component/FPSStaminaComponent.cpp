// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/FPSStaminaComponent.h"

#include "Classes/FPSUnitsBase.h"
#include "FPSHUD.h"

#include "Engine/EngineTypes.h"
#include "TimerManager.h"

// Конструктор.
UFPSStaminaComponent::UFPSStaminaComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;

	_staminaMax = 100.0f;
	_staminaAmmount = 100.0f;
	_recoveryStamina = 1.0f;
	_reductionStamina = 1.0f;

	_curveStamina = nullptr;

	// ...
}

// Пре иницилизация.
void UFPSStaminaComponent::PostInitProperties()
{
	Super::PostInitProperties();
}

// Старт игры.
void UFPSStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	// В начале игры, устанавливаем текущее стамину равную максимальной.
	SetStaminaAmmount(_staminaMax);

	// Проверяем что кривая стамины выбрана.
	if (_curveStamina)
	{
		// Делегат.
		FOnTimelineFloat _stamina;

		// Привязываем функцию к делегату.
		_stamina.BindUFunction(this, FName("AddStamina"));

		// Создаем линейную интерполяцию.
		_staminaTiemLine.AddInterpFloat(_curveStamina, _stamina);
	}

	// ...

}

// Тик.
void UFPSStaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	_staminaTiemLine.TickTimeline(DeltaTime);
}

// Установить максимальную выносливость юниту.
void UFPSStaminaComponent::SetStaminaMax(float NewMax)
{
	_staminaMax = abs(NewMax);
}

// Установить текущею выносливость юниту.
float UFPSStaminaComponent::SetStaminaAmmount(float NewHealth)
{
	// Зажимаем новое значение мужду 0 и максимальным допустимым значением стамины.
	_staminaAmmount = FMath::Clamp(NewHealth, // Поступающее значение.
		0.0f, // Минимальное значение.
		_staminaMax // Максимальное значение.
	);

	// Проверяем что владелец валиден.
	if (GetPlayerComponent())
	{
		// Ссылка на HUD.
		const AFPSHUD* _hudLocal =
			GetPlayerComponent()->GetPlayerHUD();

		// Проверяем ссылку на HUD.
		if (_hudLocal)
		{
			// Вызываем делегат изменения стамины.
			_hudLocal->OnStaminaChanged.Broadcast(
				_staminaAmmount, // Передаем текущее значение.
				_staminaMax); // Передаем максимальное значение.
		}
	}

	// Возращаем значение стамины.
	return _staminaAmmount;
}

// Регулирование кол - во стамины.
float UFPSStaminaComponent::AddStamina(float Value)
{
	if (GetPlayerComponent())
	{
		// Высчитываем новое значение стамины.
		const float _newStamina = FMath::Lerp(
			GetStaminaMax(), // Текущее кол-во стамины.
			0.0, // Значение до которого стамина изменяется.
			Value // Альфа.
		);

		// Записываем новое значение стамины.
		const float _currentStamina = 
			SetStaminaAmmount(_newStamina);

		// Проверяем, если новое значение стамины меньше равно нулю.
		if (_newStamina <= 0.0f)
		{
			// Отключаем ускорение.
			GetPlayerComponent()->SetSpeedRun(false);
			//GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, TEXT("1!"));
		}

		// Возращаем значение стамины.
		return _newStamina;
	}

	// Возращаем значение стамины.
	return 0.0f;
}

// Включает тип изменения стамины.
void UFPSStaminaComponent::PlayStaminaChange(TEnumAsByte<EStaminaChange> TypeChange)
{
	// Если выбрана регенерация стамины.
	if (TypeChange == ST_Regeneration)
	{
		// Устанавливаем время за которое стамина дойдет до максимума.
		_staminaTiemLine.SetPlayRate(1.0f / _recoveryStamina);

		// Запускам таймлайн в обратную сторону.
		_staminaTiemLine.Reverse();
	}
	else if (TypeChange == ST_Reduction) // Если выбрана уменьшение стамины.
	{
		// Устанавливаем время за которое стамина дойдет до нуля.
		_staminaTiemLine.SetPlayRate(1.0f / _reductionStamina);

		// Запускаем таймлайн, стамина уменьшается.
		_staminaTiemLine.Play();
	}
	else // Если выбран рефреш стамины.
	{
		// Восстанавливаем стамину до максимума, моментально.
		SetStaminaAmmount(GetStaminaMax());
	}
}

