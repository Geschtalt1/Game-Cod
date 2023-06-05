
#include "Classes/UnitsChild/FPSSoldier.h"
#include "Classes/WeaponChild/FPSFireArms.h"
#include "Classes/FPSWeaponBase.h"

#include "FPSHUD.h"
#include "Other/FPSTypename.h"

#include "TimerManager.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "Components/CapsuleComponent.h"
#include "Component/FPSStaminaComponent.h"
#include "Component/FPSMovementComponent.h"
#include "Component/FPSEquipmentComponent.h"
#include "Components/ChildActorComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "GameFramework/SpringArmComponent.h"

// ����������� �������.
AFPSSoldier::AFPSSoldier(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bUseControllerRotationYaw = false;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// ��������� ��������.
	Collision = CreateDefaultSubobject<UCapsuleComponent>("Soldier Capsull Collision");
	Collision->SetCollisionObjectType(CHANNELS_CAPSULE);
	Collision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	Collision->SetCollisionResponseToChannel(CHANNELS_ITEM_COLLIDER, ECollisionResponse::ECR_Ignore);
	Collision->SetCollisionResponseToChannel(CHANNELS_BULLET_LINE, ECollisionResponse::ECR_Ignore);
	Collision->SetCollisionResponseToChannel(CHANNELS_ITEM_LINE, ECollisionResponse::ECR_Ignore);
	Collision->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Ignore);
	Collision->SetCollisionResponseToChannel(ECC_Camera, ECollisionResponse::ECR_Ignore);
	Collision->SetCapsuleSize(30.0f, 92.0f);
	SetRootComponent(Collision);

	// ��������� �������.
	Skeletal = CreateDefaultSubobject<USkeletalMeshComponent>("Skeletal Mesh Component");
	Skeletal->SetupAttachment(Collision);
	Skeletal->SetWorldLocation(FVector{ 0.0f, 0.0f, -96.0f });
	Skeletal->SetWorldRotation(FRotator{ 0.0f, -100.0f, 0.0f });
	Skeletal->SetCollisionObjectType(CHANNELS_SOLDIER);
	Skeletal->SetGenerateOverlapEvents(true);

	// �������� ��������� ������.
	WeaponChildComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("Weapon Component"));
	WeaponChildComponent->SetupAttachment(Skeletal, FName(TEXT("weapon_r")));
	WeaponChildComponent->SetRelativeLocation(FVector(0.791988f, 5.657968f, -3.617635f));
	WeaponChildComponent->SetRelativeRotation(FRotator(-13.0f, 190.0f, 0.0f));

	// ������ ���.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Component");
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SetWorldLocation(FVector{ 0.0f, 0.0f, 40.0f });
	SpringArm->SocketOffset.Set(0.0f,0.0f, 25.0f);
	SpringArm->TargetArmLength = 200.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 15.0f;
	SpringArm->SetupAttachment(Collision);
	
	// ��������� ������.
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera Component");
	Camera->SetWorldLocation(FVector(0.0f, 60.0f, 0.0f));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = true;

	ItemCollision = CreateDefaultSubobject<UCapsuleComponent>("Item Capsull Collision");
	ItemCollision->SetCollisionObjectType(CHANNELS_ITEM_COLLIDER);
	ItemCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ItemCollision->SetCollisionResponseToChannel(CHANNELS_ITEM, ECollisionResponse::ECR_Overlap);
	ItemCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ItemCollision->SetCapsuleSize(10.0f, 200.0f);
	ItemCollision->SetupAttachment(Camera);
	ItemCollision->SetRelativeLocation(FVector(190.0f, 0.0f, 0.0f));
	ItemCollision->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	ItemCollision->SetVisibility(false);

	// ��������� �������.
	Stamina = CreateDefaultSubobject<UFPSStaminaComponent>(TEXT("Stamina System"));

	// ��������� ��������.
	Movement = CreateDefaultSubobject<UFPSMovementComponent>(TEXT("Movement System"));

	// ��������� ����������.
	Equipment = CreateDefaultSubobject<UFPSEquipmentComponent>(TEXT("Inventory System"));

	_recoilWhenShooting = nullptr;
	_weaponSoldier = nullptr;
	_curveAim = nullptr;

	_blockAlpha = 0.0f;

	_radiusBlock = 10.0f;
	_lenghtTrace = 100.0f;
	_highTrace = 10.0f;

	ItemCollision->OnComponentBeginOverlap.AddDynamic(this, &AFPSSoldier::OnOverlapBegin);
	ItemCollision->OnComponentEndOverlap.AddDynamic(this, &AFPSSoldier::OnOverlapEnd);

}

