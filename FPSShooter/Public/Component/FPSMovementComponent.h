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

	// ���.
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Setters.

	// ���������� �������� �����.
	UFUNCTION(BlueprintCallable, Category = "Soldier Speed")
	float SetSpeedSoldier(float NewSpeed);
	
	// Getters.

	// ��������� �������� ������.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Soldier Speed")
	FORCEINLINE float GetSpeedWalk() const { return _speedWalk; }

	// ��������� �������� ����.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Soldier Speed")
	FORCEINLINE float GetSpeedRun() const { return _speedRun; }

	// ��������� �������� ��� ����������.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Soldier Speed")
	FORCEINLINE float GetSpeedCrouch() const { return _speedCrouch; }

	// ��������� ����� �� ����.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Soldier Speed")
	bool IsRunning() const;

	// ��������� ������������� �� ���� �� ����� � ������ ������.
	bool IsMovingOnGround() const override;

	// ��������� ��������� �� ���� � ������ ������.
	bool IsCrouching() const override;
	
	// Varieble.

protected:
	virtual void BeginPlay() override;

	// ��� ����������� ����������.
	virtual void PostInitProperties() override;

	// Setters.
	
	// Getters.
	
	// Varieble.

	// ���� ����������.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gravity Setting")
	float GravityScale;

	// ���������� �������.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gravity Setting")
	bool bGravity;

	// �������� ��� ������.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (DisplayName = "Speed Walk", NoResetToDefault),
		Category = "Speed Setting|General")
	float _speedWalk;

	// �������� ��� �������.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (DisplayName = "Speed Run", NoResetToDefault),
		Category = "Speed Setting|General")
	float _speedRun;

	// �������� ��� ����������.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (DisplayName = "Speed Crouch", NoResetToDefault),
		Category = "Speed Setting|General")
	float _speedCrouch;

	// �������� ��������.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (DisplayName = "Speed Turn"), Category = "Speed Setting")
	float InterpSpeed;

	// ��� - ���� ��������.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Speed Setting")
	bool bTurn;

private:	

	// ���������� ���������� �����.
	void UpdateGravity(float DeltaTime);

	// ���������� �������� �����.
	void UpdateTurn(float DeltaTime, const float SpeedTurn);

	// Setters.
	
	// Getters.

	// Varieble.
	
	// ������ �� ��������� �����������.
	class AFPSSoldier* _soldierRef;

	// ��������� �� ������ ������.
	uint8 _bIsMoving:1;

	// ����� �� ������ ������.
	uint8 _bIsRunning:1;
		
};
