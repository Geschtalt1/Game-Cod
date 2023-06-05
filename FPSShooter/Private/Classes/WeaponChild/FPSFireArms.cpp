// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/WeaponChild/FPSFireArms.h"
#include "Classes/UnitsChild/FPSSoldier.h"
#include "Component/FPSMovementComponent.h"
#include "FPSPlayerController.h"
#include "Other/FPSBullet.h"
#include "Other/FPSTypename.h"
#include "FPSHUD.h"

#include "Animation/AnimationAsset.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Camera/CameraComponent.h"

#include "Sound/SoundCue.h"
#include "Other/FPSTypename.h"

#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

#include "Blueprint/WidgetLayoutLibrary.h"

#include "Components/SkeletalMeshComponent.h"

// ����������� �������������� ������.
AFPSFireArms::AFPSFireArms(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// ��������� ���� �������� ������.
	ItemMesh->DestroyComponent();

	// ��������� ������� �������������� ������.
	SkeletalFireArms = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
	SkeletalFireArms->SetCustomDepthStencilValue(1);
	SkeletalFireArms->SetGenerateOverlapEvents(true);
	SkeletalFireArms->SetCollisionObjectType(CHANNELS_ITEM);
	SkeletalFireArms->SetCollisionResponseToChannel(CHANNELS_ITEM, ECR_Block);
	SkeletalFireArms->SetCollisionResponseToChannel(CHANNELS_ITEM_LINE, ECR_Block);
	SetRootComponent(SkeletalFireArms);

	_fireRate = 0.0f;
	_maxAmmo = 0;
	_recoilWeapon = 0.0f;
	_currentAmmo = 0;
	_bulletSpreadMax = 1000.0f;
	_bulletSpread = 0.0f;

	_currentSpread = 0.0f;

	_bReload = false;
	_bEnableFireMode = true;

	_reloadSound = nullptr;
	_bulletTrace = nullptr;

	_breechMechanism = nullptr;
	_reloadLoadedPawn = nullptr;
	_reloadEmptyPawn = nullptr;
	_selectFireMode = nullptr;

	_fireMode = FM_Full;
}

// ��� ����������� �������������� ������.
void AFPSFireArms::PostInitProperties()
{
	Super::PostInitProperties();

	// ������������� ������� ���-�� ��������, ������ �������������.
	SetAmmoCount(_maxAmmo);
}

// ������� �� ������.
void AFPSFireArms::OnFire()
{
	if (IsAmmo()) // ���� ������� � ������ ����.
	{
		// �������� ������� ��������.
		OnFireWeapon.Broadcast();

		// ���������� ��� ���������� ��������.
		FHitResult Hit;

		// ������������ �� ������� ����������� �������.
		const float _addSpread = CalculateSpread(GetSpread());
		AddSpreadWeapon(_addSpread);

		// ���������� ���� ��������.
		PlaySoundWeapon(_fireSound, _soundAttenuation);

		// ������� ������� ��������.
		SpawnMuzzleEffect();

		// ������� ���.
		const bool bHit = SpawTraceFromCamera(Hit,  // ��������� ��������� ��������.
			GetLenghtTrace(), // ����� ���� ��� ��������.
			GetSpreadCurrent(), // ������� ����.
			_traceDebug); // ��� - ���� ��������� ���� ��������.

		// ����� ����.
		SpawnBullet(_bulletTrace, Hit);

		// �������� �������.
		GetSkeletal()->PlayAnimation(_breechMechanism, false);

		// ���� �������� ������ �������.
		if (GetWeaponOwner())
		{
			// �������� ������� ������.
			GetWeaponOwner()->_recoilTiemLine.PlayFromStart();
			
			// ���������� ������ �� HUD.
			AFPSHUD* _hudLocal = GetWeaponOwner()->
				GetPlayerHUD();

			// ��������� ������ �� HUD.
			if (_hudLocal) 
			{
				// ��������� ������ � ������.
				_hudLocal->OnCrosshireChanged.Broadcast(GetSpread());
			}
		}

		// ����������� ����� ���-�� �����������.
		// �� �������� ���-�� ��������, �������� ����.
		const int32 _ammoNew = GetAmmoCurrent() - 1;
		SetAmmoCount(_ammoNew);

		// ������������ ����� ���������, ���� ��� �������� �����������.
		if (bHit) {
			OnHandleHit(Hit);
		}

		// ���� ��� �������� ������ ��������������.
		if (GetFireMode() == FM_Full) 
		{			
			// ������.
			GetWorld()->GetTimerManager().SetTimer(_thFireMain,
			GetWeaponOwner(),
			&AFPSSoldier::OnFireMainUnit, // �������� ������� ��������.
			GetFireRate(), // ����������������.
			true // �����������.
			); // ������ ������ � ���������.
		}
		else // ���� ����� ���� ���������.
		{
			// ������� �������.
			StopFire();
		}
	}
	else
	{
		// ���� �������� ���, ����������� ���� ������.
		PlaySoundWeapon(_noAmmoSound, _soundAttenuation);

		// � ������������� �������� ���� ��� ���� ���������.
		StopFire();
	}
}

