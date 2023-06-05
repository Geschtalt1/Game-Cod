// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Classes/FPSItemBase.h"
#include "FPSDefense.generated.h"

/**
 * 
 */
UCLASS()
class FPSSHOOTER_API AFPSDefense : public AFPSItemBase
{
	GENERATED_BODY()

public:
	// Конструктор брони.
	AFPSDefense(const FObjectInitializer& ObjectInitializer);

	// Пре инициализация в редакторе.
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	// Setters.

	// Установить процент брони (0 - 100).
	UFUNCTION(BlueprintCallable, Category = "Defense Armor")
	virtual int32 SetArmorProcent(float NewProcent);

	// Getters.

	// Возращает уровнь брони.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Defense Armor")
	FORCEINLINE int32 GetArmorLevel() const { return _armorDefense; }

	// Возращает процент брони.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Defense Armor")
	FORCEINLINE float GetArmorProcent() const { return _armorLevel; }

	// Variable.

	// Компонент сцены.
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	//class USceneComponent* Scene;

protected:
	
	// Variable.

	// Проценнт защиты.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 100, NoResetToDefault, Units = "Percent"),
		Category = "Item Setting|Defense Setting|Protection Level")
	float _armorDefense;

private:

	// Variable.

	//Уровень защиты брони
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true, ClampMin = 0, ClampMax = 3, NoResetToDefault),
		Category = "Item Setting|Defense Setting|Protection Level")
	int32 _armorLevel;
	
};
