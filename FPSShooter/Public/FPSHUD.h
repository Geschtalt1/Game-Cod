// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Other/FPSEnumeration.h"
#include "FPSHUD.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAmmoMaxChangedSignature, float, NewAmmoMax);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpreadCrosshireChangedSignature, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAddItemSignature, int32, ID, class AFPSItemBase*, NewItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFireModeChangedSignature, TEnumAsByte<EFireMode>, NewFireMode);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedSignature, float, NewHealth, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStaminaChangedSignature, float, NewStamina, float, MaxStamina);

/**
 * 
 */
UCLASS()
class FPSSHOOTER_API AFPSHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	// Конструктор HUD.
	AFPSHUD(const FObjectInitializer& ObjectInitializer);

	// Getters.

	// Возращает ссылку на интерфейс игрока.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Widget Player")
	FORCEINLINE UFPSHUDPlayer* GetPlayerWidgetHUD() const { return _widgetPlayerHUD;}

	// Возращает ссылку на виджет инвентаря игрока.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Widget Player")
	FORCEINLINE UFPSInventory* GetPlayerWidgetInventory() const { return _inventoryPlayer; }

	// Возращает ссылку на 

	// Variable.

	// Делегат изменения здоровья.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "On Health Changed"))
	FOnHealthChangedSignature OnHealthChanged;

	// Делегат изменения стамины.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "On Stamina Changed"))
	FOnStaminaChangedSignature OnStaminaChanged;

	// Делегат увелечения прицела.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "On Crosshire Changed"))
	FOnSpreadCrosshireChangedSignature OnCrosshireChanged;

	// Делегат вызывается если предмет был добавлен.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "On New Item"))
	FOnAddItemSignature OnNewItem;

protected:
	// Начало игры.
	virtual void BeginPlay();

	// Пре инициализация HUD. 
	virtual void PostInitProperties() override;

	// Variable.

	// Класс виджета интефейса игрока.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Player HUD", NoResetToDefault), 
		Category = "Setting HUD|Widget")
	TSubclassOf<class UFPSHUDPlayer> _classPlayerHUD;

	// Класс виджета инвенторя игрока.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Player Inventory", NoResetToDefault),
		Category = "Setting HUD|Widget")
	TSubclassOf<class UFPSInventory> _classPlayerInventory = nullptr;

private:
	
	// Variable.

	// Ссылка на виджет интерфейса игрока.
	UPROPERTY()
	class UFPSHUDPlayer* _widgetPlayerHUD;

	// Ссылка на виджет инвенторя.
	UPROPERTY()
	class UFPSInventory* _inventoryPlayer = nullptr;
};