// ��� ����������� �����.
void AFPSSoldier::PostInitProperties()
{
	Super::PostInitProperties();

	if (_curveAim)
	{
		FOnTimelineFloat _targetLengt;
		_targetLengt.BindUFunction(this, FName("OffsetTargetLenght"));
		_aimTiemLine.AddInterpFloat(_curveAim, _targetLengt);
	}

	if (_curveRecoil)
	{
		FOnTimelineFloat _targetRecoil;
		_targetRecoil.BindUFunction(this, FName("OffsetAimToFire"));
		_recoilTiemLine.AddInterpFloat(_curveRecoil, _targetRecoil);
	}
}

// ������ ����.
void AFPSSoldier::BeginPlay()
{
	Super::BeginPlay();

	// �������� �������� ���� ���������.
	ItemCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	// ����� ������ �� ��-��� ��������� � �������� ���������.
	AActor& _childActor =
		*GetChildWeapon()->GetChildActor();

	// ���������� ������ � ������.
	_weaponSoldier = Cast<AFPSWeaponBase>(&_childActor);

	_weaponSoldier->SetWeaponOwner(this);

	// ��������� ������ �������� ��� ����� ����� �������.
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, 
		this, 
		&AFPSSoldier::BlockWeaponWall, 
		0.05f, 
		true);
}

// ����� ���.
void AFPSSoldier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	_aimTiemLine.TickTimeline(DeltaTime);
	_recoilTiemLine.TickTimeline(DeltaTime);
}

// ����� ������ ���������� Item �������.
void AFPSSoldier::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Overlap Begin"));
		
		// ��������� ��������� �� ����� ���������.
		IFPSInterfaceEquipment* Interface = 
			Cast<IFPSInterfaceEquipment>(OtherActor);

	/*if (OtherActor->GetClass()->ImplementsInterface(UFPSInterfaceEquipment::StaticClass()))
	{
			IFPSInterfaceEquipment::Execute_OnBeginLookItem(this, this);
	}*/

		// ���� ���������, ���������� ������� ����� �������� �� �������.
		if (Interface) { Interface->Execute_OnBeginLookItem(OtherActor, this);}
	}
}

// ����� ����� ���������� Item �������.
void AFPSSoldier::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Overlap End"));

		// ��������� ��������� �� ����� ���������.
		IFPSInterfaceEquipment* Interface =
			Cast<IFPSInterfaceEquipment>(OtherActor);

		// ���� ���������, ���������� ������� �������� �������� �� �������.
		if (Interface) { Interface->Execute_OnEndLookItem(OtherActor, this); }
	}
}

// ����� �������������� � ���������.
void AFPSSoldier::OnInteractionTrace_Implementation()
{	
	// ������ �������.
	const float _halfHeight =
		ItemCollision->GetScaledCapsuleHalfHeight();

	// ������ �������.
	const float _radius =
		ItemCollision->GetScaledCapsuleRadius();

	// ���� ������.
	const FVector _startTrace = GetCamera()->
		GetComponentLocation();

	// ����� ������.
	const FVector _endTrace = GetCamera()->
		GetForwardVector() * (_halfHeight * 1.9f) + _startTrace;

	// ��������� ���������.
	FHitResult _hitResult;

	// ������� ����� �����.
	const bool bHit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(),
		_startTrace, // ������ ������.
		_endTrace, // ����� ������.
		_radius, // ������ �������.
		TraceTypeQuery4, // �������� �������.
		false, // ...
		{ this }, // ����� ���������� ���� �����.
		EDrawDebugTrace::None, // �����������.
		_hitResult, // ��������� ��������� ������.
		true // ...
	);

	// ���� ��������� �� ����, ������� �����������.
	if (!bHit) { return; }

	// ��������� ���������� ���������� Equipment � ������ ������.
	IFPSInterfaceEquipment* Interface =
		Cast<IFPSInterfaceEquipment>(_hitResult.GetActor());

	// ���� ��������� ����.
	if (Interface) 
	{ 
		// �������� ������� ������ ��������.
		Interface->Execute_OnTakeItem(
			_hitResult.GetActor(), // ����� � ������� ���������� �������.
			this // �������� ������ �� ���� ��� ���������.
		);
	}

}

