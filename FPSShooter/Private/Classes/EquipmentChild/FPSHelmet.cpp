// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/EquipmentChild/FPSHelmet.h"
#include "Classes/UnitsChild/FPSSoldier.h"

#include "Engine/SkeletalMeshSocket.h"

#include "Component/FPSEquipmentComponent.h"

// Конуструктор каски.
AFPSHelmet::AFPSHelmet(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	// Относительное расположение и вращения каски.
	//_relativLocation = FVector(18.0f, 2.0f, 0.0f);
	//_relativRotation = FRotator(-90.0f, 90.f, -87.0f);

	// Индефекатор касок.
	_itemID = 1;
}

/* Заменяем модель подоброного предмета.
bool AFPSHelmet::SetItemMeshToSoldier(const AFPSSoldier& Soldier, const bool bDeleteMesh)
{
	this->AttachToComponent(
		Soldier.GetSkeletal(),
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		FName("Helmet")
	);

	//FVector sd = _relativLocation;

	//Soldier.GetSkeletal()->GetSocketByName("")->RelativeLocation.XYZ;
	
		// Устанавливаем относительное расположение и вращение.
	Soldier.HelmetMesh->SetRelativeLocation(_relativLocation);
	Soldier.HelmetMesh->SetRelativeRotation(_relativRotation);

	// Возращаем успех.
	return true;
}*/