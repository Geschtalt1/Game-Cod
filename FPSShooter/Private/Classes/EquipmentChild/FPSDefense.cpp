// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/EquipmentChild/FPSDefense.h"
#include "Other/FPSTypename.h"

// Конструктор брони.
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

// Установить процент брони (0 - 100).
int32 AFPSDefense::SetArmorProcent(float NewProcent)
{
	// Процент брони может находиться в диапозоне от 0 до 100.
	_armorDefense = FMath::Clamp(NewProcent, // Поступающиее значение.
		0.0f, // Минимальное.
		100.0f); // Максимальное.

	// Уровень брони зависит, от ее процента защиты.
	// Если процент защиты в районе от 0 до 50, то это первый уровен защиты.
	if (_armorDefense > 0.0f && _armorDefense < 50.0f) 
	{
		_armorLevel = 1;
	}
	// Если процент защиты в районе от 50 до 75, то это второй уровен защиты.
	else if (_armorDefense >= 50.0f && _armorDefense < 75.0f) 
	{
		_armorLevel = 2;
	}
	// Если процент защиты больше равно 75, то это третий уровен защиты.
	else if (_armorDefense >= 75.0f) 
	{
		_armorLevel = 3;
	}
	// Уровень защиты 0, если не соответствует всем вышеперечисленным условиям.
	else 
	{
		_armorLevel = 0;
	}

	return _armorLevel;
}

// Пре инициализация в редакторе.
void AFPSDefense::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FString Name = PropertyChangedEvent.Property->GetName();

	// Если меняем процент защиты.
	if (Name == TEXT("_armorDefense")) 
	{
		// Высчитываем новый уровень защиты.
		SetArmorProcent(_armorDefense);
	}
}