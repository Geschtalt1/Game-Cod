// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/EquipmentChild/FPSBackpack.h"
#include "Classes/UnitsChild/FPSSoldier.h"

AFPSBackpack::AFPSBackpack(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	// ������������� ������������ � �������� �������.
	//_relativLocation = FVector(22.0f, -24.0f, 0.0f);
	//_relativRotation = FRotator(-266.0f, 99.0f, -440.0f);

	// ����������� ��������.
	_itemID = 4;
}