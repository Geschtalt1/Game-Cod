// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "FPSMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPSSHOOTER_API UFPSMovementComponent : public UFloatingPawnMovement
{
	GENERATED_BODY()

public:	
	UFPSMovementComponent();

	// Тик.
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Setters.

	// Установить скорость юниту.
	UFUNCTION(BlueprintCallable, Category = "Soldier Speed")
	float SetSpeedSoldier(float NewSpeed);
	
	// Getters.

	// Возращает скорость хотьбы.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Soldier Speed")
	FORCEINLINE float GetSpeedWalk() const { return _speedWalk; }

	// Возращает скорость бега.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Soldier Speed")
	FORCEINLINE float GetSpeedRun() const { return _speedRun; }

	// Возращает скорость при приседании.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Soldier Speed")
	FORCEINLINE float GetSpeedCrouch() const { return _speedCrouch; }

	// Возращает бежит ли юнит.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Soldier Speed")
	bool IsRunning() const;

	// Возращает передвигается ли юнит по земли в данный момент.
	bool IsMovingOnGround() const override;

	// Возращает присадает ли юнит в данный момент.
	bool IsCrouching() const override;
	
	// Varieble.

protected:
	virtual void BeginPlay() override;

	// Пре инцилизация компонента.
	virtual void PostInitProperties() override;

	// Setters.
	
	// Getters.
	
	// Varieble.

	// Сила гравитации.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gravity Setting")
	float GravityScale;

	// Гравитация солдата.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gravity Setting")
	bool bGravity;

	// Скорость при хотьбе.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (DisplayName = "Speed Walk", NoResetToDefault),
		Category = "Speed Setting|General")
	float _speedWalk;

	// Скорость при спринте.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (DisplayName = "Speed Run", NoResetToDefault),
		Category = "Speed Setting|General")
	float _speedRun;

	// Скорость при присидание.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (DisplayName = "Speed Crouch", NoResetToDefault),
		Category = "Speed Setting|General")
	float _speedCrouch;

	// Скорость поворота.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (DisplayName = "Speed Turn"), Category = "Speed Setting")
	float InterpSpeed;

	// Вкл - выкл поворота.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Speed Setting")
	bool bTurn;

private:	

	// Обновление гравитации юнита.
	void UpdateGravity(float DeltaTime);

	// Обновление поворота юнита.
	void UpdateTurn(float DeltaTime, const float SpeedTurn);

	// Setters.
	
	// Getters.

	// Varieble.
	
	// Ссылка на владельца компонентом.
	class AFPSSoldier* _soldierRef;

	// Двигается ли сейчас солдат.
	uint8 _bIsMoving:1;

	// Бежит ли сейчас солдат.
	uint8 _bIsRunning:1;
		
};
