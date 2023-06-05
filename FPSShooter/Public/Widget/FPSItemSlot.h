// Fill out your copyright notice in the Description page of Project Settings.
// Meta = (ExposeOnSpawn = true)

#pragma once

#include "CoreMinimal.h"
#include "Widget/FPSCacheReferences.h"
#include "FPSItemSlot.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class FPSSHOOTER_API UFPSItemSlot : public UFPSCacheReferences
{
	GENERATED_BODY()

public:
		// ����.
	virtual bool Initialize();

	// �����������.
	virtual void NativeConstruct() override;

	// Setters.

	// ���������� ID ��������.
	UFUNCTION(BlueprintCallable, Category = "Item ID")
	int32 SetItemID(int32 NewID);

	// Getters.

	// ��������� ID ��������.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item ID")
	FORCEINLINE int32 GetItemID() const { return _idItem; }

	// Variable.

	// �������.
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UOverlay* Overlay;

	// ������ �����.
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UImage* I_Slot;

	// ������ ��������.
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UImage* I_Item;

protected:
	
	// Variable.
	// ����������� �������� � �����.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ID", ExposeOnSpawn = true, NoResetToDefault, ClampMin = -1),
		Category = "Item ID")
	int32 _idItem = -1;
};