// ����������� ������.
void AFPSFireArms::OnReload()
{
	// ���� �������� ����������.
	if (GetWeaponOwner())
	{
		// ���������, ��� ������� � ��������� �� ������.
		if (GetAmmoCurrent() != GetAmmoMax()) 
		{
			// ���� ������� �� ������, �������� �����������.
			_bReload = true;
		}
		else 
		{
			// ���� ������� ������, ��������� ������ �������.
			return;
		}

		// ��������� ��������.
		StopFire();

		// ����� ������������ �������.
		float _timeMontage;

		// ������ �������� ������� ������� EndReload.
		FTimerHandle TH_Reload;

		// ���� ���-�� ��������, �� ����� ����, 
		// �������� ����������� ����� �� ������.
		if (GetAmmoCurrent() != 0)
		{
			// �������� ���� ������ �� ������ �����������.
			_timeMontage = GetWeaponOwner()->
				PlayAnimationMontage(_reloadLoadedPawn);

			// ��������� ������� ����� ����������� �� ��������� ������� �������.
			GetWorld()->GetTimerManager().SetTimer(
				TH_Reload,               // ������.
				this,                    // � ����� ������ ����������.
				&AFPSFireArms::EndReload,// ����� ������� ����������.
				_timeMontage,            // ����� ����� ������ ���������� �����.
				false                    // ������������.
			);

		}
		else // ���� ���-�� ����� ����, �������� ����������� ����� ������.
		{
			// ������� ���� ������ ������ �����������.
			_timeMontage = GetWeaponOwner()->
				PlayAnimationMontage(_reloadEmptyPawn);

			// ��������� ������� ����� ����������� �� ��������� ������� �������.
			GetWorld()->GetTimerManager().SetTimer(
				TH_Reload,               // ������.
				this,                    // � ����� ������ ����������.
				&AFPSFireArms::EndReload,// ����� ������� ����������.
				_timeMontage,            // ����� ����� ������ ���������� �����.
				false                    // ������������.
			);
		}
	}
	else // ���� ��������� ���, ��������� ������� ��� �������� �����������.
	{
		SetAmmoCount(GetAmmoMax());

		// � ������������� �������� ���� ��� ���� ���������.
		StopFire();
	}
}

bool AFPSFireArms::SpawTraceToMuzzle(FHitResult& Result, float Lenght, bool bDebugLine) const
{
	// ��� ������ ��� ���������� ���.
	const FName _socketName = GetMuzzleSocket();

	// ����������� ����� ���� � ������ ������.
	const FVector _lenghtTraceLoc = SkeletalFireArms->GetForwardVector() * Lenght;
	const FVector _startTrace = SkeletalFireArms->GetSocketLocation(_socketName);

	// ����������� ����� ������, ���������� ����� � ������ ����.
	const FVector _endTrace = _lenghtTraceLoc + _startTrace;

	// ��������� ����.
	FCollisionQueryParams _traceParams;

	// ������� ���.
	const bool bHit = GetWorld()->LineTraceSingleByChannel(
		Result, // ��������� ��������� ��������� ����. 
		_startTrace, // ������ ����.
		_endTrace, // ����� ����.
		CHANNELS_BULLET_LINE, // �������� ����.
		_traceParams // ��������� ��������.
	);

	if (bDebugLine) // ���������� ����� ���� ������, ���� ����� �������.
	{
		// ���������� �����.
		DrawDebugLine(GetWorld(), _startTrace, _endTrace, FColor::Red, false, 2.0f);

		// ���� ����� �� ���-�� �����, �� ����� ��������� ���������� �������.
		if (bHit) {
			DrawDebugBox(GetWorld(), Result.ImpactPoint, FVector(5, 5, 5), FColor::Emerald, false, 2.0f);
		}
	}

	return bHit;
}