// �������� � ��������� ������������.
void AFPSSoldier::OnAimUnit(bool bActive)
{
	// ���� ������������ ���������.
	if (bActive) 
	{
		_aimTiemLine.Play(); // ������ ������ ��������� � ����� �����.
		_bAim = true; // ������������ ��������.
	}
	else // ���� ������������ ����������.
	{
		_aimTiemLine.Reverse(); // ������ ������ �������� �� ����� �����.
		_bAim = false; // ������������ ���������.
	}
}

// ���� �������� �� �������� ������.
void AFPSSoldier::OnFireMainUnit()
{
	// ���� ������ ������� � �������� ��������� � ���� �� ����� � �� ��������������.
	if (GetWeapon() && GetFireEnable() && !IsRunUnit() && !IsReloadUnit())
	{
		GetWeapon()->OnFire(); // �������� ������� ��������.
		PlayAnimationMontage(_recoilWhenShooting, true); // ����������� ������ ������.

		//////////////////////////////
	}
}

// ���� ������������ ������.
void AFPSSoldier::OnReloadUnit()
{
	// ��������� ��� ������ ���������� � ���� �� ��������������.
	if (GetWeapon() && !IsReloadUnit()) {
		GetWeapon()->OnReload(); // ��������� ������� ����������� � ������.
	}
}

// ���� ��������� ������ ��� �����.
void AFPSSoldier::OnMoveForward(float Value)
{
	const FRotator _playerRotation = { 0.0f, GetActorRotation().Yaw, 0.0f }; // ����� �������� ������ �� Z.
	const FVector _playerForward = UKismetMathLibrary::GetForwardVector(_playerRotation); // 

	AddMovementInput(_playerForward, Value); // ��������� �������� ������-�����.
}

// ���� ��������� ������ ��� �����.
void AFPSSoldier::OnMoveRight(float Value)
{
	const FRotator _playerRotation = { 0.0f, GetActorRotation().Yaw, 0.0f }; // ����� �������� ������ �� Z.
	const FVector _playerRight = UKismetMathLibrary::GetRightVector(_playerRotation); // 

	AddMovementInput(_playerRight, Value); // ��������� �������� �����-������.
}

// ���������� ��������.
void AFPSSoldier::OnStopFire()
{
	// ���� �� ������������� ������.
	AFPSFireArms* _fireArms = Cast<AFPSFireArms>(GetWeapon());

	// ���� ���� ��������.
	if (_fireArms)
	{
		// ��������� ��������.
		_fireArms->StopFire();
	}
}

// ������� ������������ ������.
void AFPSSoldier::SetCameraPosition(TEnumAsByte<ECameraPos> Position)
{
	FVector LocationCam = GetCamera()->GetRelativeLocation();

	if (Position == POS_FirstPerson)
	{
		// ��������.
	}
	else if (Position == POS_LeftRight) // ������ ������������ �� ����� ��� ������ ������� �����.
	{
		GetCamera()->SetRelativeLocation(LocationCam * (-1.0f));
	}
	else // ������ ������������ �� ����� �����.
	{
		GetCamera()->SetRelativeLocation(FVector{ 0.0f });
	}
}

// ���������� ������������ �������� ������������.
void AFPSSoldier::SetSpeedMax(float NewSpeed)
{
	GetMovement()->MaxSpeed = abs(NewSpeed);
}

