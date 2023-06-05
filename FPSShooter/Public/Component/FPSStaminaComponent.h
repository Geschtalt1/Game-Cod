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
	// Конструктор компонента стамина.
	UFPSStaminaComponent(const FObjectInitializer& ObjectInitializer);

	// Тик.
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Регулирование кол - во стамины.
	UFUNCTION(BlueprintCallable, Category = "Stamina Component")
	virtual float AddStamina(float Value);

	// Включает тип изменения стамины.
	UFUNCTION(BlueprintCallable, Category = "Stamina Component")
	virtual void PlayStaminaChange(TEnumAsByte<EStaminaChange> TypeChange);

	// Setters.

	// Установить максимальную выносливость юниту.
	UFUNCTION(BlueprintCallable, Category = "Stamina Component")
	virtual void SetStaminaMax(float NewMax);

	// Установить текущею выносливость юниту.
	UFUNCTION(BlueprintCallable, Category = "Stamina Component")
	virtual float SetStaminaAmmount(float NewHealth);

	// Getters.

	// Возращает максимальное кол-во стамины.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stamina Component")
	FORCEINLINE float GetStaminaMax() { return _staminaMax; }

	// Возращает текущее кол-во стамины.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stamina Component")
	FORCEINLINE float GetStaminaCount() { return _staminaAmmount; }

	// Variable.

	// Делегат начало спринта.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "On Move Start", Category = "Default", MultiLine = "true", OverrideNativeName = "OnMoveStart"))
	FOnSprintStartMulticastDelegate OnSprintStart;

protected:
	// Старт игры.
	virtual void BeginPlay() override;

	// Пре инцилизация компонента здоровья.
	virtual void PostInitProperties() override;

	// Variable.

	// Максимальное кол-во стамины.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (DisplayName = "Stamina Max",
		ClampMin = 0), Category = "Stamina")
	float _staminaMax;

	// Востановление стамины.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (DisplayName = "Stamina Recovery Second",
		ClampMin = 0, Units = "times"), Category = "Stamina")
	float _recoveryStamina;

	// Уменьшение стамины.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (DisplayName = "Stamina Reduction Second",
		ClampMin = 0, Units = "times"), Category = "Stamina")
	float _reductionStamina;

	// Кривая для плавного изменения стамины.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Stamina"), Category = "Stamina|Curve")
	class UCurveFloat* _curveStamina;

private:

	// Variable.

	// Таймлайн стамины.
	FTimeline _staminaTiemLine;

	// Текущеей кол-во стамины.
	float _staminaAmmount;
};
