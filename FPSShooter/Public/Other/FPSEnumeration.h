// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FPSEnumeration.generated.h"

// Позиция камеры.
UENUM(BlueprintType)
enum ECameraPos
{
	POS_Center                 UMETA(DisplayName = "Center"),
	POS_FirstPerson            UMETA(DisplayName = "First Person"),
	POS_LeftRight              UMETA(DisplayName = "Left Right")
};

// Тип восстановления стамины.
UENUM(BlueprintType)
enum EStaminaChange
{
	ST_Regeneration         UMETA(DisplayName = "Regeneration"),
	ST_Reduction            UMETA(DisplayName = "Reduction"),
	ST_Refresh              UMETA(DisplayName = "Reduction")
};

// Тип стрельбы.
UENUM(BlueprintType)
enum EFireMode
{
	FM_Single         UMETA(DisplayName = "Single"),
	FM_Full           UMETA(DisplayName = "Full")
};

// Куда крепится предмет.
UENUM(BlueprintType)
enum EItemSlot
{
	IS_Memory         UMETA(DisplayName = "Memory"),
	IS_Helmat         UMETA(DisplayName = "Helmat"),
	IS_Glasses        UMETA(DisplayName = "Glasses"),
	IS_BodeArmor      UMETA(DisplayName = "Bode Armor"),
	IS_Backpack       UMETA(DisplayName = "Backpack"),
	IS_Weapon         UMETA(DisplayName = "Weapon")
};
