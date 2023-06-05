// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Other/FPSEnumeration.h"
#include "Interface/FPSInterfaceEquipment.h"
#include "FPSItemBase.generated.h"

UCLASS(Abstract)
class FPSSHOOTER_API AFPSItemBase : public AActor, public IFPSInterfaceEquipment
{
	GENERATED_BODY()
	
public:	
	// �����������.
	AFPSItemBase(const FObjectInitializer& ObjectInitializer);

	virtual ~AFPSItemBase() = default;

	// ���.
	virtual void Tick(float DeltaTime) override;

	// Function Interface.

	// ���������� ����� ����� ��������� �� �������.
	virtual void OnBeginLookItem_Implementation(const class AFPSSoldier* Soldier) override;

	// ���������� ����� ����� �������� �������� �� �������.
	virtual void OnEndLookItem_Implementation(const class AFPSSoldier* Soldier) override;

	// ���������� ����� ������ �������� ��������� �������.
	virtual void OnTakeItem_Implementation(const class AFPSSoldier* Soldier) override;

	// Setters.

	// ����������� ������� � �����.
	virtual bool AttachToSoldier(const class AFPSSoldier& Soldier);

	// ���������� ������� �� �����.
	virtual bool DetachToSoldier(const class AFPSSoldier& Soldier);

	// Getters.

	// ��������� ��� ��������.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item Weight")
	FORCEINLINE float GetWeightItem() const { return _itemWeight; }

	// ��������� ID ��������.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item Enumeration")
	FORCEINLINE int32 GetItemID() { return _itemID; }

	// Variable.

	// ���� ��������.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class UStaticMeshComponent* ItemMesh;

	// ������ ������������� � ���������.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class UWidgetComponent* InteractWidget;

protected:
	// ������ ����.
	virtual void BeginPlay() override;

	// Variable.

	// �������� ����������.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Name Item", NoResetToDefault),
		Category = "Item Setting")
	FString _itemName;

	// ������ ����������.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Image Item", NoResetToDefault),
		Category = "Item Setting")
	class UTexture2D* _itemImage;

	// ���� ��������� ��������.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Item ID", NoResetToDefault),
		Category = "Item Setting")
	int32 _itemID;

	// ��������� ������������ ��������.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Item Weight", Units = "Kilograms", NoResetToDefault),
		Category = "Item Setting")
	float _itemWeight;

	// ��� ������ ���� �������� �������.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Item Socket Attach", NoResetToDefault),
		Category = "Item Setting")
	FName _socketNameAttach;

	// ��������� ������������ ��������.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Relativ Location", NoResetToDefault),
		Category = "Item Setting|Advanced")
	FVector _relativLocation;

	// ��������� �������� ��������.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Relativ Rotation", NoResetToDefault),
		Category = "Item Setting|Advanced")
	FRotator _relativRotation;

	// ���� ������� ��������.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "PickUp Item Sound", NoResetToDefault),
		Category = "Item Setting|Item Sound")
	class USoundCue* _pickUpSound;

	// ���� ����������� ��������.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Drop Item Sound", NoResetToDefault),
		Category = "Item Setting|Item Sound")
	class USoundCue* _dropItemsSound;

private:

};
