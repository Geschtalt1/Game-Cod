// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/FPSHUDPlayer.h"

#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

// Инит.
bool UFPSHUDPlayer::Initialize()
{
	Super::Initialize();

	return true;
}

// Конструктор.
void UFPSHUDPlayer::NativeConstruct()
{
	Super::NativeConstruct();
}

// Установить стамину.
void UFPSHUDPlayer::SetStamina(float NewStamina, float MaxStamina)
{
	// Зажимаем новое значение стамины между 0 и максимальным допустимым.
	const float _currentStamina = FMath::Clamp(
		NewStamina, // Новое значение
		0.0f, // Минимальное.
		MaxStamina // Максимальное.
	);

	// Выщитываем новое значение стамины.
	const float _newStamina = _currentStamina / MaxStamina;

	// Устанавливаем выносливость в прогресс бар стамины.
	PB_Stamina->SetPercent(_newStamina);
}

// Устанавливает новое значение здоровья в Progress bar.
void UFPSHUDPlayer::SetHealth(float NewHealth, float MaxHealth)
{
	// Зажимаем новое значение здоровья между 0 и максимальным допустимым.
	const float _currentHealth = FMath::Clamp(
		NewHealth, // Новое значение
		0.0f, // Минимальное.
		MaxHealth // Максимальное.
	);

	// Выщитываем новое значение здоровья.
	const float _newHealth = _currentHealth / MaxHealth;

	// Устанавливаем здоровье в прогресс бар здоровья.
	PB_Health->SetPercent(_newHealth);
}