// �������� ��� ��������� ��������� �����.
void AFPSSoldier::SetSpeedRun(bool bSprint)
{
	//const float _speedMax = GetMovement()->MaxSpeed;
	const float _speedRun = GetMovement()->GetSpeedRun();

	if (!IsCrouchSoldier()) // ���� ���� �� ��������� � �������.
	{
		if (bSprint) // ���� ������ �������. 
		{
			// ����������� �������� ������������.
			SetSpeedMax(_speedRun);

			// ��������� �������� ���������� �������.
			GetStamina()->PlayStaminaChange(ST_Reduction);
		}
		else // ���� ������ ��������.
		{
			// ���������� ����������� �������� ����.
			const float _speedWalk = GetMovement()->GetSpeedWalk();

			// ��������� ����������� �������� ������.
			SetSpeedMax(_speedWalk);

			// ��������� ������� �������������� �������.
			GetStamina()->PlayStaminaChange(ST_Regeneration);
		}
	}
}

// ���������� ����������.
bool AFPSSoldier::SetCrouch(bool bCrouch)
{
	if (bCrouch) // ���� ���������� ��������.
	{
		Collision->SetCapsuleSize(30.0f, 94.0f); // ������ �������� �����������. (55)
		GetMovement()->NavAgentProps.bCanCrouch = true;

		const float _speedDecrease = 
			GetMovement()->GetSpeedCrouch(); // ��������� �������� .

		SetSpeedMax(_speedDecrease); // ��������� �������� ������.
	}
	else // ���� ���������� ���������.
	{
		Collision->SetCapsuleSize(30.0f, 94.0f); // ������ �������� �������������.
		GetMovement()->NavAgentProps.bCanCrouch = false;

		const float _speedIncrease = 
			GetMovement()->GetSpeedWalk();

		SetSpeedMax(_speedIncrease); // ��������� ������� ����������� �������� ������.
	}

	return bCrouch;
}

// ��������� ���������� ������� � ������ ������.
bool AFPSSoldier::IsCrouchSoldier() const
{
	return GetMovement()->IsCrouching();
}

// ��������� ������������� �� ���� �� �������.
bool AFPSSoldier::IsRunUnit() const
{	
	return GetMovement()->IsRunning();
}

// ��������� �������������� �� ���� � ������ ������.
bool AFPSSoldier::IsReloadUnit() const
{
	// ���� �� ��� �������������� ������.
	const AFPSFireArms* _fireArms =
		Cast<AFPSFireArms>(_weaponSoldier);

	// ���� ���� ��������.
	if (_fireArms)
	{
		// ��������� �����������.
		return _fireArms->IsReload();
	}

	// ����������� �� ��������� false.
	return false;
}

// ����������� ������������ ������ �������
float AFPSSoldier::PlayAnimationMontage(UAnimMontage* PlayMontage, bool bStopAllMontage)
{
	if (PlayMontage)
	{
		// ������ �� ���� �������.
		UAnimInstance* _animInst = 
			GetSkeletal()->GetAnimInstance();

		if (_animInst) // ��������� ��� ������ ����.
		{
			// ���������� ������ �������.
			const float _lenghtAnim = _animInst->Montage_Play(PlayMontage,
				1.0f,
				EMontagePlayReturnType::MontageLength,
				0.f,
				bStopAllMontage);

			// ��������� ����� ������������ �������.
			return _lenghtAnim;
		}
	}

	return 0.0f;
}

// ������ ������ TargetLenhgt � SpringArm, ��� ������������.
void AFPSSoldier::OffsetTargetLenght(float Value)
{
	// �������� ����� �������� ����� ����.
	float _newLenght = UKismetMathLibrary::Lerp(
		200.0f,
		100.0f,
		Value
	);

	// �������� ����� �������� FOV.
	float _newFOV = UKismetMathLibrary::Lerp(
		90.0f,
		75.0f,
		Value
	);

	// ������������ ������.
	const FVector A = { 0.0f, 0.0f, 25.0f };
	const FVector B = { 0.0f, -15.0f, 25.0f };

	// ������������� ������������ ������ � �����.
	FVector _newSocketOffset = UKismetMathLibrary::VLerp(
		A,
		B,
		Value
	);

	// ������������� ����� ����� ���� � ������.
	SpringArm->TargetArmLength = _newLenght;

	// ������������� ����� ���� ������.
	Camera->FieldOfView = _newFOV;

	// ������������� ����� ������������ ������.
	SpringArm->SocketOffset = _newSocketOffset;
}