bool AFPSFireArms::SpawTraceFromCamera(FHitResult& Result, float Lenght, float Spread, bool bDebugLine) const
{
	// ������ �� ������ ������.
	const UCameraComponent* _cameraComp = 
		GetWeaponOwner()->GetCamera();

	// ���� �������� ������������.
	if (GetWeaponOwner())
	{
		// ������� ����.
		FVector _spray = FVector(UKismetMathLibrary::RandomFloatInRange(Spread * (-1), Spread), // X
			UKismetMathLibrary::RandomFloatInRange(Spread * (-1), Spread), // Y
			UKismetMathLibrary::RandomFloatInRange(Spread * (-1), Spread)); // Z

		// ���������� ����.
		const FVector _startTrace = _cameraComp->GetComponentLocation();
		const FVector _endTrace = ((_cameraComp->GetForwardVector() * Lenght) +
			_startTrace) + _spray;

		// ��������� ����.
		FCollisionQueryParams _traceParams;

		// ������� ���.
		const bool bHit = GetWorld()->LineTraceSingleByChannel(
			Result, // ��������� ��������� ��������� ����. 
			_startTrace, // ������ ����.
			_endTrace, // ����� ����.
			CHANNELS_BULLET_LINE, // �������� ����.
			_traceParams // ��������� ��������.
		);

		if (bDebugLine) // ���������� ����� ���� ������, ���� ����� �������.
		{
			// ���������� �����.
			DrawDebugLine(GetWorld(), _startTrace, _endTrace, FColor::Red, false, 2.0f);

			// ���� ����� �� ���-�� �����, �� ����� ��������� ���������� �������.
			if (bHit) {
				DrawDebugBox(GetWorld(), Result.ImpactPoint, FVector(5, 5, 5), FColor::Emerald, false, 2.0f);
			}
		}

		return bHit;
	}

	return false;
}

// ���������� ����� ����.
void AFPSFireArms::SetFireMode(TEnumAsByte<EFireMode> FireMode)
{
	// ��������� ���� ����� ������ ���� ���������.
	if (_bEnableFireMode)
	{
		// ������ ����� ����.
		_fireMode = FireMode;

		// ���������� ���� ����� ������ ����.
		PlaySoundWeapon(_selectFireMode, _soundAttenuation);
	}
}

// ���������� ������������ ���-�� ��������.
void AFPSFireArms::SetAmmoMax(int32 NewMax)
{
	_maxAmmo = abs(NewMax);
}

// ���������� ������� ���-�� ��������.
int32 AFPSFireArms::SetAmmoCount(int32 NewCount)
{
	_currentAmmo = FMath::Clamp(NewCount, // ����� �������� ������� ��������.
		NULL, // ����������� �������� ������� ����� ���� � ��������.
		GetAmmoMax()); // ������������ �������� ������� ����� ���� � ��������.

	return _currentAmmo;
}

// ����� ���� ��� ��������.
void AFPSFireArms::SpawnBullet(TSubclassOf<AFPSBullet> Bullet, FHitResult Hit)
{
	if (Bullet)
	{
		// ��������� ����� �� ������� ����� ���������� ����.
		const FName _socketBullet = GetMuzzleSocket();

		// ���������� ������������ ������ ��� ���������� ����.
		const FVector _locationSpawn = GetSkeletal()->GetSocketLocation(_socketBullet);

		// ���������� ������ ��� ������ �������� ������, � ����������� ����� ��� �� ����� ��� �� ��� ������.
		const FVector _locationTrace = UKismetMathLibrary::SelectVector(
			Hit.ImpactPoint, // ������� ���� � �������� �� ����.
			Hit.TraceEnd, // ����� ������.
			Hit.bBlockingHit // �������� ��� �� �������� ��� ����������.
		);

		// ������� ����������� ������ ���� �� ����.
		const FRotator _directionSpawn = UKismetMathLibrary::FindLookAtRotation(
			_locationSpawn,
			_locationTrace
		);

		// ������� ����.
		GetWorld()->SpawnActor<AFPSBullet>(
			Bullet, // ����� ����.
			_locationSpawn, // ����� ������.
			_directionSpawn); // ����������� ������ ����.
	}
}

