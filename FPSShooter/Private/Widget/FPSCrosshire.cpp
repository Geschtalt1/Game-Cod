// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/FPSCrosshire.h"

#include "Classes/FPSUnitsBase.h"

#include "FPSHUD.h"

#include "Components/Border.h"
#include "Components/CanvasPanel.h"

// Инит.
bool UFPSCrosshire::Initialize()
{
	Super::Initialize();

	return true;
}

// Конструктор.
void UFPSCrosshire::NativeConstruct()
{
	Super::NativeConstruct();
}

// Тик.
void UFPSCrosshire::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::Tick(MyGeometry, InDeltaTime);

	// Увеличиваем прицел.
	SetAimOffset(B_Left, _spreadCrosshire * (-1.0f), 0.0f);
	SetAimOffset(B_Right, _spreadCrosshire, 0.0f);
	SetAimOffset(B_Up, 0.0f, _spreadCrosshire * (-1.0f));
	SetAimOffset(B_Down, 0.0f, _spreadCrosshire);

}

// Установить смещение компонента (Работает на тике).
void UFPSCrosshire::SetAimOffset(UContentWidget* WidgetComponent, float X, float Y)
{
	// Проверяем выбран ли компонет.
	if (WidgetComponent)
	{
		// Записываем текущее местоположение компонента.
		const FVector2D _locationScreen = 
			WidgetComponent->RenderTransform.Translation;

		// Плавно меняем значение по X.
		const float _newX = FMath::Lerp(
			_locationScreen.X, // Текущее расположение.
			X, // Следующее ррасположение.
			_alphaCrosshire // Плавность.
		);

		// Плавно меняем значение по Y.
		const float _newY = FMath::Lerp(
			_locationScreen.Y, // Текущее расположение.
			Y, // Следующее ррасположение.
			_alphaCrosshire // Плавность.
		);

		// Устанавливаем новое расположение компонента.
		WidgetComponent->SetRenderTranslation(FVector2D(_newX, _newY));
	}
}

// Добавляет разброс прицела.
void UFPSCrosshire::AddSpreadCrosshire(float Value)
{
	// Зажимаем разброс прицела.
	_spreadCrosshire = FMath::Clamp(
		_spreadCrosshire + Value, // Складываем текущее разброс с добавленным значение.
		0.0f, // Минимальное значение.
		_spreadCrosshireMax // Максимальное.
	);

	// Возращаем разброс прицела.
	//return _spreadCrosshire;
}