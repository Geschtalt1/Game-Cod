
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Kismet/KismetMathLibrary.h"

#include "Other/FPSEnumeration.h"
#include "Interface/FPSInterfaceControll.h"

#include "FPSUnitsBase.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFireMulticastDelegate);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Abstract)
class FPSSHOOTER_API AFPSUnitsBase : public APawn, public IFPSInterfaceControll
{
	GENERATED_BODY()

public:
	// Конструктор.
	AFPSUnitsBase(const FObjectInitializer& ObjectInitializer);

	// Тик.
	virtual void Tick(float DeltaTime) override;

	// Получение урона.
	float TakeDamage(float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser) override;

	// Function Interface.

	// Евент взаимодействия с предметом.
	virtual void OnInteractionTrace_Implementation() override;

	// Юнит стреляет из главного оружия.
	UFUNCTION(BlueprintCallable, Category = "Unit Action")
	virtual void OnFireMainUnit();

	// Юнит перезарежается.
	UFUNCTION(BlueprintCallable, Category = "Unit Action")
	virtual void OnReloadUnit();

	// Юнит целится.
	UFUNCTION(BlueprintCallable, Category = "Unit Action")
	virtual void OnAimUnit(bool bActive);

	// Юнит движеться вперед или назад.
	UFUNCTION(BlueprintCallable, Category = "Unit Action")
	virtual void OnMoveForward(float Value);

	// Юнит движеться вправо или влево.
	UFUNCTION(BlueprintCallable, Category = "Unit Action")
	virtual void OnMoveRight(float Value);

	// Остановить стрельбу.
	UFUNCTION(BlueprintCallable, Category = "Unit Action")
	virtual void OnStopFire();

	// Setters.

	// Установить максимальную скорость передвижения.
	UFUNCTION(BlueprintCallable, Category = "Unit Speed")
	virtual void SetSpeedMax(float NewSpeed);

	// Установить увеличиную скорость передвижения.
	UFUNCTION(BlueprintCallable, Category = "Unit Speed")
	virtual void SetSpeedRun(bool bSprint);

	// Выбрать расположение камеры.
	UFUNCTION(BlueprintCallable, Category = "Unit Setting")
	virtual void SetCameraPosition(TEnumAsByte<ECameraPos> Position);

	// Включить или отключить стрельбу из оружия.
	UFUNCTION(BlueprintCallable, Category = "Unit Setting")
	virtual void SetFireEnable(bool bEnable);

	// Getters.

	// Возращает жив или мертв юнит в данный момент.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Unit Alive")
	bool GetAlive() const;

	// Возращает перезарежается ли юнит в данный момент.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Unit Reload")
	virtual bool IsReloadUnit() const;

	// Возращает передвигается ли юнит на спринте.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Unit Speed")
	virtual bool IsRunUnit() const;

	// Возращает прицеливается ли юнит.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Unit Aim")
	virtual bool IsAimUnit() const;

	// Возращает можно ли юниту стрелять.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Unit Action")
	FORCEINLINE bool GetFireEnable() const { return (bool)_bFireEnable; };

	// Возращает скорость юнита.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Unit Speed")
	FORCEINLINE float GetSpeedUnit() const { 
		return UKismetMathLibrary::VSize(GetVelocity()); }

	// Возращает компонент здоровья.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Unit Speed")
	FORCEINLINE UFPSHealthComponent* GetHealth() const { return Health; }

	// Возращает ссылку на HUD игрока.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Unit Widget")
	FORCEINLINE AFPSHUD* GetPlayerHUD() const { return _hudPlayer; }
	
	// Varieble.

	// Компонент здоровья.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class UFPSHealthComponent* Health;

	// Делегат выстрела.
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "On Fire", Category = "Default", MultiLine = "true", OverrideNativeName = "OnFire"))
	//FOnFireMulticastDelegate OnFireUnit;

protected:
	// Начало игры.
	virtual void BeginPlay() override;

	// Пре инцилизация юнита.
	virtual void PostInitProperties() override;

	// Убить юнита.
	UFUNCTION()
	virtual void OnKillUnit();

	// Setters.

	// Getters.

	// Varieble.

	// Прицеливается ли юнит.
	uint8 _bAim;

private:
	// Setters.

	// Getters.

	// Varieble.

	// Разрешенно ли юниту, стрелять.
	uint8 _bFireEnable;

	// Ссылка на HUD игрока.
	class AFPSHUD* _hudPlayer;

};
