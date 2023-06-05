// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/EquipmentChild/FPSDefense.h"
#include "Other/FPSTypename.h"

// ����������� �����.
AFPSDefense::AFPSDefense(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//Scene = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	//SetRootComponent(Scene);

	//ItemMesh->SetupAttachment(Scene);

	//DefenseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Armor"));
	//DefenseMesh->SetCollisionResponseToChannel(CHANNELS_ITEM_LINE, ECR_Block);
	//DefenseMesh->SetCollisionObjectType(CHANNELS_ITEM);
	//DefenseMesh->SetupAttachment(Scene);

	_armorDefense = 0.0f;
	_armorLevel = 0;
}

// ���������� ������� ����� (0 - 100).
int32 AFPSDefense::SetArmorProcent(float NewProcent)
{
	// ������� ����� ����� ���������� � ��������� �� 0 �� 100.
	_armorDefense = FMath::Clamp(NewProcent, // ������������ ��������.
		0.0f, // �����������.
		100.0f); // ������������.

	// ������� ����� �������, �� �� �������� ������.
	// ���� ������� ������ � ������ �� 0 �� 50, �� ��� ������ ������ ������.
	if (_armorDefense > 0.0f && _armorDefense < 50.0f) 
	{
		_armorLevel = 1;
	}
	// ���� ������� ������ � ������ �� 50 �� 75, �� ��� ������ ������ ������.
	else if (_armorDefense >= 50.0f && _armorDefense < 75.0f) 
	{
		_armorLevel = 2;
	}
	// ���� ������� ������ ������ ����� 75, �� ��� ������ ������ ������.
	else if (_armorDefense >= 75.0f) 
	{
		_armorLevel = 3;
	}
	// ������� ������ 0, ���� �� ������������� ���� ����������������� ��������.
	else 
	{
		_armorLevel = 0;
	}

	return _armorLevel;
}

// ��� ������������� � ���������.
void AFPSDefense::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FString Name = PropertyChangedEvent.Property->GetName();

	// ���� ������ ������� ������.
	if (Name == TEXT("_armorDefense")) 
	{
		// ����������� ����� ������� ������.
		SetArmorProcent(_armorDefense);
	}
}