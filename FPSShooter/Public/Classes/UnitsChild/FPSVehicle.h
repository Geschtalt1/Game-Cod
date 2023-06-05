// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Classes/FPSUnitsBase.h"
#include "FPSVehicle.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class FPSSHOOTER_API AFPSVehicle : public AFPSUnitsBase
{
	GENERATED_BODY()

public:
	// ����������� �������.
	AFPSVehicle(const FObjectInitializer& ObjectInitializer);

	// Setters.

	// Getters.

	// ��������� ������ �������.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Vehicle Component")
	FORCEINLINE USkeletalMeshComponent* GetSkeletal() const { return Skeletal; }

	// ��������� ������ �������.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Soldier Component")
	FORCEINLINE UCameraComponent* GetCamera() const { return Camera; }

	// Variable.
		
	// ������ �������.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class USkeletalMeshComponent* Skeletal;

	// ������ ���.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class USpringArmComponent* SpringArm;

	// ������ �������.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class UCameraComponent* Camera;
	
};
