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
	// Конструктор.
	UFPSEquipmentComponent(const FObjectInitializer& ObjectInitializer);

	// Пре инициализация в редакторе.
#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	// Добавить предмет через ссылку.
	UFUNCTION(BlueprintCallable, Category = "Item Interaction")
	bool AddItemToInventory(const int32 ID_Item, AFPSItemBase* Item);

	// Выкинуть предмет.
	UFUNCTION(BlueprintCallable, Category = "Item Interaction")
	bool RemoveItemFromInventory(const int32 ID, const bool bThrowOut);

	// Добавить предмет через класс.

	// Setters.

	// Добавить вес солдату.
	UFUNCTION(BlueprintCallable, Category = "Item Weight")
	float AddWeightSoldier(float Value);

	// Getters.
	
	// Возращает максимальный переносимый вес.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item Weight")
	FORCEINLINE float GetWeightMax() const { return _maxWeightSoldier; }

	// Возращает текущий вес юнита.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item Weight")
	FORCEINLINE float GetWeightCurrent() const { return _currentWeightSoldier; }

	// Variable.

	// Делегат вызывается если предмет был добавлен.
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "On Add Item", Category = "Delegate", MultiLine = "true"))
	//FOnAddItemMulticastSignature _onAddItem;

protected:
	// Начало игры.
	virtual void BeginPlay() override;

	// Variable.
	
	// Массив предметов.
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//TArray<UObject*> _items;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int32, class AFPSItemBase*> _items;

	// Винтовка.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Rifle"), 
		Category = "Equipment|Weapon|Rifle")
	TSubclassOf<class AFPSRifle> _rifleClassSpawn;

	// Максимальный вес солдата.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Max Weight Soldier"), 
		Category = "Equipment|General")
	float _maxWeightSoldier;

private:	
	// Variable.

	// Установить предмет в меш компонент.
	//bool SetItemMeshComponent(EItemSlot Slot, class AFPSItemBase& Item);
	
	// Ссылка на винтовку.
	class AFPSRifle* _Rifle;
		
	// Текущий вес солдата.
	UPROPERTY();
	float _currentWeightSoldier;
};
