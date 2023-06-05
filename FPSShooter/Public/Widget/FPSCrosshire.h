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
	// ����.
	virtual bool Initialize() override;

	// �����������.
	virtual void NativeConstruct() override;

	// ���.
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// ���������� �������� ���������� (�������� �� ����).
	UFUNCTION(BlueprintCallable, Category = "Setting Crosshire")
	virtual void SetAimOffset(UContentWidget* WidgetComponent, float X, float Y);

	// �������� ������� �������.
	UFUNCTION(BlueprintCallable, Category = "Setting Crosshire")
	virtual void AddSpreadCrosshire(float Value);

	// Variable.

protected:
	
	// Variable.

	// ����� ��������� �������� �������.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (DisplayName = "Alpha Crosshire", NoResetToDefault,
		ClampMin = 0, ClampMax = 1), Category = "Crosshire Setting")
	float _alphaCrosshire = 0.15f;

	// ������������ ������� �������.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (DisplayName = "Spread Crosshire Max", NoResetToDefault,
		ClampMin = 0), Category = "Crosshire Setting")
	float _spreadCrosshireMax = 100.0f;

	// ������ ����� �������.
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UBorder* B_Right;

	// ����� ����� �������.
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UBorder* B_Left;

	// ������� ����� �������.
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UBorder* B_Up;

	// ������ ����� �������.
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UBorder* B_Down;

private:

	// ���������� ��������� ������� �������.
	float _spreadCrosshire;
};
