// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget/FPSCacheReferences.h"
#include "FPSHUDPlayer.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class FPSSHOOTER_API UFPSHUDPlayer : public UFPSCacheReferences
{
	GENERATED_BODY()

public:
	// Инит.
	virtual bool Initialize();

	// Конструктор.
	virtual void NativeConstruct() override;

	// Setters.

	// Устанавливает новое значение стамины в Progress bar.
	UFUNCTION(BlueprintCallable, Category = "Player State HUD")
	virtual void SetStamina(float NewStamina, float MaxStamina);

	// Устанавливает новое значение здоровья в Progress bar.
	UFUNCTION(BlueprintCallable, Category = "Player State HUD")
	virtual void SetHealth(float NewHealth, float MaxHealth);

	// Variable.

	// Виджет прицела.
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UFPSCrosshire* WB_Crosshire;

	// Текст блок текущих кол-во патрон.
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* TB_CurrentAmmo;

	// Полоска ХП.
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UProgressBar* PB_Health;

	// Полоска выносливости.
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UProgressBar* PB_Stamina;
};