// ������ ��� ��������, �������� ������ �����.
void AFPSSoldier::OffsetAimToFire(float Value)
{
	// �������� ������� ������ � ���� ����������.
	AFPSFireArms* _weaponFire =
		Cast<AFPSFireArms>(GetWeapon());

	// ������ �������.
	if (_weaponFire)
	{
		// ���������.
		float _recoilAmp = 0.2;

		// ����� ��������� � ����������� �����.
		const float _deltaSecond = 
			UGameplayStatics::GetWorldDeltaSeconds(GetWorld());

		// ���������� ��������� ������.
		const float _recoilWeapon = _weaponFire->GetRecoil();

		// ������������ ������, � ������� ������� ������� ��������.
		const float _newRecoil = _weaponFire->CalculateSpread(_recoilWeapon);

		// ������������ ������������ �������.
		const float _addPitch = (Value * _recoilAmp) * _newRecoil * _deltaSecond;

		// ������ ���������� �����.
		AddControllerPitchInput(_addPitch);

	}
}

// ����������� ����� ��� ������� � �������.
void AFPSSoldier::BlockWeaponWall()
{
	// ����������� ������ ������.
	const FVector _startTrace = 
		GetSkeletal()->GetSocketLocation(FName("head")) + FVector(
			0.0,       // X
			0.0f,      // Y
			_highTrace  // Z
		);

	// ��������� ������� ������.
	const FRotator _contrRotat = GetControlRotation();

	// ������� ������.
	const float _angle = FMath::ClampAngle(_contrRotat.Pitch,
		-90.0f, // ������������ �������� �� ������� ����� ����������� ����.
		90.0    // ������������ �������� �� ������� ����� ���������� �����.
	);

	// ����������� ����������� ������.
	const FVector _forwardVec = UKismetMathLibrary::GetForwardVector(FRotator(
		_angle, 
		_contrRotat.Yaw,
		_contrRotat.Roll)
	) * _lenghtTrace; // ����� ������.

	// ����������� ����� ������.
	const FVector _endTrace = _startTrace + _forwardVec;

	// ��������� ������.
	FHitResult _hitResult;

	// ����� �����.
	const bool bHit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(),
		_startTrace, // ������ ������.
		_endTrace, // ����� ������.
		_radiusBlock, // ������ ������.
		_traceBlock, // ������� ������.
		false, //
		{this}, // ����� ���������� ���� �������.
		_drawDebugBlock, // ��� ����������� ������.
		_hitResult, // ��������� ��������� ������.
		true // ���������� ����.
	);

	// ����������� ��������� �� ������,
	// ��� �� ������������ �� ������� ������ ��� �� ����������.
	const float _distanceVec = UKismetMathLibrary::Vector_Distance(
		_hitResult.Location, // ����� ��� ������ ������������.
		_startTrace // ������ �������.
	);

	const float _lerp = UKismetMathLibrary::Lerp(
		_blockAlpha,
		abs(_distanceVec / _lenghtTrace - 1.0f) * 2.0f,
		0.25f
	);

	// �������� ��������.
	const float _selectFloat = UKismetMathLibrary::SelectFloat(
		_lerp, // ����������� �������� ��������� ������.
		0.0f, // ����� �������� ������������� �� �����.
		bHit // ���� ����� ����������.
	);

	// ������ ������.
	const float _deltaSecond = 
		UGameplayStatics::GetWorldDeltaSeconds(GetWorld());

	// ������ ������ ��������.
	_blockAlpha = UKismetMathLibrary::FInterpTo(
		_blockAlpha, // ������� ��������.
		_selectFloat, // �������� ������� �������� ��� ������������.
		_deltaSecond, // ����� � ����� ����.
		25.0f // ���������.
	);

	// ���� ����� ������.
	if (_blockAlpha >= 0.15f)
	{
		// ��������� ��������.
		SetFireEnable(false);
	}
	else // ���� ������.
	{
		// ��������� ��������.
		SetFireEnable(true);
	}
}