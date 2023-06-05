// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/FPSCrosshire.h"

#include "Classes/FPSUnitsBase.h"

#include "FPSHUD.h"

#include "Components/Border.h"
#include "Components/CanvasPanel.h"

// ����.
bool UFPSCrosshire::Initialize()
{
	Super::Initialize();

	return true;
}

// �����������.
void UFPSCrosshire::NativeConstruct()
{
	Super::NativeConstruct();
}

// ���.
void UFPSCrosshire::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::Tick(MyGeometry, InDeltaTime);

	// ����������� ������.
	SetAimOffset(B_Left, _spreadCrosshire * (-1.0f), 0.0f);
	SetAimOffset(B_Right, _spreadCrosshire, 0.0f);
	SetAimOffset(B_Up, 0.0f, _spreadCrosshire * (-1.0f));
	SetAimOffset(B_Down, 0.0f, _spreadCrosshire);

}

// ���������� �������� ���������� (�������� �� ����).
void UFPSCrosshire::SetAimOffset(UContentWidget* WidgetComponent, float X, float Y)
{
	// ��������� ������ �� ��������.
	if (WidgetComponent)
	{
		// ���������� ������� �������������� ����������.
		const FVector2D _locationScreen = 
			WidgetComponent->RenderTransform.Translation;

		// ������ ������ �������� �� X.
		const float _newX = FMath::Lerp(
			_locationScreen.X, // ������� ������������.
			X, // ��������� �������������.
			_alphaCrosshire // ���������.
		);

		// ������ ������ �������� �� Y.
		const float _newY = FMath::Lerp(
			_locationScreen.Y, // ������� ������������.
			Y, // ��������� �������������.
			_alphaCrosshire // ���������.
		);

		// ������������� ����� ������������ ����������.
		WidgetComponent->SetRenderTranslation(FVector2D(_newX, _newY));
	}
}

// ��������� ������� �������.
void UFPSCrosshire::AddSpreadCrosshire(float Value)
{
	// �������� ������� �������.
	_spreadCrosshire = FMath::Clamp(
		_spreadCrosshire + Value, // ���������� ������� ������� � ����������� ��������.
		0.0f, // ����������� ��������.
		_spreadCrosshireMax // ������������.
	);

	// ��������� ������� �������.
	//return _spreadCrosshire;
}