
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

// ����������� �����������.
AFPSPlayerController::AFPSPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	_player = nullptr;

}

// ����������.
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

// ������ ����.
void AFPSPlayerController::BeginPlay()
{
	if (IsLocalPlayerController()) {
		_player = Cast<AFPSUnitsBase>(GetPawn());; // ������ �� ��������������� ������.
	}
}

// �������� ������ - �����.
void AFPSPlayerController::CallMoveForward(float Value)
{
	if (_player) {
		_player->OnMoveForward(Value);
	}
}

// �������� ����� - ������.
void AFPSPlayerController::CallMoveRight(float Value)
{
	if (_player) {
		_player->OnMoveRight(Value);
	}
}

// ������� ������ �� �����������.
void AFPSPlayerController::CallTurn(float Value)
{
	if (_player) {
		_player->AddControllerYawInput(Value);
	}
}

// ������� ������ �� ���������.
void AFPSPlayerController::CallLookUp(float Value)
{
	if (_player) {
		_player->AddControllerPitchInput(Value);
	}
}

// ������ �������.
void AFPSPlayerController::CallSprintOn()
{
	if (_player) // ���� ����� ������������.
	{
		if (!_player->IsRunUnit()) { // � ���� �� � ������ ������ �� �������.
			_player->SetSpeedRun(true); // �������� ���������.
		}
	}
}

// ������ ��������.
void AFPSPlayerController::CallSprintOff()
{
	if (_player)  // ���� ����� ������������.
	{ 
		_player->SetSpeedRun(false); // ��������� ���������.
	}
}

// ���������� �����.
void AFPSPlayerController::CallCrouch()
{
	// ���� �� �������.
	AFPSSoldier* _soldierPlayer = Cast<AFPSSoldier>(_player);

	if (_soldierPlayer)  // ���� ���� ��� �������� (�� ������ �� �������).
	{
		const bool bCrouch = 
			_soldierPlayer->IsCrouchSoldier(); // ��������� ���������� � ������ ������.

		if (!bCrouch) { // ��������� ��������� �� �� � ������ ������.
			CallSprintOff(); // ��������� ������.
			_soldierPlayer->SetCrouch(true); // ���� ���, �� �������� ����������.
		}
		else {
			_soldierPlayer->SetCrouch(false); // ���� ��� �����, �� ��������� ����������.
		}
	}
}

// �������� �������� �� �������� ������.
void AFPSPlayerController::CallFireMainOn()
{
	if (_player) {
		_player->OnFireMainUnit(); // �������� ��������.
	}
}

// ��������� �������� �� �������� ������.
void AFPSPlayerController::CallFireMainOff()
{
	if (_player) {
		_player->OnStopFire(); // ������������� ��������.
	}
}

// �������� ����������� �����.
void AFPSPlayerController::CallReload()
{
	if (_player) {
		_player->OnReloadUnit();
	}
}

// �������.
void AFPSPlayerController::CallAimOn()
{
	if (_player) {
		_player->OnAimUnit(true);
	}
}

// ��������� ������������.
void AFPSPlayerController::CallAimOff()
{
	if (_player) {
		_player->OnAimUnit(false);
	}
}

// ��������������� � ���������.
void AFPSPlayerController::CallIteraction()
{
	// ������ �� ���������, ��������� ��������� �� �� �����.
	IFPSInterfaceControll* InterfaceControll = 
		Cast<IFPSInterfaceControll>(GetPawn());

	// ��������� ������.
	if (InterfaceControll) 
	{ 
		// �������� ����� ������ ��� ������� ���������.
		InterfaceControll->
			Execute_OnInteractionTrace(GetPawn());
	}
}