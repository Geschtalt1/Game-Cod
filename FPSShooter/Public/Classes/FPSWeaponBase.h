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
	// Конструктор базового класса оружия.
	AFPSWeaponBase(const FObjectInitializer& ObjectInitializer);

	// Взависимости от типа оружия, выполняет действие по нажатию ЛКМ.
	UFUNCTION(BlueprintCallable, Category = "Weapon Action")
	virtual void OnFire();

	// Перезарядка.
	UFUNCTION(BlueprintCallable, Category = "Weapon Action")
	virtual void OnReload();

	// Обработать место попадание.
	UFUNCTION(BlueprintCallable, Category = "Weapon Action")
	virtual void OnHandleHit(FHitResult Hit);

	// Setters.

	// Установить владельца оружия.
	UFUNCTION(BlueprintCallable, Category = "Weapon Owner")
	virtual void SetWeaponOwner(class AFPSSoldier* NewOwner);
	
	// Установить урон оружия.
	UFUNCTION(BlueprintCallable, Category = "Weapon Dammage")
	virtual void SetDammageWeapon(float NewDammage);

	// Програть звук оружия.
	UFUNCTION(BlueprintCallable, Category = "Weapon Sound")
	virtual void PlaySoundWeapon(class USoundBase* Sound, class USoundAttenuation* SoundAttenutaion = nullptr);

	// Getters.

	// Возращает владельца оружия.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon Owner")
	FORCEINLINE AFPSSoldier* GetWeaponOwner() const { return _ownerWeapon; }

	// Возращает урон оружия.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon Dammage")
	FORCEINLINE float GetDamage() const { return _totalDammage; }

	// Возращает длину луча при выстреле.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon Line Trace")
	FORCEINLINE float GetLenghtTrace() const { return _lenghtTrace; }

	// Возращает сокет на котором спавниться луч.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon Line Trace")
	FORCEINLINE FName GetMuzzleSocket() const { return _muzzleSocket; }

	// Спавн лайн трейса по на правлению оружия (дула).
	// Возращает true или false если попал.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon Line Trace")
	virtual bool SpawTraceToMuzzle(FHitResult& Result, float Lenght, bool bDebugLine = false) const;

	// Спавн лайн трейса по на правлению камеры.
	// Возращает true или false если попал.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon Line Trace")
	virtual bool SpawTraceFromCamera(FHitResult& Result, float Lenght, float Spread, bool bDebugLine = false) const;

	// Variable.

	// Делегат выстрела.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "On Fire", Category = "Default", MultiLine = "true", OverrideNativeName = "OnFire"))
	FOnFireSignatura OnFireWeapon;

protected:
	// Начало игры.
	virtual void BeginPlay() override;

	// Variable.

	// Звук стрельбы.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Fire Sound", NoResetToDefault, ClampMin = 0),
		Category = "Item Setting|Weapon Setting|Sound")
	class USoundCue* _fireSound;

	// Sound Attenuation.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Sound Attenuation", NoResetToDefault, ClampMin = 0),
		Category = "Item Setting|Weapon Setting|Sound|Attenuation")
	class USoundAttenuation* _soundAttenuation;

	// Урон оружия.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Total Dammage", NoResetToDefault, ClampMin = 0),
		Category = "Item Setting|Weapon Setting")
	float _totalDammage;

	// Длина луча при выстрели.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Lenght Trace", NoResetToDefault, ClampMin = 0),
		Category = "Item Setting|Weapon Setting")
	float _lenghtTrace;

	// Вкл или Выкл видимость линии лайнтрейса при выстрели.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Trace Debug", NoResetToDefault, ClampMin = 0),
		Category = "Item Setting|Weapon Setting|Advanced|Debug")
	bool _traceDebug;

private:	
	// Имя сокета спавна Line Trace.
	const FName _muzzleSocket = { "Muzzle" };

	// Владелец оружием.
	UPROPERTY()
	class AFPSSoldier* _ownerWeapon;
};
