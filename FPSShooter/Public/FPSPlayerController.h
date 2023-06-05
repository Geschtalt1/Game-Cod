
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FPSPlayerController.generated.h"

class AFPSUnitsBase;

UCLASS()
class FPSSHOOTER_API AFPSPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	// �����������. 
	AFPSPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// ����������.
	virtual void SetupInputComponent();

	// �������� ������ - �����.
	virtual void CallMoveForward(float Value);

	// �������� ����� - ������.
	virtual void CallMoveRight(float Value);

	// ������� ������ �� �����������.
	virtual void CallTurn(float Value);

	// ������� ������ �� ���������.
	virtual void CallLookUp(float Value);

	// ������ �������.
	virtual void CallSprintOn();

	// ������ ��������.
	virtual void CallSprintOff();

	// ����������.
	virtual void CallCrouch();

	// �������� �������� �� �������� ������ �����.
	virtual void CallFireMainOn();

	// �������� �� �������� ������ ���������.
	virtual void CallFireMainOff();

	// ��������� ����������� ������.
	virtual void CallReload();

	// �������������.
	virtual void CallAimOn();

	// ��������� ������������.
	virtual void CallAimOff();

	// ��������������� � ���������.
	virtual void CallIteraction();

	// �������� ����� 

	// Setters.


	// Getters.

	// Varieble.

protected:
	// ������ ����
	virtual void BeginPlay() override;

	// Setters.
	

	// Getters.
	
	
	// Varieble.

private:
	// Varieble.

	// ������ �� ��������� ������������.
	class AFPSUnitsBase* _player;
};
