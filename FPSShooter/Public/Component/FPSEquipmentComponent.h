// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/FPSCacheReferencesComponent.h"
#include "Components/StaticMeshComponent.h"
#include "FPSEquipmentComponent.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAddItemMulticastSignature);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPSSHOOTER_API UFPSEquipmentComponent : public UFPSCacheReferencesComponent
{
	GENERATED_BODY()

public:	
	// �����������.
	UFPSEquipmentComponent(const FObjectInitializer& ObjectInitializer);

	// ��� ������������� � ���������.
#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	// �������� ������� ����� ������.
	UFUNCTION(BlueprintCallable, Category = "Item Interaction")
	bool AddItemToInventory(const int32 ID_Item, AFPSItemBase* Item);

	// �������� �������.
	UFUNCTION(BlueprintCallable, Category = "Item Interaction")
	bool RemoveItemFromInventory(const int32 ID, const bool bThrowOut);

	// �������� ������� ����� �����.

	// Setters.

	// �������� ��� �������.
	UFUNCTION(BlueprintCallable, Category = "Item Weight")
	float AddWeightSoldier(float Value);

	// Getters.
	
	// ��������� ������������ ����������� ���.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item Weight")
	FORCEINLINE float GetWeightMax() const { return _maxWeightSoldier; }

	// ��������� ������� ��� �����.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item Weight")
	FORCEINLINE float GetWeightCurrent() const { return _currentWeightSoldier; }

	// Variable.

	// ������� ���������� ���� ������� ��� ��������.
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "On Add Item", Category = "Delegate", MultiLine = "true"))
	//FOnAddItemMulticastSignature _onAddItem;

protected:
	// ������ ����.
	virtual void BeginPlay() override;

	// Variable.
	
	// ������ ���������.
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//TArray<UObject*> _items;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int32, class AFPSItemBase*> _items;

	// ��������.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Rifle"), 
		Category = "Equipment|Weapon|Rifle")
	TSubclassOf<class AFPSRifle> _rifleClassSpawn;

	// ������������ ��� �������.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Max Weight Soldier"), 
		Category = "Equipment|General")
	float _maxWeightSoldier;

private:	
	// Variable.

	// ���������� ������� � ��� ���������.
	//bool SetItemMeshComponent(EItemSlot Slot, class AFPSItemBase& Item);
	
	// ������ �� ��������.
	class AFPSRifle* _Rifle;
		
	// ������� ��� �������.
	UPROPERTY();
	float _currentWeightSoldier;
};
