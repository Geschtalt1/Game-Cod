// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/FPSCacheReferencesComponent.h"
#include "Components/TimelineComponent.h"
#include "Other/FPSEnumeration.h"
#include "FPSStaminaComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSprintStartMulticastDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPSSHOOTER_API UFPSStaminaComponent : public UFPSCacheReferencesComponent
{
	GENERATED_BODY()


public:	
	// ����������� ���������� �������.
	UFPSStaminaComponent(const FObjectInitializer& ObjectInitializer);

	// ���.
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// ������������� ��� - �� �������.
	UFUNCTION(BlueprintCallable, Category = "Stamina Component")
	virtual float AddStamina(float Value);

	// �������� ��� ��������� �������.
	UFUNCTION(BlueprintCallable, Category = "Stamina Component")
	virtual void PlayStaminaChange(TEnumAsByte<EStaminaChange> TypeChange);

	// Setters.

	// ���������� ������������ ������������ �����.
	UFUNCTION(BlueprintCallable, Category = "Stamina Component")
	virtual void SetStaminaMax(float NewMax);

	// ���������� ������� ������������ �����.
	UFUNCTION(BlueprintCallable, Category = "Stamina Component")
	virtual float SetStaminaAmmount(float NewHealth);

	// Getters.

	// ��������� ������������ ���-�� �������.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stamina Component")
	FORCEINLINE float GetStaminaMax() { return _staminaMax; }

	// ��������� ������� ���-�� �������.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stamina Component")
	FORCEINLINE float GetStaminaCount() { return _staminaAmmount; }

	// Variable.

	// ������� ������ �������.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "On Move Start", Category = "Default", MultiLine = "true", OverrideNativeName = "OnMoveStart"))
	FOnSprintStartMulticastDelegate OnSprintStart;

protected:
	// ����� ����.
	virtual void BeginPlay() override;

	// ��� ����������� ���������� ��������.
	virtual void PostInitProperties() override;

	// Variable.

	// ������������ ���-�� �������.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (DisplayName = "Stamina Max",
		ClampMin = 0), Category = "Stamina")
	float _staminaMax;

	// ������������� �������.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (DisplayName = "Stamina Recovery Second",
		ClampMin = 0, Units = "times"), Category = "Stamina")
	float _recoveryStamina;

	// ���������� �������.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (DisplayName = "Stamina Reduction Second",
		ClampMin = 0, Units = "times"), Category = "Stamina")
	float _reductionStamina;

	// ������ ��� �������� ��������� �������.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Stamina"), Category = "Stamina|Curve")
	class UCurveFloat* _curveStamina;

private:

	// Variable.

	// �������� �������.
	FTimeline _staminaTiemLine;

	// �������� ���-�� �������.
	float _staminaAmmount;
};
