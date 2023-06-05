// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Classes/FPSItemBase.h"

#include "FPSWeaponBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFireSignatura);

UCLASS(Abstract)
class FPSSHOOTER_API AFPSWeaponBase : public AFPSItemBase
{
	GENERATED_BODY()
	
public:	
	// ����������� �������� ������ ������.
	AFPSWeaponBase(const FObjectInitializer& ObjectInitializer);

	// ������������ �� ���� ������, ��������� �������� �� ������� ���.
	UFUNCTION(BlueprintCallable, Category = "Weapon Action")
	virtual void OnFire();

	// �����������.
	UFUNCTION(BlueprintCallable, Category = "Weapon Action")
	virtual void OnReload();

	// ���������� ����� ���������.
	UFUNCTION(BlueprintCallable, Category = "Weapon Action")
	virtual void OnHandleHit(FHitResult Hit);

	// Setters.

	// ���������� ��������� ������.
	UFUNCTION(BlueprintCallable, Category = "Weapon Owner")
	virtual void SetWeaponOwner(class AFPSSoldier* NewOwner);
	
	// ���������� ���� ������.
	UFUNCTION(BlueprintCallable, Category = "Weapon Dammage")
	virtual void SetDammageWeapon(float NewDammage);

	// �������� ���� ������.
	UFUNCTION(BlueprintCallable, Category = "Weapon Sound")
	virtual void PlaySoundWeapon(class USoundBase* Sound, class USoundAttenuation* SoundAttenutaion = nullptr);

	// Getters.

	// ��������� ��������� ������.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon Owner")
	FORCEINLINE AFPSSoldier* GetWeaponOwner() const { return _ownerWeapon; }

	// ��������� ���� ������.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon Dammage")
	FORCEINLINE float GetDamage() const { return _totalDammage; }

	// ��������� ����� ���� ��� ��������.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon Line Trace")
	FORCEINLINE float GetLenghtTrace() const { return _lenghtTrace; }

	// ��������� ����� �� ������� ���������� ���.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon Line Trace")
	FORCEINLINE FName GetMuzzleSocket() const { return _muzzleSocket; }

	// ����� ���� ������ �� �� ��������� ������ (����).
	// ��������� true ��� false ���� �����.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon Line Trace")
	virtual bool SpawTraceToMuzzle(FHitResult& Result, float Lenght, bool bDebugLine = false) const;

	// ����� ���� ������ �� �� ��������� ������.
	// ��������� true ��� false ���� �����.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon Line Trace")
	virtual bool SpawTraceFromCamera(FHitResult& Result, float Lenght, float Spread, bool bDebugLine = false) const;

	// Variable.

	// ������� ��������.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "On Fire", Category = "Default", MultiLine = "true", OverrideNativeName = "OnFire"))
	FOnFireSignatura OnFireWeapon;

protected:
	// ������ ����.
	virtual void BeginPlay() override;

	// Variable.

	// ���� ��������.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Fire Sound", NoResetToDefault, ClampMin = 0),
		Category = "Item Setting|Weapon Setting|Sound")
	class USoundCue* _fireSound;

	// Sound Attenuation.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Sound Attenuation", NoResetToDefault, ClampMin = 0),
		Category = "Item Setting|Weapon Setting|Sound|Attenuation")
	class USoundAttenuation* _soundAttenuation;

	// ���� ������.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Total Dammage", NoResetToDefault, ClampMin = 0),
		Category = "Item Setting|Weapon Setting")
	float _totalDammage;

	// ����� ���� ��� ��������.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Lenght Trace", NoResetToDefault, ClampMin = 0),
		Category = "Item Setting|Weapon Setting")
	float _lenghtTrace;

	// ��� ��� ���� ��������� ����� ���������� ��� ��������.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Trace Debug", NoResetToDefault, ClampMin = 0),
		Category = "Item Setting|Weapon Setting|Advanced|Debug")
	bool _traceDebug;

private:	
	// ��� ������ ������ Line Trace.
	const FName _muzzleSocket = { "Muzzle" };

	// �������� �������.
	UPROPERTY()
	class AFPSSoldier* _ownerWeapon;
};
