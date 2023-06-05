// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/FPSCacheReferences.h"

#include "Classes/FPSUnitsBase.h"

// ����.
bool UFPSCacheReferences::Initialize()
{
	Super::Initialize();

	return true;
}

// �����������.
void UFPSCacheReferences::NativeConstruct()
{
	Super::NativeConstruct();

	_player = GetPlayerWidget();

}

// ��������� ������.
AFPSUnitsBase* UFPSCacheReferences::GetPlayerWidget() const
{
	// ������ �� �����.
	APawn* _pawnLocal = GetOwningPlayerPawn();

	// ���� �� �����.
	AFPSUnitsBase* _playerLocal = Cast<AFPSUnitsBase>(_pawnLocal);

	// ��������� ������ �� �����.
	return _playerLocal;
}