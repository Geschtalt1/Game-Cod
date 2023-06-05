/*--------------------------------------------------------------------------------------------------*/
/*                                  ОГНЕСТРЕЛЬНОЕ ОРУЖИЕ                                           */
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
	// Конструктор огнестрельного оружия.
	AFPSFireArms(const FObjectInitializer& ObjectInitializer);

	// Лайн трейс выстрела из дула.
	bool SpawTraceToMuzzle(FHitResult& Result, float Lenght, bool bDebugLine = false) const override;

	// Лайн трейс выстрела из камеры.
	bool SpawTraceFromCamera(FHitResult& Result, float Lenght, float Spread, bool bDebugLine = false) const override;

	// Вызывается когда игрок посмотрел на предмет.
	void OnBeginLookItem_Implementation(const class AFPSSoldier* Soldier) override;

	// Вызывается когда игрок закончил смотреть на предмет.
	void OnEndLookItem_Implementation(const class AFPSSoldier* Soldier) override;

	// Выстрел из оружия.
	// Дефолтная функция стрельбы для всего огнестрела.
	void OnFire() override;

	// Перезарядка оружия.
	// Дефолтная функция перезарядки для всего оружия.
	void OnReload() override;

	// Конец перезарядки.
	// // _bReload становиться false.
	// Устанавливает новое кол-во патронов в магазин.
	UFUNCTION(BlueprintCallable, Category = "Fire Arms Action")
	virtual void EndReload();

	// Остановить стрельбу.
	UFUNCTION(BlueprintCallable, Category = "Fire Arms Action")
	virtual void StopFire();

	// Спавн пули при выстреле.
	UFUNCTION(BlueprintCallable, Category = "Fire Arms Effects")
	virtual void SpawnBullet(TSubclassOf<class AFPSBullet> Bullet, FHitResult Hit);

	// Спавн эффектов.
	UFUNCTION(BlueprintCallable, Category = "Fire Arms Effects")
	virtual void SpawnMuzzleEffect();

	// Добавить разбросс.
	UFUNCTION(BlueprintCallable, Category = "Fire Arms Spread")
	virtual float AddSpreadWeapon(float Value);

	// Сбросить текущий разброс;
	UFUNCTION(BlueprintCallable, Category = "Fire Arms Spread")
	virtual void ResetSpread();

	// Setters.

	// Установить максимальное кол-во патронов.
	UFUNCTION(BlueprintCallable, Category = "Fire Arms Ammo")
	virtual void SetAmmoMax(int32 NewMax);

	// Установить текущее кол-во птронов.
	UFUNCTION(BlueprintCallable, Category = "Fire Arms Ammo")
	virtual int32 SetAmmoCount(int32 NewCount);

	// Установить режим огня.
	UFUNCTION(BlueprintCallable, Category = "Fire Arms Fire Mode")
	virtual void SetFireMode(TEnumAsByte<EFireMode> FireMode);

	// Расчитать разброс.
	// Установить множитель разброса.
	// Установить процент увелечения разброса при движение.
	// Установить процент увелечения разброса при не приседание.
	// Установить процент увелечения разброса при не прицеливание.
	UFUNCTION(BlueprintCallable, Category = "Fire Arms Spread")
	virtual float CalculateSpread(float Spread, 
		float PercentMove = 15.0f, 
		float PercentNotCrouch = 25.0f,
		float PercentNotAim = 35.0f);

	// Getters.

	// Возращает скелет винтовки.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fire Arms Component")
	FORCEINLINE USkeletalMeshComponent* GetSkeletal() const { return SkeletalFireArms; }
	
	// Возращает текущее кол-во патронов.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fire Arms Ammo")
	FORCEINLINE int32 GetAmmoCurrent() const { return _currentAmmo; }

	// Возращает максимальное кол-во патронов.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fire Arms Ammo")
	FORCEINLINE int32 GetAmmoMax() const { return _maxAmmo; }

	// Возращает скорострельность оружия.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fire Arms Ammo")
	FORCEINLINE float GetFireRate() const { return _fireRate; }

	// Возращает есть ли патроны в данный момент.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fire Arms Ammo")
	FORCEINLINE bool IsAmmo() const { return _currentAmmo > 0; }

	// Возращает перезарежается ли юнит в данный момент.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fire Arms Reload")
	FORCEINLINE bool IsReload() const { return _bReload; }

	// Возращает множетель отдачи.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fire Arms Recoil")
	FORCEINLINE float GetRecoil() const { return _recoilWeapon; }
	
	// Возращает множитель разброса.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fire Arms Spread")
	FORCEINLINE float GetSpread() const { return _bulletSpread; }

	// Возращает текущий разброс при стрельбе.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fire Arms Spread")
	FORCEINLINE float GetSpreadCurrent() const { return _currentSpread; }

	// Возращает тип стрельбы.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fire Arms Fire Mode")
	FORCEINLINE TEnumAsByte<EFireMode> GetFireMode() const { return _fireMode; }

	// Variable.

	// Скелет солдата.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class USkeletalMeshComponent* SkeletalFireArms;

