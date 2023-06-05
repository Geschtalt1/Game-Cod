
#include "Component/FPSMovementComponent.h"

#include "Classes/UnitsChild/FPSSoldier.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "Components/CapsuleComponent.h"

UFPSMovementComponent::UFPSMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;

	MaxSpeed = 240.0f;
	Acceleration = 6000.0f;
	Deceleration = 8000.0f;
	TurningBoost = 8.0f;
	GravityScale = 9.8f;
	InterpSpeed = 15.0f;

	_speedWalk = 240.0f;
	_speedRun = 480.0f;
	_speedCrouch = 160.0f;

	bGravity = true;
	bTurn = true;
	_bIsMoving = false;
	_bIsRunning = false;

	_soldierRef = nullptr;
	// ...
}

// Пре инцилизация компонента.
void UFPSMovementComponent::PostInitProperties()
{
	Super::PostInitProperties();

}

// Начало игры.
void UFPSMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// Записываем в компонент передвижения, ссылку на владельца этого компонента.
	_soldierRef = Cast<AFPSSoldier>(GetOwner());

	SetSpeedSoldier(_speedWalk);

	// ...
	
}


// Тик.
void UFPSMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateGravity(DeltaTime);

	UpdateTurn(DeltaTime, InterpSpeed);

	// ...
}

// Обновление гравитации юнита.
void UFPSMovementComponent::UpdateGravity(float DeltaTime)
{
	// Проверка валидности игрока и включеной гравитации.
	if (_soldierRef && bGravity)
	{
		// Записываем ссылку на капсулу юнита.
		const class UCapsuleComponent& _capsuleSoldier = *_soldierRef->Collision;
		
		// Записываем высоту капсулы и текущую скорость юнита.
		const float _capsuleHeight = _capsuleSoldier.GetScaledCapsuleHalfHeight();
		const float _speedSoldier = _soldierRef->GetSpeedUnit();

		// Рассчитываем с какой скоростью будет падать юнит.
		const FVector GravityAcc = FVector(0.0f, 0.0f, -1.0f) * GravityScale;

		// Синхронизируем скорость падения с DeltaTime.
		const FVector OffsetLocation = FVector(1.0f * 
			DeltaTime, 0.0f, 0.0f)  + GravityAcc;

		// Начало и конец луча.
		FVector Start = _capsuleSoldier.GetComponentLocation();
		FVector End = Start - (_capsuleSoldier.GetUpVector() * _capsuleHeight);

		// Результат попадания луча.
		FHitResult Hit;
		FCollisionQueryParams ParamsCollision;

		// Создаем луч.
		bool bHit = GetWorld()->LineTraceSingleByChannel(
			Hit,   // Сюда записываем куда он попал.
			Start, // Здесь координаты от куда луч начинается.
			End,   // Тут координаты где луч закончиться. 
			ECC_Visibility, // Это коллизия луча.
			ParamsCollision // Параметры коллизии.
		);

		// Если луч не попадает по земле.
		if (!bHit)
		{
			// Юнит будет падать.
			_soldierRef->AddActorLocalOffset(OffsetLocation, true);

			//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0.0f);
		}
	}
}

// Обновление поворота юнита.
void UFPSMovementComponent::UpdateTurn(float DeltaTime, const float SpeedTurn)
{
	if (_soldierRef && bTurn)
	{
		// Текущее вращение.
		const FRotator _currentRotation = _soldierRef->GetActorRotation();
		const FRotator _aimRotation = _soldierRef->GetBaseAimRotation();

		const float _aimYaw = _aimRotation.Yaw;

		FRotator _newRotation = UKismetMathLibrary::RInterpTo(
			_currentRotation, // Текущий поворот. 
			FRotator(0.0f, _aimYaw, 0.0f), // Поворот по направлению глаз игрока.
			DeltaTime, // Время за которое сменился последний кадр.
			SpeedTurn); // Скорость вращения.

		// Устанавливаем новое вращение.
		_soldierRef->SetActorRotation(_newRotation);
	}
}

// Возращает присадает ли юнит.
bool UFPSMovementComponent::IsCrouching() const
{
	return NavAgentProps.bCanCrouch;
}

// Возращает передвигается ли юнит по земли в данный момент.
bool UFPSMovementComponent::IsMovingOnGround() const
{
	return _soldierRef->GetSpeedUnit() > 0.0f;
}

// Возращает бежит ли юнит в данный момент.
bool UFPSMovementComponent::IsRunning() const
{
	const float _walkSpeed = GetSpeedWalk();
	const float _currentSpeed = _soldierRef->GetSpeedUnit();

	return _currentSpeed > _walkSpeed;
}

// Установить базовую скорость хотьбы.
float UFPSMovementComponent::SetSpeedSoldier(float NewSpeed)
{
	MaxSpeed = abs(NewSpeed);
	// FMath::Clamp(NewSpeedWalk,100.0f, 300.0f);

	return MaxSpeed;
}
