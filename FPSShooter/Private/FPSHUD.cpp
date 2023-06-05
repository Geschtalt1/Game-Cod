// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSHUD.h"

#include "Widget/FPSHUDPlayer.h"
#include "Kismet/GameplayStatics.h"

#include "Widget/FPSInventory.h"

#include "Widget/FPSCrosshire.h"

// Конструктор HUD.
AFPSHUD::AFPSHUD(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	_classPlayerHUD = nullptr;
	_widgetPlayerHUD = nullptr;
	_classPlayerInventory = nullptr;
	_inventoryPlayer = nullptr;
}

// Пре инцилизация HUD.
void AFPSHUD::PostInitProperties()
{
	Super::PostInitProperties();
}

// Начало игры.
void AFPSHUD::BeginPlay()
{
	Super::BeginPlay();

	// Проверяем что класс виджета интерфейса игрока выбран.
	if (_classPlayerHUD && _classPlayerInventory)
	{
		// Создаем виджет интерфейса игрока.
		_widgetPlayerHUD = CreateWidget<UFPSHUDPlayer>(
			GetOwningPlayerController(), // Владелец виджета.
			_classPlayerHUD); // Класс виджета.

		// Создаем виджет инвенторя игрока.
		_inventoryPlayer = CreateWidget<UFPSInventory>(
			GetOwningPlayerController(), // Владелец виджета.
			_classPlayerInventory); // Класс виджета.

		// Проверяем валидность ссылки на интерфейс игрока.
		if (_widgetPlayerHUD)
		{
			// Выводим виджет на экран.
			_widgetPlayerHUD->AddToViewport();

			// Ссылка на виджет прицела.
			UFPSCrosshire* _widgetCrosshire = _widgetPlayerHUD->WB_Crosshire;

			// Привязываем делегат изменения стамины, с функцией из виджета игрока.
			OnStaminaChanged.AddDynamic(_widgetPlayerHUD, &UFPSHUDPlayer::SetStamina);

			// Привязываем делегат изменения здоровья, с функцией из виджета игрока.
			OnHealthChanged.AddDynamic(_widgetPlayerHUD, &UFPSHUDPlayer::SetHealth);

			// Привязываем делегат изменения прицела.
			OnCrosshireChanged.AddDynamic(_widgetCrosshire, &UFPSCrosshire::AddSpreadCrosshire);
		}
	}
}