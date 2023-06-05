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
	// ������������ �����.
	AFPSHelmet(const FObjectInitializer& ObjectInitializer);

	// Setters.

	// �������� ������ ����������� ��������.
	//bool SetItemMeshToSoldier(const class AFPSSoldier& Soldier, const bool bDeleteMesh = false) override;
	
};
