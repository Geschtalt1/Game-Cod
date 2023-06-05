
#include "FPSPlayerController.h"

#include "Classes/FPSUnitsBase.h"
#include "Classes/UnitsChild/FPSSoldier.h"
#include "Classes/WeaponChild/FPSFireArms.h"

#include "Component/FPSMovementComponent.h"
#include "Components/InputComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Math/UnrealMathUtility.h"

#include "Interface/FPSInterfaceControll.h"

#include "Actions/PawnActionsComponent.h"

// Конструктор контроллера.
AFPSPlayerController::AFPSPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	_player = nullptr;

}

// Управление.
void AFPSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	check(InputComponent);

	// BindAxis.
	InputComponent->BindAxis("MoveForward", this, &AFPSPlayerController::CallMoveForward);
	InputComponent->BindAxis("MoveRight", this, &AFPSPlayerController::CallMoveRight);
	InputComponent->BindAxis("Turn", this, &AFPSPlayerController::CallTurn);
	InputComponent->BindAxis("LookUp", this, &AFPSPlayerController::CallLookUp);

	// BindAction.

	InputComponent->BindAction("Run", IE_Pressed, this, &AFPSPlayerController::CallSprintOn);
	InputComponent->BindAction("Run", IE_Released, this, &AFPSPlayerController::CallSprintOff);
	InputComponent->BindAction("FireMain", IE_Pressed, this, &AFPSPlayerController::CallFireMainOn);
	InputComponent->BindAction("FireMain", IE_Released, this, &AFPSPlayerController::CallFireMainOff);
	//InputComponent->BindAction("FireMode", IE_Pressed, this, &AFPSPlayerController::CallFireMode);
	InputComponent->BindAction("Crouch", IE_Pressed, this, &AFPSPlayerController::CallCrouch);
	InputComponent->BindAction("Reload", IE_Pressed, this, &AFPSPlayerController::CallReload);
	InputComponent->BindAction("Aim", IE_Pressed, this, &AFPSPlayerController::CallAimOn);
	InputComponent->BindAction("Aim", IE_Released, this, &AFPSPlayerController::CallAimOff);
	InputComponent->BindAction("Iteraction", IE_Pressed, this, &AFPSPlayerController::CallIteraction);
}

// Начало игры.
void AFPSPlayerController::BeginPlay()
{
	if (IsLocalPlayerController()) {
		_player = Cast<AFPSUnitsBase>(GetPawn());; // Ссылка на контроллируемый объект.
	}
}

// Движение вперед - назад.
void AFPSPlayerController::CallMoveForward(float Value)
{
	if (_player) {
		_player->OnMoveForward(Value);
	}
}

// Движение влево - вправо.
void AFPSPlayerController::CallMoveRight(float Value)
{
	if (_player) {
		_player->OnMoveRight(Value);
	}
}

// Поворот камеры по горизонтали.
void AFPSPlayerController::CallTurn(float Value)
{
	if (_player) {
		_player->AddControllerYawInput(Value);
	}
}

// Поворот камеры по вертикали.
void AFPSPlayerController::CallLookUp(float Value)
{
	if (_player) {
		_player->AddControllerPitchInput(Value);
	}
}

// Спринт включен.
void AFPSPlayerController::CallSprintOn()
{
	if (_player) // Если игрок действителен.
	{
		if (!_player->IsRunUnit()) { // И если он в данный момент не ускорен.
			_player->SetSpeedRun(true); // Включаем ускорение.
		}
	}
}

// Спринт выключен.
void AFPSPlayerController::CallSprintOff()
{
	if (_player)  // Если игрок действителен.
	{ 
		_player->SetSpeedRun(false); // Отключаем ускорение.
	}
}

// Приседание юнита.
void AFPSPlayerController::CallCrouch()
{
	// Каст на солдата.
	AFPSSoldier* _soldierPlayer = Cast<AFPSSoldier>(_player);

	if (_soldierPlayer)  // Если каст был успешный (мы играем за солдата).
	{
		const bool bCrouch = 
			_soldierPlayer->IsCrouchSoldier(); // Записывае приседание в данный момент.

		if (!bCrouch) { // Проверяем приседаем ли мы в данный момент.
			CallSprintOff(); // Отключаем спринт.
			_soldierPlayer->SetCrouch(true); // Если нет, то включаем приседание.
		}
		else {
			_soldierPlayer->SetCrouch(false); // Если уже сидим, то отключаем приседание.
		}
	}
}

// Включить стрельбу из главного орудия.
void AFPSPlayerController::CallFireMainOn()
{
	if (_player) {
		_player->OnFireMainUnit(); // Вызываем стрельбу.
	}
}

// Отключаем стрельбу из главного орудия.
void AFPSPlayerController::CallFireMainOff()
{
	if (_player) {
		_player->OnStopFire(); // Останавливаем стрельбу.
	}
}

// Включаем перезарядку юниту.
void AFPSPlayerController::CallReload()
{
	if (_player) {
		_player->OnReloadUnit();
	}
}

// Целимся.
void AFPSPlayerController::CallAimOn()
{
	if (_player) {
		_player->OnAimUnit(true);
	}
}

// Отключаем прицелевание.
void AFPSPlayerController::CallAimOff()
{
	if (_player) {
		_player->OnAimUnit(false);
	}
}

// Взаимодействуем с предметом.
void AFPSPlayerController::CallIteraction()
{
	// Ссылка на интерфейс, проверяем реализует ли ее игрок.
	IFPSInterfaceControll* InterfaceControll = 
		Cast<IFPSInterfaceControll>(GetPawn());

	// Проверяем ссылку.
	if (InterfaceControll) 
	{ 
		// Вызываем спавн трейса для подбора предметов.
		InterfaceControll->
			Execute_OnInteractionTrace(GetPawn());
	}
}