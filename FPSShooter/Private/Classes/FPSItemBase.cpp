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

// �����������.
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

	 // ��������� ������ ���������������.
	static ConstructorHelpers::FClassFinder<UUserWidget> WB_ItemIteraction(TEXT
	("/Game/FPSGame/Widget/Gameplay/WB_ItemIteraction"));

	// ���� ������ ������.
	if (WB_ItemIteraction.Succeeded()) {
		InteractWidget->SetWidgetClass(WB_ItemIteraction.Class);
	}
}

// ����� ����.
void AFPSItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// ���.
void AFPSItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// ���������� ����� ����� ��������� �� �������.
void AFPSItemBase::OnBeginLookItem_Implementation(const AFPSSoldier* Soldier)
{
	// �������� ��������� �������.
	InteractWidget->SetVisibility(true);

	// �������� ��� ���.
	InteractWidget->SetTickMode(ETickMode::Enabled);

	// �������� �������.
	ItemMesh->SetRenderCustomDepth(true);
}

// ���������� ����� ����� �������� �������� �� �������.
void AFPSItemBase::OnEndLookItem_Implementation(const AFPSSoldier* Soldier)
{
	// ��������� ��� ���.
	InteractWidget->SetTickMode(ETickMode::Automatic);

	// ��������� ��������� �������.
	InteractWidget->SetVisibility(false);

	// ��������� �������.
	ItemMesh->SetRenderCustomDepth(false);
}

// ���������� ����� ����� �������� ��������� �������.
void AFPSItemBase::OnTakeItem_Implementation(const AFPSSoldier* Soldier)
{
	// ���� ������ �� ������� ���, ������� ���������� ��������.
	if (!Soldier) { return; }

	// ��������� � ��������� �������.
	const bool _isAdd = Soldier->GetEquipment()->
		AddItemToInventory(_itemID, this);

	// ���� ������� ���������.
	if (_isAdd) 
	{ 
		// �������� �������.
		//if (_itemID == 0)
	//	{
			// ���������� �� ����.
			//Destroy();
		//}

		// ����������� ������� � �����.
		AttachToSoldier(*Soldier);

		// ����������� ���� ������� ��������.
		UGameplayStatics::PlaySound2D(GetWorld(),
			_pickUpSound, // ���� ������ ��������.
			1.0f, // ��������� ������.
			1.0f, // ��������� ������ ����.
			0.0f,
			nullptr,
			Soldier
		);
	}
}

// �������� ������ ����������� ��������.
bool AFPSItemBase::AttachToSoldier(const AFPSSoldier& Soldier)
{
	// ����������� ������� � ������.
	AttachToComponent(
		Soldier.GetSkeletal(), // ������ �������� ������.
		FAttachmentTransformRules(EAttachmentRule::SnapToTarget, // ������������� ������������.
			EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true),
		_socketNameAttach // �������� ������.
	);

	// ��������� ������ ����.
	ItemMesh->SetSimulatePhysics(false);

	// ��������� �������� ��������.
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// ������������� ������������� ������������.
	SetActorRelativeLocation(_relativLocation);
	SetActorRelativeRotation(_relativRotation);

	// ��������� �����.
	return true;
}

// ���������� ������� �� �����.
bool AFPSItemBase::DetachToSoldier(const AFPSSoldier& Soldier)
{
	// �������� ������� �� ������� �����. 
	ItemMesh->DetachFromComponent(FDetachmentTransformRules(
		EDetachmentRule::KeepWorld,
		EDetachmentRule::KeepWorld,
		EDetachmentRule::KeepWorld,
		true)
	);

	// ���������� �����.
	const FVector _actorLocation = Soldier.GetActorLocation();
	const FVector _actorForward = Soldier.GetActorForwardVector();

	// ����������� ���������� ����� ������� ��������.
	const FVector _locationSpawn =
		_actorLocation + ((_actorForward * 100.0f) +
			FVector(0.0, 0.0, 50.0f));

	// ���������� ������� ����� ����������.
	SetActorLocation(_locationSpawn);

	// ��������� ��� �������� � ������.
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	// �������� ������.
	ItemMesh->SetSimulatePhysics(true);

	// ����������� ���� ������� ��������.
	UGameplayStatics::PlaySound2D(GetWorld(),
		_dropItemsSound, // ���� ������� ��������.
		1.0f, // ��������� ������.
		1.0f, // ��������� ������ ����.
		0.0f,
		nullptr,
		&Soldier
	);

	// ��������� �����.
	return true;
}

