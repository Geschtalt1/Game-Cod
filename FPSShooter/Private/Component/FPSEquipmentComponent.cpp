// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/FPSEquipmentComponent.h"

#include "FPSHUD.h"

#include "Other/FPSEnumeration.h"

#include "Classes/FPSItemBase.h"
#include "Classes/FPSWeaponBase.h"
#include "Classes/EquipmentChild/FPSHelmet.h"
#include "Classes/UnitsChild/FPSSoldier.h"
#include "Classes/WeaponChild/FireArmsChild/FPSRifle.h"

// �����������.
UFPSEquipmentComponent::UFPSEquipmentComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	_currentWeightSoldier = 0.0f;
	_maxWeightSoldier = 120.0f;

	// ...
}


// ������ ����.
void UFPSEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	// ������ �� ��a������ �����������.
	//_OwnerComponent = Cast<AFPSSoldier>(GetOwner());
	// 
	// ...
	
}

bool UFPSEquipmentComponent::AddItemToInventory(const int32 ID_Item, AFPSItemBase* Item)
{
	// ���� ������ �� �����������, ������� ��������� �������� ������.
	if (!Item && GetPlayerComponent()) { return false; }

	// ��������� ������ ��������.
	// ������ ������� �� ����� ���� ��������� �����������.
	if (ID_Item != 0)
	{
		// ���������� True ��� False, ���� �� ����� ����������.
		const bool bValidEquipment = _items.Contains(ID_Item);

		// ���� ����� ���������� ����.
		if (bValidEquipment)
		{
			// ���������� ������ ����������.
			RemoveItemFromInventory(ID_Item, true);

			// ��������� �����.
			AddItemToInventory(ID_Item, Item);

			// ������� �� �������.
			return true;

			/* ������ �� HUD.
			const AFPSHUD* _hudLocal =
				GetPlayerComponent()->GetPlayerHUD();

			// ��������� ������ �� HUD.
			if (!_hudLocal) { return false; }*/

			// ��������� ������� � ���������.
			//_hudLocal->OnNewItem.Broadcast(ID_Item, ItemClass);

			// ��������� ����� �������.
			//return true;
			
		}
	}
	
	// ��������� �������.
	_items.Add(ID_Item, Item);

	// ���������� ��� ��������.
	const float _weightItem = Item->GetWeightItem();

	// ��������� ��� �������.
	AddWeightSoldier(_weightItem);

	return true;
}

// ������� ������� �� ���������.
bool UFPSEquipmentComponent::RemoveItemFromInventory(const int32 ID, const bool bThrowOut)
{
	// ��������� ���� �� ������� �� ��������� �������.
	const bool bValidItem = _items.Contains(ID);

	// ���� ���� �������.
	if (bValidItem)
	{
		// ����� ������ �� ������� �� �����.
		AFPSItemBase* _itemLocal = _items.FindRef(ID);

		// ���� �������� ���, ���������� ������ �������.
		if (!_itemLocal) { return false; }

		// �������� � ���� �������.
		AFPSSoldier& _soldierLocal = *Cast<AFPSSoldier>(GetPlayerComponent());

		// ���������� �������.
		_itemLocal->DetachToSoldier(_soldierLocal);

		// �������� ��� ��������.
		const float _weightItem = _itemLocal->GetWeightItem();

		// �������� ��� � �����.
		AddWeightSoldier(_weightItem * (-1.0f));

		// ������� ��� �� ��������� � ��������� ��������� ��������.
		return _items.RemoveAndCopyValue(ID, _itemLocal);

	}

	return false;
}

// �������� ��� �������.
float UFPSEquipmentComponent::AddWeightSoldier(float Value)
{
	// ��������� ����� ��� � �������� ���� ������.
	_currentWeightSoldier = FMath::Clamp(
		_currentWeightSoldier + Value, // ���������� � ������� �����.
		0.0f, // ����������� ���.
		_maxWeightSoldier // ������������ ���.
	);
	
	// ��������� ���.
	return _currentWeightSoldier;
}


// ��� ������������� � ���������.
#if WITH_EDITOR
void UFPSEquipmentComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FString Name = PropertyChangedEvent.Property->GetName();

	// ���� ������ ������� ������.
	if (Name == TEXT("_rifleClassSpawn"))
	{
		//AActor& _unitRef = *GetOwner();

		AFPSSoldier* _soldierRef = Cast<AFPSSoldier>(GetOwner());

		if (_soldierRef && _rifleClassSpawn)
		{
			_soldierRef->GetChildWeapon()->
				SetChildActorClass(_rifleClassSpawn);
		}

		//TArray<AActor*> _actorsComponent;
		//_unitRef.GetAllChildActors(_actorsComponent);

	}
}
#endif
