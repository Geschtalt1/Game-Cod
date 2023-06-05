// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/FPSItemSlot.h"

#include "Components/Overlay.h"
#include "Components/Image.h"

// Инит.
bool UFPSItemSlot::Initialize()
{
	Super::Initialize();

	return true;
}

// Конструктор.
void UFPSItemSlot::NativeConstruct()
{
	Super::NativeConstruct();

}

// Установить ID предмету.
int32 UFPSItemSlot::SetItemID(int32 NewID)
{
	_idItem = NewID;

	return _idItem;
}