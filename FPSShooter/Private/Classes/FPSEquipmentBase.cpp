
#include "Classes/FPSEquipmentBase.h"
#include "Classes/UnitsChild/FPSSoldier.h"

// Конструктор.
AFPSEquipmentBase::AFPSEquipmentBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

// Начало игры.
void AFPSEquipmentBase::BeginPlay()
{
	Super::BeginPlay();
}

// Установить владельца оружия.
void AFPSEquipmentBase::SetEquipmentOwner(AFPSSoldier& NewOwner)
{
	_OwnerEquipment = &NewOwner;
}