// ����� �������.
void AFPSFireArms::SpawnMuzzleEffect()
{
	if (_muzzleEffect)
	{
		// ���������� ��� ������ �� ������.
		const FName _socketMuzzle = GetMuzzleSocket();
		
		// ����� ���������� ������������ ������ �� ������.
		const FVector _socketLocation = 
			GetSkeletal()->GetSocketLocation(_socketMuzzle);

		// ����� �������� ������ �� ������.
		const FRotator _socketRotation = 
			GetSkeletal()->GetSocketRotation(_socketMuzzle);

		// ������� ������ ��������.
		UNiagaraFunctionLibrary::SpawnSystemAttached(
			_muzzleEffect, // ������ �������.
			GetSkeletal(), // � ���� �������� ������.
			FName{}, // 
			_socketLocation, // ����� ������.
			_socketRotation, // ������� �������.
			EAttachLocation::KeepWorldPosition, // ��� ��������� � �������.
			true // �������������� �����������.
		);
	}
}

// ��������� �������.
// ���������� ��������� ��������.
// ���������� ������� ���������� �������� ��� ��������.
// ���������� ������� ���������� �������� ��� �� ����������.
// ���������� ������� ���������� �������� ��� �� ������������.
float AFPSFireArms::CalculateSpread(float Spread, float PercentMove, float PercentNotCrouch, float PercentNotAim)
{
	if (GetWeaponOwner())
	{
		// ������ �� �����.
		const AFPSSoldier& _soldier = *GetWeaponOwner();

		// ���������� ��� �������� ��������� ����� � ������ ������.
		const bool _bMove = _soldier.GetMovement()->IsMovingOnGround();
		const bool _bCrouch = _soldier.IsCrouchSoldier();
		const bool _bAim = _soldier.IsAimUnit();

		if (_bMove) // ���� ���� ���������, ����������� �������.
		{
			Spread += (Spread / 100.0f) * PercentMove;
		}

		if (!_bCrouch) // ���� ���� �� ���������, ����������� �������.  
		{
			Spread += (Spread / 100.0f) * PercentNotCrouch;
		}

		if (!_bAim) // ���� ���� �� �������������, ����������� �������.
		{
			Spread += (Spread / 100.0f) * PercentNotAim;
		}

		return Spread; // ��������� �������.
	}

	return 0.0f;
}

// ���������� ������� �������.
void AFPSFireArms::ResetSpread()
{
	_currentSpread = 0.0f;
}

// �������� ��������.
float AFPSFireArms::AddSpreadWeapon(float Value)
{
	// ������� �������.
	_currentSpread = FMath::Clamp(
		GetSpreadCurrent() + Value, // ���������� � �������� ��������.
		0.0f, // ����������� �������.
		_bulletSpreadMax // ������������ �������.
	);
	
	// ��������� ������� �������.
	return _currentSpread;
}

// ����� �����������.
void AFPSFireArms::EndReload()
{
	// ��������� �����������.
	_bReload = false;

	// ��������� ������� � ��������.
	SetAmmoCount(GetAmmoMax());
}

// ���������� ��������.
void AFPSFireArms::StopFire()
{
	// ���� ����� ���� ������.
	if (GetFireMode() == FM_Full) 
	{
		// ������� ������.
		GetWorld()->GetTimerManager().ClearTimer(_thFireMain);
	}

	// ���������� �������.
	ResetSpread();
}

// ���������� ����� ����� ��������� �� �������.
void AFPSFireArms::OnBeginLookItem_Implementation(const AFPSSoldier* Soldier)
{
	// �������� ������� ������.
	SkeletalFireArms->SetRenderCustomDepth(true);
}

// ���������� ����� ����� �������� �������� �� �������.
void AFPSFireArms::OnEndLookItem_Implementation(const AFPSSoldier* Soldier)
{
	// ��������� ������� ������.
	SkeletalFireArms->SetRenderCustomDepth(false);
}