protected:

	// Пре инцилизация огнестрельного оружия.
	virtual void PostInitProperties() override; 

	// Variable.

	// Максимальное кол-во патрон.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Max Ammo", NoResetToDefault, ClampMin = 0),
		Category = "Item Setting|Weapon Setting")
	int32 _maxAmmo;
		
	// Скорострельность оружия.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Fire Rate", NoResetToDefault, ClampMin = 0, Units = "times"),
		Category = "Item Setting|Weapon Setting")
	float _fireRate;

	// Тип стрельбы.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Fire Mode", NoResetToDefault),
		Category = "Item Setting|Weapon Setting")
	TEnumAsByte<EFireMode> _fireMode;

	// Множитель отдачи.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Recoil Weapon", NoResetToDefault, ClampMin = 0, ClampMax = 100),
		Category = "Item Setting|Weapon Setting")
	float _recoilWeapon;

	// Разброс оружия.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Bullet Spread", NoResetToDefault, ClampMin = 0, ClampMax = 100),
		Category = "Item Setting|Weapon Setting")
	float _bulletSpread;

	// Максимальный разброс оружия.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Bullet Spread Max", NoResetToDefault, ClampMin = 0),
		Category = "Item Setting|Weapon Setting|Advanced")
	float _bulletSpreadMax;

	// Разрешить ли использовать FireMode.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Enable Fire Mode", NoResetToDefault),
		Category = "Item Setting|Weapon Setting|Advanced")
	bool _bEnableFireMode;

	// Звук перезарядки.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Reload Sound", NoResetToDefault),
		Category = "Item Setting|Weapon Setting|Sound")
	class USoundCue* _reloadSound;

	// Звук перезарядки.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "No Ammo Sound", NoResetToDefault),
		Category = "Item Setting|Weapon Setting|Sound")
	class USoundCue* _noAmmoSound;

	// Звук смена режима огня.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Select Fire Mode", NoResetToDefault),
		Category = "Item Setting|Weapon Setting|Sound")
	class USoundCue* _selectFireMode;

	// Класс пули.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Bullet", NoResetToDefault),
		Category = "Item Setting|Weapon Setting|Effect")
	TSubclassOf<class AFPSBullet> _bulletTrace;

	// Эффект вспышки при выстреле.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Muzzle", NoResetToDefault),
		Category = "Item Setting|Weapon Setting|Effect")
	class UNiagaraSystem* _muzzleEffect;

	// Анимация затвора. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Breech Mechanism", NoResetToDefault),
		Category = "Item Setting|Weapon Setting|Animation|Weapon")
	class UAnimationAsset* _breechMechanism;

	// Анимация не полной перезарядки (человек). 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Reload Loaded", NoResetToDefault),
		Category = "Item Setting|Weapon Setting|Animation|Pawn")
	class UAnimMontage* _reloadLoadedPawn;

	// Анимация полной перезарядки (человек). 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Reload Empty", NoResetToDefault),
		Category = "Item Setting|Weapon Setting|Animation|Pawn")
	class UAnimMontage* _reloadEmptyPawn;

private:

	// Function.
	
	// Variable.

	// Текущее кол-во патронов.
	UPROPERTY()
	int32 _currentAmmo;
	
	// Текущий разброс.
	UPROPERTY()
	float _currentSpread;

	// Перезарядка в данный момент.
	UPROPERTY()
	bool _bReload;

	// Таймер стрельбы.
	UPROPERTY()
	FTimerHandle _thFireMain;
};
