// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FPSCacheReferences.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class FPSSHOOTER_API UFPSCacheReferences : public UUserWidget
{
	GENERATED_BODY()

public:
	// Инит.
	virtual bool Initialize();

	// Конструктор.
	virtual void NativeConstruct() override;

	// Getters.

	// Возращает ссылку на игрока виджета.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Widget References")
	FORCEINLINE AFPSUnitsBase* GetPlayer() const { return _player; }

	// Возращает игрока.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Widget References")
	class AFPSUnitsBase* GetPlayerWidget() const;

private:

	// Ссылка на игрока.
	UPROPERTY()
	class AFPSUnitsBase* _player;
	
};
