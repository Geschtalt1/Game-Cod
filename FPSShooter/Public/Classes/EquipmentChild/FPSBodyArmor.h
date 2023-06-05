// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Classes/EquipmentChild/FPSDefense.h"
#include "FPSBodyArmor.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class FPSSHOOTER_API AFPSBodyArmor : public AFPSDefense
{
	GENERATED_BODY()

public:
	// Конструктор бронежелета.
	AFPSBodyArmor(const FObjectInitializer& ObjectInitializern);

protected:

private:
	
};
