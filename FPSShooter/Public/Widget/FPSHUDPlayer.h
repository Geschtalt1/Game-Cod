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
	// ����.
	virtual bool Initialize();

	// �����������.
	virtual void NativeConstruct() override;

	// Setters.

	// ������������� ����� �������� ������� � Progress bar.
	UFUNCTION(BlueprintCallable, Category = "Player State HUD")
	virtual void SetStamina(float NewStamina, float MaxStamina);

	// ������������� ����� �������� �������� � Progress bar.
	UFUNCTION(BlueprintCallable, Category = "Player State HUD")
	virtual void SetHealth(float NewHealth, float MaxHealth);

	// Variable.

	// ������ �������.
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UFPSCrosshire* WB_Crosshire;

	// ����� ���� ������� ���-�� ������.
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* TB_CurrentAmmo;

	// ������� ��.
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UProgressBar* PB_Health;

	// ������� ������������.
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UProgressBar* PB_Stamina;
};
