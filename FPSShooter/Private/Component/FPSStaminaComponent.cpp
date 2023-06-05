// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/FPSStaminaComponent.h"

#include "Classes/FPSUnitsBase.h"
#include "FPSHUD.h"

#include "Engine/EngineTypes.h"
#include "TimerManager.h"

// �����������.
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

// ��� ������������.
void UFPSStaminaComponent::PostInitProperties()
{
	Super::PostInitProperties();
}

// ����� ����.
void UFPSStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	// � ������ ����, ������������� ������� ������� ������ ������������.
	SetStaminaAmmount(_staminaMax);

	// ��������� ��� ������ ������� �������.
	if (_curveStamina)
	{
		// �������.
		FOnTimelineFloat _stamina;

		// ����������� ������� � ��������.
		_stamina.BindUFunction(this, FName("AddStamina"));

		// ������� �������� ������������.
		_staminaTiemLine.AddInterpFloat(_curveStamina, _stamina);
	}

	// ...

}

// ���.
void UFPSStaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	_staminaTiemLine.TickTimeline(DeltaTime);
}

// ���������� ������������ ������������ �����.
void UFPSStaminaComponent::SetStaminaMax(float NewMax)
{
	_staminaMax = abs(NewMax);
}

// ���������� ������� ������������ �����.
float UFPSStaminaComponent::SetStaminaAmmount(float NewHealth)
{
	// �������� ����� �������� ����� 0 � ������������ ���������� ��������� �������.
	_staminaAmmount = FMath::Clamp(NewHealth, // ����������� ��������.
		0.0f, // ����������� ��������.
		_staminaMax // ������������ ��������.
	);

	// ��������� ��� �������� �������.
	if (GetPlayerComponent())
	{
		// ������ �� HUD.
		const AFPSHUD* _hudLocal =
			GetPlayerComponent()->GetPlayerHUD();

		// ��������� ������ �� HUD.
		if (_hudLocal)
		{
			// �������� ������� ��������� �������.
			_hudLocal->OnStaminaChanged.Broadcast(
				_staminaAmmount, // �������� ������� ��������.
				_staminaMax); // �������� ������������ ��������.
		}
	}

	// ��������� �������� �������.
	return _staminaAmmount;
}

// ������������� ��� - �� �������.
float UFPSStaminaComponent::AddStamina(float Value)
{
	if (GetPlayerComponent())
	{
		// ����������� ����� �������� �������.
		const float _newStamina = FMath::Lerp(
			GetStaminaMax(), // ������� ���-�� �������.
			0.0, // �������� �� �������� ������� ����������.
			Value // �����.
		);

		// ���������� ����� �������� �������.
		const float _currentStamina = 
			SetStaminaAmmount(_newStamina);

		// ���������, ���� ����� �������� ������� ������ ����� ����.
		if (_newStamina <= 0.0f)
		{
			// ��������� ���������.
			GetPlayerComponent()->SetSpeedRun(false);
			//GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, TEXT("1!"));
		}

		// ��������� �������� �������.
		return _newStamina;
	}

	// ��������� �������� �������.
	return 0.0f;
}

// �������� ��� ��������� �������.
void UFPSStaminaComponent::PlayStaminaChange(TEnumAsByte<EStaminaChange> TypeChange)
{
	// ���� ������� ����������� �������.
	if (TypeChange == ST_Regeneration)
	{
		// ������������� ����� �� ������� ������� ������ �� ���������.
		_staminaTiemLine.SetPlayRate(1.0f / _recoveryStamina);

		// �������� �������� � �������� �������.
		_staminaTiemLine.Reverse();
	}
	else if (TypeChange == ST_Reduction) // ���� ������� ���������� �������.
	{
		// ������������� ����� �� ������� ������� ������ �� ����.
		_staminaTiemLine.SetPlayRate(1.0f / _reductionStamina);

		// ��������� ��������, ������� �����������.
		_staminaTiemLine.Play();
	}
	else // ���� ������ ������ �������.
	{
		// ��������������� ������� �� ���������, �����������.
		SetStaminaAmmount(GetStaminaMax());
	}
}

