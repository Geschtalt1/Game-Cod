// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Classes/WeaponChild/FPSFireArms.h"
#include "FPSRifle.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class FPSSHOOTER_API AFPSRifle : public AFPSFireArms
{
	GENERATED_BODY()

public:
	// Конструктор винтовки.
	AFPSRifle(const FObjectInitializer& ObjectInitializer);
	
};
