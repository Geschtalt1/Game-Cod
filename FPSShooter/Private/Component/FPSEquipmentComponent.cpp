// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/FPSEquipmentComponent.h"

#include "FPSHUD.h"

#include "Other/FPSEnumeration.h"

#include "Classes/FPSItemBase.h"
#include "Classes/FPSWeaponBase.h"
#include "Classes/EquipmentChild/FPSHelmet.h"
#include "Classes/UnitsChild/FPSSoldier.h"
#include "Classes/WeaponChild/FireArmsChild/FPSRifle.h"

// Конструктор.
UFPSEquipmentComponent::UFPSEquipmentComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	_currentWeightSoldier = 0.0f;
	_maxWeightSoldier = 120.0f;

	// ...
}


// Начало игры.
void UFPSEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	// Ссылка на влaдельца компонентом.
	//_OwnerComponent = Cast<AFPSSoldier>(GetOwner());
	// 
	// ...
	
}

bool UFPSEquipmentComponent::AddItemToInventory(const int32 ID_Item, AFPSItemBase* Item)
{
	// Если ссылка не установлена, функция перестает работать дальше.
	if (!Item && GetPlayerComponent()) { return false; }

	// Проверяем индекс предмета.
	// Индекс который не равен нулю считается экипировкой.
	if (ID_Item != 0)
	{
		// Выозращаем True или False, есть ли такая экипировка.
		const bool bValidEquipment = _items.Contains(ID_Item);

		// Если такая экипировки есть.
		if (bValidEquipment)
		{
			// Выкидываем старую экипировку.
			RemoveItemFromInventory(ID_Item, true);

			// Добовляем новую.
			AddItemToInventory(ID_Item, Item);

			// Выходим из функции.
			return true;

			/* Ссылка на HUD.
			const AFPSHUD* _hudLocal =
				GetPlayerComponent()->GetPlayerHUD();

			// Проверяем ссылку на HUD.
			if (!_hudLocal) { return false; }*/

			// Обновляем предмет в инвентаре.
			//_hudLocal->OnNewItem.Broadcast(ID_Item, ItemClass);

			// Возращаем успех функции.
			//return true;
			
		}
	}
	
	// Добавляем предмет.
	_items.Add(ID_Item, Item);

	// Записываем вес предмета.
	const float _weightItem = Item->GetWeightItem();

	// Добавляем вес солдату.
	AddWeightSoldier(_weightItem);

	return true;
}

// Удалить предмет из инвенторя.
bool UFPSEquipmentComponent::RemoveItemFromInventory(const int32 ID, const bool bThrowOut)
{
	// Проверяем есть ли предмет по указаному индексу.
	const bool bValidItem = _items.Contains(ID);

	// Если есть предмет.
	if (bValidItem)
	{
		// Берем ссылку на предмет по ключу.
		AFPSItemBase* _itemLocal = _items.FindRef(ID);

		// Если предмета нет, прекращаем работу функции.
		if (!_itemLocal) { return false; }

		// Приводим к типу солдата.
		AFPSSoldier& _soldierLocal = *Cast<AFPSSoldier>(GetPlayerComponent());

		// Открепляем предмет.
		_itemLocal->DetachToSoldier(_soldierLocal);

		// Получаем вес предмета.
		const float _weightItem = _itemLocal->GetWeightItem();

		// Отнимаем вес у юнита.
		AddWeightSoldier(_weightItem * (-1.0f));

		// Удаляем его из инвентаря и возращаем результат удаления.
		return _items.RemoveAndCopyValue(ID, _itemLocal);

	}

	return false;
}

// Добавить вес солдату.
float UFPSEquipmentComponent::AddWeightSoldier(float Value)
{
	// Добавляем новый вес к текущему весу солдта.
	_currentWeightSoldier = FMath::Clamp(
		_currentWeightSoldier + Value, // Складываем с текущим весом.
		0.0f, // Минимальный вес.
		_maxWeightSoldier // Максимальный вес.
	);
	
	// Возращаем вес.
	return _currentWeightSoldier;
}


// Пре инициализация в редакторе.
#if WITH_EDITOR
void UFPSEquipmentComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FString Name = PropertyChangedEvent.Property->GetName();

	// Если меняем процент защиты.
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
