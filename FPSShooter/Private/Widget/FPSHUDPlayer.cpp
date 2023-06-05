// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/FPSHUDPlayer.h"

#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

// ����.
bool UFPSHUDPlayer::Initialize()
{
	Super::Initialize();

	return true;
}

// �����������.
void UFPSHUDPlayer::NativeConstruct()
{
	Super::NativeConstruct();
}

// ���������� �������.
void UFPSHUDPlayer::SetStamina(float NewStamina, float MaxStamina)
{
	// �������� ����� �������� ������� ����� 0 � ������������ ����������.
	const float _currentStamina = FMath::Clamp(
		NewStamina, // ����� ��������
		0.0f, // �����������.
		MaxStamina // ������������.
	);

	// ���������� ����� �������� �������.
	const float _newStamina = _currentStamina / MaxStamina;

	// ������������� ������������ � �������� ��� �������.
	PB_Stamina->SetPercent(_newStamina);
}

// ������������� ����� �������� �������� � Progress bar.
void UFPSHUDPlayer::SetHealth(float NewHealth, float MaxHealth)
{
	// �������� ����� �������� �������� ����� 0 � ������������ ����������.
	const float _currentHealth = FMath::Clamp(
		NewHealth, // ����� ��������
		0.0f, // �����������.
		MaxHealth // ������������.
	);

	// ���������� ����� �������� ��������.
	const float _newHealth = _currentHealth / MaxHealth;

	// ������������� �������� � �������� ��� ��������.
	PB_Health->SetPercent(_newHealth);
}