// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/FPSCacheReferences.h"

#include "Classes/FPSUnitsBase.h"

// Инит.
bool UFPSCacheReferences::Initialize()
{
	Super::Initialize();

	return true;
}

// Конструктор.
void UFPSCacheReferences::NativeConstruct()
{
	Super::NativeConstruct();

	_player = GetPlayerWidget();

}

// Возращает игрока.
AFPSUnitsBase* UFPSCacheReferences::GetPlayerWidget() const
{
	// Ссылка на пешку.
	APawn* _pawnLocal = GetOwningPlayerPawn();

	// Каст на юнита.
	AFPSUnitsBase* _playerLocal = Cast<AFPSUnitsBase>(_pawnLocal);

	// Возращаем ссылку на юнита.
	return _playerLocal;
}