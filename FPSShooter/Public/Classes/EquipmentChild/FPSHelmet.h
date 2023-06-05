// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Classes/EquipmentChild/FPSDefense.h"
#include "FPSHelmet.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class FPSSHOOTER_API AFPSHelmet : public AFPSDefense
{
	GENERATED_BODY()

public:
	// Конуструктор каски.
	AFPSHelmet(const FObjectInitializer& ObjectInitializer);

	// Setters.

	// Заменяем модель подоброного предмета.
	//bool SetItemMeshToSoldier(const class AFPSSoldier& Soldier, const bool bDeleteMesh = false) override;
	
};
