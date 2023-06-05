//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("This is an on screen message!")); прин стринг
//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("TeamNumberUnit: %i"), TeamNumberUnit));


#include "Classes/FPSUnitsBase.h"

#include "Component/FPSHealthComponent.h"

#include "FPSPlayerController.h"
#include "FPSHUD.h"

// Конструктор юнита.
AFPSUnitsBase::AFPSUnitsBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Тик отключен.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// Компонент здоровья.
	Health = CreateDefaultSubobject<UFPSHealthComponent>(TEXT("Health Component"));

	_hudPlayer = nullptr;

	_bAim = false;
	_bFireEnable = true;

	// Делегат смерти, в компоненте здоровья, привязываем к функции убить юнита.
	GetHealth()->OnDead.AddDynamic(this, &AFPSUnitsBase::OnKillUnit);
}

// Пре инцилизация юнита.
void AFPSUnitsBase::PostInitProperties()
{
	Super::PostInitProperties();

}

// Начало игры.
void AFPSUnitsBase::BeginPlay()
{
	Super::BeginPlay();

	// Каст на контроллер.
	AFPSPlayerController* _playerController = Cast<AFPSPlayerController>(GetOwner());

	// Если каст успешный.
	if (_playerController)
	{
		// Ссылка на HUD из контроллера.
		AHUD* _hudLocal = _playerController->GetHUD();

		// Делаем каст на HUD игрока и записываем в переменную.
		_hudPlayer = Cast<AFPSHUD>(_hudLocal);
	}
}

// Евент тик.
void AFPSUnitsBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Если скорость не равна нулю.
	if (GetPlayerHUD())
	{
		if (GetSpeedUnit() != 0.0f)
		{
			// Увеличиваем прицел.
			GetPlayerHUD()->OnCrosshireChanged.Broadcast(10.0f);
		}
		else
		{
			// Уменьшаем прицел.
			GetPlayerHUD()->OnCrosshireChanged.Broadcast(-10.0f);
		}
	}
	else
	{
		PrimaryActorTick.bCanEverTick = false;
		PrimaryActorTick.bStartWithTickEnabled = false;
	}
}

// Евент взаимодействия с предметом.
void AFPSUnitsBase::OnInteractionTrace_Implementation()
{
}

// Возращает целится ли юнит в данный момент.
bool AFPSUnitsBase::IsAimUnit() const
{
	return (bool)_bAim;
}

// Возращает передвигается ли юнит на спринте.
bool AFPSUnitsBase::IsRunUnit() const
{
	return false;
}

// Возращает жив ли юнит в текущий момент.
bool AFPSUnitsBase::GetAlive() const
{
	// Записываем текущее, кол-во хп.
	const float _healthCurrent = 
		GetHealth()->GetHealthCount();
	
	// Если текущее ХП больше нуля, значит юнит жив.
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

// Разрешить стрелять из оружия или нет.
void AFPSUnitsBase::SetFireEnable(bool bEnable)
{
	_bFireEnable = bEnable;
}

// Юнит получает урон.
float AFPSUnitsBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	GetHealth()->AnyDammage(DamageAmount);

	return DamageAmount;
}
