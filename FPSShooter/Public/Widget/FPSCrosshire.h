// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/FPSCacheReferences.h"
#include "FPSCrosshire.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class FPSSHOOTER_API UFPSCrosshire : public UFPSCacheReferences
{
	GENERATED_BODY()
	
public:
	// Инит.
	virtual bool Initialize() override;

	// Конструктор.
	virtual void NativeConstruct() override;

	// Тик.
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// Установить смещение компонента (Работает на тике).
	UFUNCTION(BlueprintCallable, Category = "Setting Crosshire")
	virtual void SetAimOffset(UContentWidget* WidgetComponent, float X, float Y);

	// Добавить разброс прицела.
	UFUNCTION(BlueprintCallable, Category = "Setting Crosshire")
	virtual void AddSpreadCrosshire(float Value);

	// Variable.

protected:
	
	// Variable.

	// Альфа плавности анимации прицела.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (DisplayName = "Alpha Crosshire", NoResetToDefault,
		ClampMin = 0, ClampMax = 1), Category = "Crosshire Setting")
	float _alphaCrosshire = 0.15f;

	// Максимальный разброс прицела.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (DisplayName = "Spread Crosshire Max", NoResetToDefault,
		ClampMin = 0), Category = "Crosshire Setting")
	float _spreadCrosshireMax = 100.0f;

	// Правая часть прицела.
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UBorder* B_Right;

	// Левая часть прицела.
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UBorder* B_Left;

	// Верхняя часть прицела.
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UBorder* B_Up;

	// Нижняя часть прицела.
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UBorder* B_Down;

private:

	// Перемменая увеливает разброс прицела.
	float _spreadCrosshire;
};
