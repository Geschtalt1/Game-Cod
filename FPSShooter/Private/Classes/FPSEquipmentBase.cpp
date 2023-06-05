
#include "Classes/FPSEquipmentBase.h"
#include "Classes/UnitsChild/FPSSoldier.h"

// �����������.
AFPSEquipmentBase::AFPSEquipmentBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

// ������ ����.
void AFPSEquipmentBase::BeginPlay()
{
	Super::BeginPlay();
}

// ���������� ��������� ������.
void AFPSEquipmentBase::SetEquipmentOwner(AFPSSoldier& NewOwner)
{
	_OwnerEquipment = &NewOwner;
}

