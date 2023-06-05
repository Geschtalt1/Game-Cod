// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Classes/FPSItemBase.h"
#include "FPSEquipmentBase.generated.h"


UCLASS(Abstract)
class FPSSHOOTER_API AFPSEquipmentBase : public AFPSItemBase
{
	GENERATED_BODY()
	
public:	
	// Конструктор.
	AFPSEquipmentBase(const FObjectInitializer& ObjectInitializer);

	// Деструктор.
	virtual ~AFPSEquipmentBase() = default;

	// Setters.

	// Установить владельца экипировки.
	//UFUNCTION(BlueprintCallable, Category = "Equipment Owner")
	virtual void SetEquipmentOwner(class AFPSSoldier& NewOwner);

	// Getters.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Equipment Owner")
	FORCEINLINE AFPSSoldier* GetEquipmentOwner() const { return _OwnerEquipment; }

	// Variable.

protected:
	// Начало игры.
	virtual void BeginPlay() override;

	// Setters.
	
	// Getters.

	// Variable.

	// Название экипировки.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Name Item", NoResetToDefault),
		Category = "Equipment Setting")
	FString EquipmentName;

	// Иконка экипировки.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Image Item", NoResetToDefault),
		Category = "Equipment Setting")
	class UTexture2D* EquipmentImage;

private:	

	// Setters.

	// Getters.

	// Variable.
	UPROPERTY()
	class AFPSSoldier* _OwnerEquipment;

};
