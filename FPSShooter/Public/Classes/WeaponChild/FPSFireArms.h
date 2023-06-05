/*--------------------------------------------------------------------------------------------------*/
/*                                  ������������� ������                                           */
/*------------------------------------------------------------------------------------------------*/

#pragma once

#include "CoreMinimal.h"
#include "Classes/FPSWeaponBase.h"
#include "Other/FPSEnumeration.h"
#include "FPSFireArms.generated.h"

UCLASS(Abstract)
class FPSSHOOTER_API AFPSFireArms : public AFPSWeaponBase
{
	GENERATED_BODY()

public:
	// ����������� �������������� ������.
	AFPSFireArms(const FObjectInitializer& ObjectInitializer);

	// ���� ����� �������� �� ����.
	bool SpawTraceToMuzzle(FHitResult& Result, float Lenght, bool bDebugLine = false) const override;

	// ���� ����� �������� �� ������.
	bool SpawTraceFromCamera(FHitResult& Result, float Lenght, float Spread, bool bDebugLine = false) const override;

	// ���������� ����� ����� ��������� �� �������.
	void OnBeginLookItem_Implementation(const class AFPSSoldier* Soldier) override;

	// ���������� ����� ����� �������� �������� �� �������.
	void OnEndLookItem_Implementation(const class AFPSSoldier* Soldier) override;

	// ������� �� ������.
	// ��������� ������� �������� ��� ����� ����������.
	void OnFire() override;

	// ����������� ������.
	// ��������� ������� ����������� ��� ����� ������.
	void OnReload() override;

	// ����� �����������.
	// // _bReload ����������� false.
	// ������������� ����� ���-�� �������� � �������.
	UFUNCTION(BlueprintCallable, Category = "Fire Arms Action")
	virtual void EndReload();

	// ���������� ��������.
	UFUNCTION(BlueprintCallable, Category = "Fire Arms Action")
	virtual void StopFire();

	// ����� ���� ��� ��������.
	UFUNCTION(BlueprintCallable, Category = "Fire Arms Effects")
	virtual void SpawnBullet(TSubclassOf<class AFPSBullet> Bullet, FHitResult Hit);

	// ����� ��������.
	UFUNCTION(BlueprintCallable, Category = "Fire Arms Effects")
	virtual void SpawnMuzzleEffect();

	// �������� ��������.
	UFUNCTION(BlueprintCallable, Category = "Fire Arms Spread")
	virtual float AddSpreadWeapon(float Value);

	// �������� ������� �������;
	UFUNCTION(BlueprintCallable, Category = "Fire Arms Spread")
	virtual void ResetSpread();

	// Setters.

	// ���������� ������������ ���-�� ��������.
	UFUNCTION(BlueprintCallable, Category = "Fire Arms Ammo")
	virtual void SetAmmoMax(int32 NewMax);

	// ���������� ������� ���-�� �������.
	UFUNCTION(BlueprintCallable, Category = "Fire Arms Ammo")
	virtual int32 SetAmmoCount(int32 NewCount);

	// ���������� ����� ����.
	UFUNCTION(BlueprintCallable, Category = "Fire Arms Fire Mode")
	virtual void SetFireMode(TEnumAsByte<EFireMode> FireMode);

	// ��������� �������.
	// ���������� ��������� ��������.
	// ���������� ������� ���������� �������� ��� ��������.
	// ���������� ������� ���������� �������� ��� �� ����������.
	// ���������� ������� ���������� �������� ��� �� ������������.
	UFUNCTION(BlueprintCallable, Category = "Fire Arms Spread")
	virtual float CalculateSpread(float Spread, 
		float PercentMove = 15.0f, 
		float PercentNotCrouch = 25.0f,
		float PercentNotAim = 35.0f);

	// Getters.

	// ��������� ������ ��������.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fire Arms Component")
	FORCEINLINE USkeletalMeshComponent* GetSkeletal() const { return SkeletalFireArms; }
	
	// ��������� ������� ���-�� ��������.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fire Arms Ammo")
	FORCEINLINE int32 GetAmmoCurrent() const { return _currentAmmo; }

	// ��������� ������������ ���-�� ��������.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fire Arms Ammo")
	FORCEINLINE int32 GetAmmoMax() const { return _maxAmmo; }

	// ��������� ���������������� ������.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fire Arms Ammo")
	FORCEINLINE float GetFireRate() const { return _fireRate; }

