// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSHUD.h"

#include "Widget/FPSHUDPlayer.h"
#include "Kismet/GameplayStatics.h"

#include "Widget/FPSInventory.h"

#include "Widget/FPSCrosshire.h"

// ����������� HUD.
AFPSHUD::AFPSHUD(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	_classPlayerHUD = nullptr;
	_widgetPlayerHUD = nullptr;
	_classPlayerInventory = nullptr;
	_inventoryPlayer = nullptr;
}

// ��� ����������� HUD.
void AFPSHUD::PostInitProperties()
{
	Super::PostInitProperties();
}

// ������ ����.
void AFPSHUD::BeginPlay()
{
	Super::BeginPlay();

	// ��������� ��� ����� ������� ���������� ������ ������.
	if (_classPlayerHUD && _classPlayerInventory)
	{
		// ������� ������ ���������� ������.
		_widgetPlayerHUD = CreateWidget<UFPSHUDPlayer>(
			GetOwningPlayerController(), // �������� �������.
			_classPlayerHUD); // ����� �������.

		// ������� ������ ��������� ������.
		_inventoryPlayer = CreateWidget<UFPSInventory>(
			GetOwningPlayerController(), // �������� �������.
			_classPlayerInventory); // ����� �������.

		// ��������� ���������� ������ �� ��������� ������.
		if (_widgetPlayerHUD)
		{
			// ������� ������ �� �����.
			_widgetPlayerHUD->AddToViewport();

			// ������ �� ������ �������.
			UFPSCrosshire* _widgetCrosshire = _widgetPlayerHUD->WB_Crosshire;

			// ����������� ������� ��������� �������, � �������� �� ������� ������.
			OnStaminaChanged.AddDynamic(_widgetPlayerHUD, &UFPSHUDPlayer::SetStamina);

			// ����������� ������� ��������� ��������, � �������� �� ������� ������.
			OnHealthChanged.AddDynamic(_widgetPlayerHUD, &UFPSHUDPlayer::SetHealth);

			// ����������� ������� ��������� �������.
			OnCrosshireChanged.AddDynamic(_widgetCrosshire, &UFPSCrosshire::AddSpreadCrosshire);
		}
	}
}