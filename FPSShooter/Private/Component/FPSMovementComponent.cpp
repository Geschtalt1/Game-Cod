
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

// ��� ����������� ����������.
void UFPSMovementComponent::PostInitProperties()
{
	Super::PostInitProperties();

}

// ������ ����.
void UFPSMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// ���������� � ��������� ������������, ������ �� ��������� ����� ����������.
	_soldierRef = Cast<AFPSSoldier>(GetOwner());

	SetSpeedSoldier(_speedWalk);

	// ...
	
}


// ���.
void UFPSMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateGravity(DeltaTime);

	UpdateTurn(DeltaTime, InterpSpeed);

	// ...
}

// ���������� ���������� �����.
void UFPSMovementComponent::UpdateGravity(float DeltaTime)
{
	// �������� ���������� ������ � ��������� ����������.
	if (_soldierRef && bGravity)
	{
		// ���������� ������ �� ������� �����.
		const class UCapsuleComponent& _capsuleSoldier = *_soldierRef->Collision;
		
		// ���������� ������ ������� � ������� �������� �����.
		const float _capsuleHeight = _capsuleSoldier.GetScaledCapsuleHalfHeight();
		const float _speedSoldier = _soldierRef->GetSpeedUnit();

		// ������������ � ����� ��������� ����� ������ ����.
		const FVector GravityAcc = FVector(0.0f, 0.0f, -1.0f) * GravityScale;

		// �������������� �������� ������� � DeltaTime.
		const FVector OffsetLocation = FVector(1.0f * 
			DeltaTime, 0.0f, 0.0f)  + GravityAcc;

		// ������ � ����� ����.
		FVector Start = _capsuleSoldier.GetComponentLocation();
		FVector End = Start - (_capsuleSoldier.GetUpVector() * _capsuleHeight);

		// ��������� ��������� ����.
		FHitResult Hit;
		FCollisionQueryParams ParamsCollision;

		// ������� ���.
		bool bHit = GetWorld()->LineTraceSingleByChannel(
			Hit,   // ���� ���������� ���� �� �����.
			Start, // ����� ���������� �� ���� ��� ����������.
			End,   // ��� ���������� ��� ��� �����������. 
			ECC_Visibility, // ��� �������� ����.
			ParamsCollision // ��������� ��������.
		);

		// ���� ��� �� �������� �� �����.
		if (!bHit)
		{
			// ���� ����� ������.
			_soldierRef->AddActorLocalOffset(OffsetLocation, true);

			//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0.0f);
		}
	}
}

// ���������� �������� �����.
void UFPSMovementComponent::UpdateTurn(float DeltaTime, const float SpeedTurn)
{
	if (_soldierRef && bTurn)
	{
		// ������� ��������.
		const FRotator _currentRotation = _soldierRef->GetActorRotation();
		const FRotator _aimRotation = _soldierRef->GetBaseAimRotation();

		const float _aimYaw = _aimRotation.Yaw;

		FRotator _newRotation = UKismetMathLibrary::RInterpTo(
			_currentRotation, // ������� �������. 
			FRotator(0.0f, _aimYaw, 0.0f), // ������� �� ����������� ���� ������.
			DeltaTime, // ����� �� ������� �������� ��������� ����.
			SpeedTurn); // �������� ��������.

		// ������������� ����� ��������.
		_soldierRef->SetActorRotation(_newRotation);
	}
}

// ��������� ��������� �� ����.
bool UFPSMovementComponent::IsCrouching() const
{
	return NavAgentProps.bCanCrouch;
}

// ��������� ������������� �� ���� �� ����� � ������ ������.
bool UFPSMovementComponent::IsMovingOnGround() const
{
	return _soldierRef->GetSpeedUnit() > 0.0f;
}

// ��������� ����� �� ���� � ������ ������.
bool UFPSMovementComponent::IsRunning() const
{
	const float _walkSpeed = GetSpeedWalk();
	const float _currentSpeed = _soldierRef->GetSpeedUnit();

	return _currentSpeed > _walkSpeed;
}

// ���������� ������� �������� ������.
float UFPSMovementComponent::SetSpeedSoldier(float NewSpeed)
{
	MaxSpeed = abs(NewSpeed);
	// FMath::Clamp(NewSpeedWalk,100.0f, 300.0f);

	return MaxSpeed;
}
