//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("This is an on screen message!")); ���� ������
//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("TeamNumberUnit: %i"), TeamNumberUnit));


#include "Classes/FPSUnitsBase.h"

#include "Component/FPSHealthComponent.h"

#include "FPSPlayerController.h"
#include "FPSHUD.h"

// ����������� �����.
AFPSUnitsBase::AFPSUnitsBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// ��� ��������.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// ��������� ��������.
	Health = CreateDefaultSubobject<UFPSHealthComponent>(TEXT("Health Component"));

	_hudPlayer = nullptr;

	_bAim = false;
	_bFireEnable = true;

	// ������� ������, � ���������� ��������, ����������� � ������� ����� �����.
	GetHealth()->OnDead.AddDynamic(this, &AFPSUnitsBase::OnKillUnit);
}

// ��� ����������� �����.
void AFPSUnitsBase::PostInitProperties()
{
	Super::PostInitProperties();

}

// ������ ����.
void AFPSUnitsBase::BeginPlay()
{
	Super::BeginPlay();

	// ���� �� ����������.
	AFPSPlayerController* _playerController = Cast<AFPSPlayerController>(GetOwner());

	// ���� ���� ��������.
	if (_playerController)
	{
		// ������ �� HUD �� �����������.
		AHUD* _hudLocal = _playerController->GetHUD();

		// ������ ���� �� HUD ������ � ���������� � ����������.
		_hudPlayer = Cast<AFPSHUD>(_hudLocal);
	}
}

// ����� ���.
void AFPSUnitsBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ���� �������� �� ����� ����.
	if (GetPlayerHUD())
	{
		if (GetSpeedUnit() != 0.0f)
		{
			// ����������� ������.
			GetPlayerHUD()->OnCrosshireChanged.Broadcast(10.0f);
		}
		else
		{
			// ��������� ������.
			GetPlayerHUD()->OnCrosshireChanged.Broadcast(-10.0f);
		}
	}
	else
	{
		PrimaryActorTick.bCanEverTick = false;
		PrimaryActorTick.bStartWithTickEnabled = false;
	}
}

// ����� �������������� � ���������.
void AFPSUnitsBase::OnInteractionTrace_Implementation()
{
}

// ��������� ������� �� ���� � ������ ������.
bool AFPSUnitsBase::IsAimUnit() const
{
	return (bool)_bAim;
}

// ��������� ������������� �� ���� �� �������.
bool AFPSUnitsBase::IsRunUnit() const
{
	return false;
}

// ��������� ��� �� ���� � ������� ������.
bool AFPSUnitsBase::GetAlive() const
{
	// ���������� �������, ���-�� ��.
	const float _healthCurrent = 
		GetHealth()->GetHealthCount();
	
	// ���� ������� �� ������ ����, ������ ���� ���.
	return _healthCurrent > 0.0f;
}

bool AFPSUnitsBase::IsReloadUnit() const
{
	return false;
}

void AFPSUnitsBase::SetSpeedMax(float NewSpeed)
{
}

void AFPSUnitsBase::SetSpeedRun(bool bSprint)
{
}

void AFPSUnitsBase::SetCameraPosition(TEnumAsByte<ECameraPos> Position)
{
}

void AFPSUnitsBase::OnFireMainUnit()
{
}

void AFPSUnitsBase::OnMoveForward(float Value)
{
}

void AFPSUnitsBase::OnMoveRight(float Value)
{
}

void AFPSUnitsBase::OnKillUnit()
{
}

void AFPSUnitsBase::OnReloadUnit()
{
}

void AFPSUnitsBase::OnAimUnit(bool bActive)
{
}

void AFPSUnitsBase::OnStopFire()
{
}

// ��������� �������� �� ������ ��� ���.
void AFPSUnitsBase::SetFireEnable(bool bEnable)
{
	_bFireEnable = bEnable;
}

// ���� �������� ����.
float AFPSUnitsBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	GetHealth()->AnyDammage(DamageAmount);

	return DamageAmount;
}
