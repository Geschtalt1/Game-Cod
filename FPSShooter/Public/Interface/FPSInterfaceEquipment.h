// Fill out your copyright notice in the Description page of Project Settings.
//bool bUsesInterface = ClassOfYourObject->ImplementsInterface(UYourInterface::StaticClass());
//bool bUsesInterface = YourObject->Implements<UYourInterface>();

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FPSInterfaceEquipment.generated.h"

// ���� ����� �� ����� ��������������.
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

	// �������� ������� ���������� � ���� �����. ��� �����, ������� ����� ����������� ��� ���������� ����� ����������.
public:
	
	// ����� �������� �� �� �������.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Event Item")
	void OnBeginLookItem(const class AFPSSoldier* Soldier);

	// �������� �������� �� �������.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Event Item")
	void OnEndLookItem(const class AFPSSoldier* Soldier);

	// ����� �������.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Event Item")
	void OnTakeItem(const class AFPSSoldier* Soldier);
};
