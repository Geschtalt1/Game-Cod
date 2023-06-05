// Fill out your copyright notice in the Description page of Project Settings.
//bool bUsesInterface = ClassOfYourObject->ImplementsInterface(UYourInterface::StaticClass());
//bool bUsesInterface = YourObject->Implements<UYourInterface>();

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FPSInterfaceEquipment.generated.h"

// Этот класс не нужно модифицировать.
UINTERFACE(MinimalAPI)
class UFPSInterfaceEquipment : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FPSSHOOTER_API IFPSInterfaceEquipment
{
	GENERATED_BODY()

	// Добавьте функции интерфейса в этот класс. Это класс, который будет унаследован для реализации этого интерфейса.
public:
	
	// Начал смотреть на на предмет.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Event Item")
	void OnBeginLookItem(const class AFPSSoldier* Soldier);

	// Закончил смотреть на предмет.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Event Item")
	void OnEndLookItem(const class AFPSSoldier* Soldier);

	// Взять предмет.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Event Item")
	void OnTakeItem(const class AFPSSoldier* Soldier);
};
