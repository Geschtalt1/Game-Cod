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
	// Конструктор наземной техники.
	AFPSGroundVehicle(const FObjectInitializer& ObjectInitializer);

	//

	// Getters.

	// Возращает компонент движения техники.
	//UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Ground Vehicle Component")
	//FORCEINLINE URTSVehicleMovementComponent* GetMovementVehicle() const { return MovementVehicle;}

	// Variable.

	// Компонент движения техники.
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//class URTSVehicleMovementComponent* MovementVehicle;
};
