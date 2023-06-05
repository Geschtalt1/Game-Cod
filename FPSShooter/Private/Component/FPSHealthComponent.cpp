// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/FPSHealthComponent.h"

#include "Classes/FPSUnitsBase.h"

#include "FPSHUD.h"

// ����������� ���������� ��������.
UFPSHealthComponent::UFPSHealthComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// ��� ��������.
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	_healthMax = 100.0f;
	_healthAmmount = 100.0f;
	_recoveryHealth = 1.0f;

	// ...
}

// ��� ����������� ���������� ��������.
void UFPSHealthComponent::PostInitProperties()
{
	Super::PostInitProperties();
}

// Called when the game starts
void UFPSHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ������������� � ������ �� �����, ������ ������������� ��������.
	SetHealthAmmount(_healthMax);

	// ...
	
}

// ���������� ������������ �������� �����.
void UFPSHealthComponent::SetHealthMax(float NewMax)
{
	_healthMax = abs(NewMax);
}

// ���������� ������� �������� �����.
float UFPSHealthComponent::SetHealthAmmount(float NewHealth)
{
	// ����� �������� �������� ��������, 
	// ������ ����� ���������� ���� � ������������ ���-�� ��.
	_healthAmmount = FMath::Clamp(NewHealth,
		0.0f, GetHealthMax());

	// ��������� ��� �������� �������.
	if (GetPlayerComponent())
	{
		// ������ �� HUD.
		const AFPSHUD* _hudLocal =
			GetPlayerComponent()->GetPlayerHUD();

		// ��������� ������ �� HUD.
		if (_hudLocal)
		{
			// �������� ������� ��������� ��������.
			_hudLocal->OnHealthChanged.Broadcast(
				_healthAmmount, // �������� ������� ��������.
				_healthMax); // �������� ������������ ��������.
		}
	}

	// ���������, ��� �� ����� ��� ������ ����.
	// ����� ���������, ��� ������� ������ �������� � �������.
	if (GetHealthCount() <= 0) {
		OnDead.Broadcast(); // �������� ������� ������.
	}

	// ��������� ������� ��������.
	return _healthAmmount;
}

// ��������� �����.
float UFPSHealthComponent::AnyDammage(float DamageAmount)
{
	// ����������� ����� �������� ��.
	// �� �������� �� �������� ���� ������.
	const float _newHealth = GetHealthCount() - DamageAmount;
	
	// ������������� ����� �������� ��.
	SetHealthAmmount(_newHealth);

	// ��������� ���������� ����.
	return DamageAmount;
}

