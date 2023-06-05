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

// Взависимости от типа оружия, выполняет действие по нажатию ЛКМ.
void AFPSWeaponBase::OnFire()
{
}

// Перезарядка.
void AFPSWeaponBase::OnReload()
{
}

// Установить владельца оружием.
void AFPSWeaponBase::SetWeaponOwner(AFPSSoldier* NewOwner)
{
	_ownerWeapon = NewOwner;
}

// Установить урон оружия.
void AFPSWeaponBase::SetDammageWeapon(float NewDammage)
{
	_totalDammage = abs(NewDammage);
}

// Спавн луча из дула.
bool AFPSWeaponBase::SpawTraceToMuzzle(FHitResult& Result, float Lenght, bool bDebugLine) const
{
	return false;
}

// Спавн луча из камеры.
bool AFPSWeaponBase::SpawTraceFromCamera(FHitResult& Result, float Lenght, float Spread, bool bDebugLine) const
{
	return false;
}

// Обработать место попадание.
void AFPSWeaponBase::OnHandleHit(FHitResult Hit)
{
	// Ссылка на актора.
	AActor* _actorLocal = Hit.GetActor();

	// Если попали в актора.
	if (_actorLocal) 
	{
		// Проверяем что это солдат.
		AFPSSoldier* _soldierLoc = Cast<AFPSSoldier>(_actorLocal);

		// Если проверка оказалась успешной.
		if (_soldierLoc)
		{
			// Наносим юниту урон.
			_soldierLoc->TakeDamage(GetDamage(), // Кол-во наносимого урона.
				FDamageEvent(), // 
				nullptr, // Контроллер того кто наносит урон.
				GetWeaponOwner()); // Кто-то наносит урон.
		}
	}
}

// Проиграть звук оружия.
void AFPSWeaponBase::PlaySoundWeapon(USoundBase* Sound, USoundAttenuation* SoundAttenutaion)
{
	if (Sound)
	{
		// Где воспроизводиться звук.
		const FVector _locationSound = GetActorLocation();
		const FRotator _rotationSound = GetActorRotation();

		// Включаем звук на локации.
		UGameplayStatics::PlaySoundAtLocation(GetWorld(),
			Sound, // Какой звук проигрываем.
			_locationSound, // Место проигрование.
			_rotationSound, // Поворот проигрование.
			1.0f, // Громкость звука.
			1.0f,
			0.0f,
			SoundAttenutaion
		);
	}
}
