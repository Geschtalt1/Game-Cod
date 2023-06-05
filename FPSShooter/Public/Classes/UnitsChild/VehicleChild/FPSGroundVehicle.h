// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Classes/UnitsChild/FPSVehicle.h"
#include "FPSGroundVehicle.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class FPSSHOOTER_API AFPSGroundVehicle : public AFPSVehicle
{
	GENERATED_BODY()
	
public:
	// ����������� �������� �������.
	AFPSGroundVehicle(const FObjectInitializer& ObjectInitializer);

	//

	// Getters.

	// ��������� ��������� �������� �������.
	//UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Ground Vehicle Component")
	//FORCEINLINE URTSVehicleMovementComponent* GetMovementVehicle() const { return MovementVehicle;}

	// Variable.

	// ��������� �������� �������.
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//class URTSVehicleMovementComponent* MovementVehicle;
};