	// ��������� ���� �� ������� � ������ ������.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fire Arms Ammo")
	FORCEINLINE bool IsAmmo() const { return _currentAmmo > 0; }

	// ��������� �������������� �� ���� � ������ ������.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fire Arms Reload")
	FORCEINLINE bool IsReload() const { return _bReload; }

	// ��������� ��������� ������.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fire Arms Recoil")
	FORCEINLINE float GetRecoil() const { return _recoilWeapon; }
	
	// ��������� ��������� ��������.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fire Arms Spread")
	FORCEINLINE float GetSpread() const { return _bulletSpread; }

	// ��������� ������� ������� ��� ��������.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fire Arms Spread")
	FORCEINLINE float GetSpreadCurrent() const { return _currentSpread; }

	// ��������� ��� ��������.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fire Arms Fire Mode")
	FORCEINLINE TEnumAsByte<EFireMode> GetFireMode() const { return _fireMode; }

	// Variable.

	// ������ �������.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class USkeletalMeshComponent* SkeletalFireArms;

protected:

	// ��� ����������� �������������� ������.
	virtual void PostInitProperties() override; 

	// Variable.

	// ������������ ���-�� ������.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Max Ammo", NoResetToDefault, ClampMin = 0),
		Category = "Item Setting|Weapon Setting")
	int32 _maxAmmo;
		
	// ���������������� ������.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Fire Rate", NoResetToDefault, ClampMin = 0, Units = "times"),
		Category = "Item Setting|Weapon Setting")
	float _fireRate;

	// ��� ��������.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Fire Mode", NoResetToDefault),
		Category = "Item Setting|Weapon Setting")
	TEnumAsByte<EFireMode> _fireMode;

	// ��������� ������.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Recoil Weapon", NoResetToDefault, ClampMin = 0, ClampMax = 100),
		Category = "Item Setting|Weapon Setting")
	float _recoilWeapon;

	// ������� ������.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Bullet Spread", NoResetToDefault, ClampMin = 0, ClampMax = 100),
		Category = "Item Setting|Weapon Setting")
	float _bulletSpread;

	// ������������ ������� ������.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Bullet Spread Max", NoResetToDefault, ClampMin = 0),
		Category = "Item Setting|Weapon Setting|Advanced")
	float _bulletSpreadMax;

	// ��������� �� ������������ FireMode.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Enable Fire Mode", NoResetToDefault),
		Category = "Item Setting|Weapon Setting|Advanced")
	bool _bEnableFireMode;

	// ���� �����������.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Reload Sound", NoResetToDefault),
		Category = "Item Setting|Weapon Setting|Sound")
	class USoundCue* _reloadSound;

	// ���� �����������.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "No Ammo Sound", NoResetToDefault),
		Category = "Item Setting|Weapon Setting|Sound")
	class USoundCue* _noAmmoSound;

	// ���� ����� ������ ����.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Select Fire Mode", NoResetToDefault),
		Category = "Item Setting|Weapon Setting|Sound")
	class USoundCue* _selectFireMode;

	// ����� ����.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Bullet", NoResetToDefault),
		Category = "Item Setting|Weapon Setting|Effect")
	TSubclassOf<class AFPSBullet> _bulletTrace;

	// ������ ������� ��� ��������.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Muzzle", NoResetToDefault),
		Category = "Item Setting|Weapon Setting|Effect")
	class UNiagaraSystem* _muzzleEffect;

	// �������� �������. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Breech Mechanism", NoResetToDefault),
		Category = "Item Setting|Weapon Setting|Animation|Weapon")
	class UAnimationAsset* _breechMechanism;

	// �������� �� ������ ����������� (�������). 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Reload Loaded", NoResetToDefault),
		Category = "Item Setting|Weapon Setting|Animation|Pawn")
	class UAnimMontage* _reloadLoadedPawn;

	// �������� ������ ����������� (�������). 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Reload Empty", NoResetToDefault),
		Category = "Item Setting|Weapon Setting|Animation|Pawn")
	class UAnimMontage* _reloadEmptyPawn;

private:

	// Function.
	
	// Variable.

	// ������� ���-�� ��������.
	UPROPERTY()
	int32 _currentAmmo;
	
	// ������� �������.
	UPROPERTY()
	float _currentSpread;

	// ����������� � ������ ������.
	UPROPERTY()
	bool _bReload;

	// ������ ��������.
	UPROPERTY()
	FTimerHandle _thFireMain;
};
