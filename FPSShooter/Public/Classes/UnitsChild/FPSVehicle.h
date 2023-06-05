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
	// Конструктор техники.
	AFPSVehicle(const FObjectInitializer& ObjectInitializer);

	// Setters.

	// Getters.

	// Возращает скелет техники.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Vehicle Component")
	FORCEINLINE USkeletalMeshComponent* GetSkeletal() const { return Skeletal; }

	// Возращает камеру техники.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Soldier Component")
	FORCEINLINE UCameraComponent* GetCamera() const { return Camera; }

	// Variable.
		
	// Скелет техники.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class USkeletalMeshComponent* Skeletal;

	// Спринг арм.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class USpringArmComponent* SpringArm;

	// Камера техники.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class UCameraComponent* Camera;
	
};
