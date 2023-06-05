// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/FPSItemBase.h"
#include "Classes/UnitsChild/FPSSoldier.h"

#include "Component/FPSEquipmentComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/WidgetComponent.h"

#include "Other/FPSTypename.h"
#include "Sound/SoundCue.h"

#include "Kismet/GameplayStatics.h"

#include "Materials/MaterialInstance.h"
#include "Components/StaticMeshComponent.h"

// Конструктор.
AFPSItemBase::AFPSItemBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>("Item Mesh");
	ItemMesh->SetCollisionResponseToChannel(CHANNELS_ITEM_LINE, ECR_Block);
	ItemMesh->SetCollisionResponseToChannel(CHANNELS_ITEM, ECR_Block);
	ItemMesh->SetCollisionResponseToChannel(CHANNELS_BULLET_LINE, ECR_Ignore);
	ItemMesh->SetCollisionObjectType(CHANNELS_ITEM);
	ItemMesh->SetCustomDepthStencilValue(1);
	ItemMesh->SetSimulatePhysics(true);
	SetRootComponent(ItemMesh);

	InteractWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Interact Widget"));
	InteractWidget->SetWidgetSpace(EWidgetSpace::Screen);
	InteractWidget->SetTickMode(ETickMode::Automatic);
	InteractWidget->SetDrawSize(FVector2D(32, 32));
	InteractWidget->SetDrawAtDesiredSize(true);
	InteractWidget->SetVisibility(false);
	InteractWidget->SetupAttachment(ItemMesh);

	_socketNameAttach = { "No Attach" };
	_itemName = {"No Name"};
	_itemID = 0;

	_itemImage = nullptr;
	_pickUpSound = nullptr;
	_dropItemsSound = nullptr;

	_itemWeight = 0.0f;

	_relativLocation = {};
	_relativRotation = {};

	 // Добавялем виджет взайимодействия.
	static ConstructorHelpers::FClassFinder<UUserWidget> WB_ItemIteraction(TEXT
	("/Game/FPSGame/Widget/Gameplay/WB_ItemIteraction"));

	// Если виджет найден.
	if (WB_ItemIteraction.Succeeded()) {
		InteractWidget->SetWidgetClass(WB_ItemIteraction.Class);
	}
}

// Старт игры.
void AFPSItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Тик.
void AFPSItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Вызывается когда игрок посмотрел на предмет.
void AFPSItemBase::OnBeginLookItem_Implementation(const AFPSSoldier* Soldier)
{
	// Включаем видимость виджета.
	InteractWidget->SetVisibility(true);

	// Включаем тик мод.
	InteractWidget->SetTickMode(ETickMode::Enabled);

	// Включаем обводку.
	ItemMesh->SetRenderCustomDepth(true);
}

// Вызывается когда игрок закончил смотреть на предмет.
void AFPSItemBase::OnEndLookItem_Implementation(const AFPSSoldier* Soldier)
{
	// Отключаем тик мод.
	InteractWidget->SetTickMode(ETickMode::Automatic);

	// Отключаем видимость виджета.
	InteractWidget->SetVisibility(false);

	// Выключаем обводку.
	ItemMesh->SetRenderCustomDepth(false);
}

// Вызывается когда игрок нажимает подобрать предмет.
void AFPSItemBase::OnTakeItem_Implementation(const AFPSSoldier* Soldier)
{
	// Если ссылки на солдата нет, функция прекращает работать.
	if (!Soldier) { return; }

	// Добавляем в инвентарь предмет.
	const bool _isAdd = Soldier->GetEquipment()->
		AddItemToInventory(_itemID, this);

	// Если предмет добавился.
	if (_isAdd) 
	{ 
		// Заменяем предмет.
		//if (_itemID == 0)
	//	{
			// Уничтожаем из мира.
			//Destroy();
		//}

		// Прикрепляем предмет к юниту.
		AttachToSoldier(*Soldier);

		// Проигрываем звук подбора предмета.
		UGameplayStatics::PlaySound2D(GetWorld(),
			_pickUpSound, // Звук подбор предмата.
			1.0f, // Множитель объема.
			1.0f, // Множитель высоты тона.
			0.0f,
			nullptr,
			Soldier
		);
	}
}

// Заменяем модель подоброного предмета.
bool AFPSItemBase::AttachToSoldier(const AFPSSoldier& Soldier)
{
	// Прикрепляем предмет к сокету.
	AttachToComponent(
		Soldier.GetSkeletal(), // Скелет которому крепим.
		FAttachmentTransformRules(EAttachmentRule::SnapToTarget, // Относительное расположение.
			EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true),
		_socketNameAttach // Название сокета.
	);

	// Отключаем физику меша.
	ItemMesh->SetSimulatePhysics(false);

	// Отключаем коллизию предмета.
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Устанавливаем относительное расположение.
	SetActorRelativeLocation(_relativLocation);
	SetActorRelativeRotation(_relativRotation);

	// Возращаем успех.
	return true;
}

// открепляем предмет от юнита.
bool AFPSItemBase::DetachToSoldier(const AFPSSoldier& Soldier)
{
	// Деатачем предмет от скелета юнита. 
	ItemMesh->DetachFromComponent(FDetachmentTransformRules(
		EDetachmentRule::KeepWorld,
		EDetachmentRule::KeepWorld,
		EDetachmentRule::KeepWorld,
		true)
	);

	// Координаты юнита.
	const FVector _actorLocation = Soldier.GetActorLocation();
	const FVector _actorForward = Soldier.GetActorForwardVector();

	// Высчитываем координаты место падения предмета.
	const FVector _locationSpawn =
		_actorLocation + ((_actorForward * 100.0f) +
			FVector(0.0, 0.0, 50.0f));

	// Перемещаем предмет перед персонажем.
	SetActorLocation(_locationSpawn);

	// Возращаем тип коллизии и физики.
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	// Включаем физику.
	ItemMesh->SetSimulatePhysics(true);

	// Проигрываем звук выброса предмета.
	UGameplayStatics::PlaySound2D(GetWorld(),
		_dropItemsSound, // Звук выброса предмата.
		1.0f, // Множитель объема.
		1.0f, // Множитель высоты тона.
		0.0f,
		nullptr,
		&Soldier
	);

	// Возращаем успех.
	return true;
}

