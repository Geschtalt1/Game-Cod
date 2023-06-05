
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
	// Конструктор. 
	AFPSPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Управление.
	virtual void SetupInputComponent();

	// Движение вперед - назад.
	virtual void CallMoveForward(float Value);

	// Движение влево - вправо.
	virtual void CallMoveRight(float Value);

	// Поворот камеры по горизонтали.
	virtual void CallTurn(float Value);

	// Поворот камеры по вертикали.
	virtual void CallLookUp(float Value);

	// Спринт включен.
	virtual void CallSprintOn();

	// Спринт выключен.
	virtual void CallSprintOff();

	// Приседание.
	virtual void CallCrouch();

	// Включаем стрельбу из главного орудия юнита.
	virtual void CallFireMainOn();

	// Стрельба из главного орудия отключена.
	virtual void CallFireMainOff();

	// Запускаем перезарядку оружия.
	virtual void CallReload();

	// Прицеливаемся.
	virtual void CallAimOn();

	// Отключаем прицеливание.
	virtual void CallAimOff();

	// Взаимодействуем с предметом.
	virtual void CallIteraction();

	// Выбираем режим 

	// Setters.


	// Getters.

	// Varieble.

protected:
	// Начало игры
	virtual void BeginPlay() override;

	// Setters.
	

	// Getters.
	
	
	// Varieble.

private:
	// Varieble.

	// Ссылка на владельца контроллером.
	class AFPSUnitsBase* _player;
};
