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
	// ����.
	virtual bool Initialize();

	// �����������.
	virtual void NativeConstruct() override;

	// Getters.

	// ��������� ������ �� ������ �������.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Widget References")
	FORCEINLINE AFPSUnitsBase* GetPlayer() const { return _player; }

	// ��������� ������.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Widget References")
	class AFPSUnitsBase* GetPlayerWidget() const;

private:

	// ������ �� ������.
	UPROPERTY()
	class AFPSUnitsBase* _player;
	
};
