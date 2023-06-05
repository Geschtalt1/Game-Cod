// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Classes/FPSItemBase.h"
#include "FPSBackpack.generated.h"

/**
 * 
 */
UCLASS()
class FPSSHOOTER_API AFPSBackpack : public AFPSItemBase
{
	GENERATED_BODY()
	
public:

	// Конструктор.
	AFPSBackpack(const FObjectInitializer& ObjectInitializer);

	// Меняет меш модели у солдата.
	//bool SetItemMeshToSoldier(const class AFPSSoldier& Soldier, const bool bDeleteMesh = false) override;
};
