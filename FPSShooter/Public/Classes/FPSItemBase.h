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
	// Конструктор.
	AFPSItemBase(const FObjectInitializer& ObjectInitializer);

	virtual ~AFPSItemBase() = default;

	// Тик.
	virtual void Tick(float DeltaTime) override;

	// Function Interface.

	// Вызывается когда игрок посмотрел на предмет.
	virtual void OnBeginLookItem_Implementation(const class AFPSSoldier* Soldier) override;

	// Вызывается когда игрок закончил смотреть на предмет.
	virtual void OnEndLookItem_Implementation(const class AFPSSoldier* Soldier) override;

	// Вызывается когда игрока нажимает подобрать предмет.
	virtual void OnTakeItem_Implementation(const class AFPSSoldier* Soldier) override;

	// Setters.

	// Прикрепляем предмет к юниту.
	virtual bool AttachToSoldier(const class AFPSSoldier& Soldier);

	// открепляем предмет от юнита.
	virtual bool DetachToSoldier(const class AFPSSoldier& Soldier);

	// Getters.

	// Возращает вес предмета.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item Weight")
	FORCEINLINE float GetWeightItem() const { return _itemWeight; }

	// Возращает ID предмета.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item Enumeration")
	FORCEINLINE int32 GetItemID() { return _itemID; }

	// Variable.

	// Мешь предмета.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class UStaticMeshComponent* ItemMesh;

	// Виджет взимодействия с предметом.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class UWidgetComponent* InteractWidget;

protected:
	// Начало игры.
	virtual void BeginPlay() override;

	// Variable.

	// Название экипировки.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Name Item", NoResetToDefault),
		Category = "Item Setting")
	FString _itemName;

	// Иконка экипировки.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Image Item", NoResetToDefault),
		Category = "Item Setting")
	class UTexture2D* _itemImage;

	// Слот крепления предмета.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Item ID", NoResetToDefault),
		Category = "Item Setting")
	int32 _itemID;

	// Настройка расположения предмета.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Item Weight", Units = "Kilograms", NoResetToDefault),
		Category = "Item Setting")
	float _itemWeight;

	// Имя сокета куда крепится предмет.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Item Socket Attach", NoResetToDefault),
		Category = "Item Setting")
	FName _socketNameAttach;

	// Настройка расположения предмета.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Relativ Location", NoResetToDefault),
		Category = "Item Setting|Advanced")
	FVector _relativLocation;

	// Настройка вращения предмета.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Relativ Rotation", NoResetToDefault),
		Category = "Item Setting|Advanced")
	FRotator _relativRotation;

	// Звук подбора предмета.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "PickUp Item Sound", NoResetToDefault),
		Category = "Item Setting|Item Sound")
	class USoundCue* _pickUpSound;

	// Звук выкидывания предмета.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Drop Item Sound", NoResetToDefault),
		Category = "Item Setting|Item Sound")
	class USoundCue* _dropItemsSound;

private:

};
