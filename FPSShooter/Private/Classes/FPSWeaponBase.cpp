// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/FPSWeaponBase.h"

#include "Classes/UnitsChild/FPSSoldier.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AFPSWeaponBase::AFPSWeaponBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	_totalDammage = 0.0f;
	_lenghtTrace = 10000.0f;

	_traceDebug = false;

	_fireSound = nullptr;
	_ownerWeapon = nullptr;
}

// Called when the game starts or when spawned
void AFPSWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// ������������ �� ���� ������, ��������� �������� �� ������� ���.
void AFPSWeaponBase::OnFire()
{
}

// �����������.
void AFPSWeaponBase::OnReload()
{
}

// ���������� ��������� �������.
void AFPSWeaponBase::SetWeaponOwner(AFPSSoldier* NewOwner)
{
	_ownerWeapon = NewOwner;
}

// ���������� ���� ������.
void AFPSWeaponBase::SetDammageWeapon(float NewDammage)
{
	_totalDammage = abs(NewDammage);
}

// ����� ���� �� ����.
bool AFPSWeaponBase::SpawTraceToMuzzle(FHitResult& Result, float Lenght, bool bDebugLine) const
{
	return false;
}

// ����� ���� �� ������.
bool AFPSWeaponBase::SpawTraceFromCamera(FHitResult& Result, float Lenght, float Spread, bool bDebugLine) const
{
	return false;
}

// ���������� ����� ���������.
void AFPSWeaponBase::OnHandleHit(FHitResult Hit)
{
	// ������ �� ������.
	AActor* _actorLocal = Hit.GetActor();

	// ���� ������ � ������.
	if (_actorLocal) 
	{
		// ��������� ��� ��� ������.
		AFPSSoldier* _soldierLoc = Cast<AFPSSoldier>(_actorLocal);

		// ���� �������� ��������� ��������.
		if (_soldierLoc)
		{
			// ������� ����� ����.
			_soldierLoc->TakeDamage(GetDamage(), // ���-�� ���������� �����.
				FDamageEvent(), // 
				nullptr, // ���������� ���� ��� ������� ����.
				GetWeaponOwner()); // ���-�� ������� ����.
		}
	}
}

// ��������� ���� ������.
void AFPSWeaponBase::PlaySoundWeapon(USoundBase* Sound, USoundAttenuation* SoundAttenutaion)
{
	if (Sound)
	{
		// ��� ���������������� ����.
		const FVector _locationSound = GetActorLocation();
		const FRotator _rotationSound = GetActorRotation();

		// �������� ���� �� �������.
		UGameplayStatics::PlaySoundAtLocation(GetWorld(),
			Sound, // ����� ���� �����������.
			_locationSound, // ����� ������������.
			_rotationSound, // ������� ������������.
			1.0f, // ��������� �����.
			1.0f,
			0.0f,
			SoundAttenutaion
		);
	}
}
