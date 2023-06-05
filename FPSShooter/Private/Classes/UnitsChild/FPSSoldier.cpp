
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

// Конструктор солдата.
AFPSSoldier::AFPSSoldier(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bUseControllerRotationYaw = false;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// Компонент коллизии.
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

	// Компонент скелета.
	Skeletal = CreateDefaultSubobject<USkeletalMeshComponent>("Skeletal Mesh Component");
	Skeletal->SetupAttachment(Collision);
	Skeletal->SetWorldLocation(FVector{ 0.0f, 0.0f, -96.0f });
	Skeletal->SetWorldRotation(FRotator{ 0.0f, -100.0f, 0.0f });
	Skeletal->SetCollisionObjectType(CHANNELS_SOLDIER);
	Skeletal->SetGenerateOverlapEvents(true);

	// Дочерний компонент оружия.
	WeaponChildComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("Weapon Component"));
	WeaponChildComponent->SetupAttachment(Skeletal, FName(TEXT("weapon_r")));
	WeaponChildComponent->SetRelativeLocation(FVector(0.791988f, 5.657968f, -3.617635f));
	WeaponChildComponent->SetRelativeRotation(FRotator(-13.0f, 190.0f, 0.0f));

	// Спринг арм.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Component");
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SetWorldLocation(FVector{ 0.0f, 0.0f, 40.0f });
	SpringArm->SocketOffset.Set(0.0f,0.0f, 25.0f);
	SpringArm->TargetArmLength = 200.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 15.0f;
	SpringArm->SetupAttachment(Collision);
	
	// Компонент камеры.
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

	// Компонент стамины.
	Stamina = CreateDefaultSubobject<UFPSStaminaComponent>(TEXT("Stamina System"));

	// Компонент движения.
	Movement = CreateDefaultSubobject<UFPSMovementComponent>(TEXT("Movement System"));

	// Компонент экипировки.
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

// Пре инцилизация юнита.
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

// Начало игры.
void AFPSSoldier::BeginPlay()
{
	Super::BeginPlay();

	// Включаем коллизию йтем колайдера.
	ItemCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	// Берем ссылку на то-что находится в дочернем копоненте.
	AActor& _childActor =
		*GetChildWeapon()->GetChildActor();

	// Записываем ссылку в оружие.
	_weaponSoldier = Cast<AFPSWeaponBase>(&_childActor);

	_weaponSoldier->SetWeaponOwner(this);

	// Запускаем таймер проверки что перед лицом солдата.
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, 
		this, 
		&AFPSSoldier::BlockWeaponWall, 
		0.05f, 
		true);
}

// Евент тик.
void AFPSSoldier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	_aimTiemLine.TickTimeline(DeltaTime);
	_recoilTiemLine.TickTimeline(DeltaTime);
}

// Эвент начало перекрытия Item капсулы.
void AFPSSoldier::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Overlap Begin"));
		
		// Проверяем реализует ли Актор интерфейс.
		IFPSInterfaceEquipment* Interface = 
			Cast<IFPSInterfaceEquipment>(OtherActor);

	/*if (OtherActor->GetClass()->ImplementsInterface(UFPSInterfaceEquipment::StaticClass()))
	{
			IFPSInterfaceEquipment::Execute_OnBeginLookItem(this, this);
	}*/

		// Если реализует, включается функция начал смотреть на предмет.
		if (Interface) { Interface->Execute_OnBeginLookItem(OtherActor, this);}
	}
}

// Эвент конец перекрытия Item капсулы.
void AFPSSoldier::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Overlap End"));

		// Проверяем реализует ли Актор интерфейс.
		IFPSInterfaceEquipment* Interface =
			Cast<IFPSInterfaceEquipment>(OtherActor);

		// Если реализует, включается функция закончил смотреть на предмет.
		if (Interface) { Interface->Execute_OnEndLookItem(OtherActor, this); }
	}
}

// Евент взаимодействия с предметом.
void AFPSSoldier::OnInteractionTrace_Implementation()
{	
	// Размер капсулы.
	const float _halfHeight =
		ItemCollision->GetScaledCapsuleHalfHeight();

	// Радиус капсулы.
	const float _radius =
		ItemCollision->GetScaledCapsuleRadius();

	// Стар трейса.
	const FVector _startTrace = GetCamera()->
		GetComponentLocation();

	// Конец трейса.
	const FVector _endTrace = GetCamera()->
		GetForwardVector() * (_halfHeight * 1.9f) + _startTrace;

	// Результат попадания.
	FHitResult _hitResult;

	// Создаме трейс сферы.
	const bool bHit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(),
		_startTrace, // Начало трейса.
		_endTrace, // Конец трейса.
		_radius, // Радиус капсулы.
		TraceTypeQuery4, // Коллизия капсулы.
		false, // ...
		{ this }, // Трейс игнорирует этот класс.
		EDrawDebugTrace::None, // Отображение.
		_hitResult, // Результат попадания трейса.
		true // ...
	);

	// Если попадания не было, функция завершается.
	if (!bHit) { return; }

	// Проверяем реализацию интерфейса Equipment в другом классе.
	IFPSInterfaceEquipment* Interface =
		Cast<IFPSInterfaceEquipment>(_hitResult.GetActor());

	// Если интерфейс есть.
	if (Interface) 
	{ 
		// Вызываем функцию подбор предмета.
		Interface->Execute_OnTakeItem(
			_hitResult.GetActor(), // Класс в котором вызывается функция.
			this // Передаем ссылку на того кто подберает.
		);
	}

}

// Включает и отключает прицеливание.
void AFPSSoldier::OnAimUnit(bool bActive)
{
	// Если прицеливание включенно.
	if (bActive) 
	{
		_aimTiemLine.Play(); // Камера плавно подлетает к плечу юнита.
		_bAim = true; // Прицеливание включено.
	}
	else // Если прицеливание отключенно.
	{
		_aimTiemLine.Reverse(); // Камера плавно отлетает от плеча юнита.
		_bAim = false; // Прицеливание отключено.
	}
}

// Юнит стреляет из главного оружия.
void AFPSSoldier::OnFireMainUnit()
{
	// Если оружие валидно и стрельтя разрешено и юнит не бежит и не перезарежается.
	if (GetWeapon() && GetFireEnable() && !IsRunUnit() && !IsReloadUnit())
	{
		GetWeapon()->OnFire(); // Вызываем функцию стрельбы.
		PlayAnimationMontage(_recoilWhenShooting, true); // Проигрываем монтаж отдачи.

		//////////////////////////////
	}
}

// Юнит перезарежает оружие.
void AFPSSoldier::OnReloadUnit()
{
	// Проверяем что оружие существует и юнит не перезарежается.
	if (GetWeapon() && !IsReloadUnit()) {
		GetWeapon()->OnReload(); // Запускаем функцию перезарядки в оружие.
	}
}

// Юнит движеться вперед или назад.
void AFPSSoldier::OnMoveForward(float Value)
{
	const FRotator _playerRotation = { 0.0f, GetActorRotation().Yaw, 0.0f }; // Берем вращение игрока по Z.
	const FVector _playerForward = UKismetMathLibrary::GetForwardVector(_playerRotation); // 

	AddMovementInput(_playerForward, Value); // Добавляем движение вперед-назад.
}

// Юнит движеться вправо или влево.
void AFPSSoldier::OnMoveRight(float Value)
{
	const FRotator _playerRotation = { 0.0f, GetActorRotation().Yaw, 0.0f }; // Берем вращение игрока по Z.
	const FVector _playerRight = UKismetMathLibrary::GetRightVector(_playerRotation); // 

	AddMovementInput(_playerRight, Value); // Добавляем движение влево-вправо.
}

// Остановить стрельбу.
void AFPSSoldier::OnStopFire()
{
	// Каст на огнестрельное оружие.
	AFPSFireArms* _fireArms = Cast<AFPSFireArms>(GetWeapon());

	// Если каст успешный.
	if (_fireArms)
	{
		// Отключаем стрельбу.
		_fireArms->StopFire();
	}
}

// Выбрать расположение камеры.
void AFPSSoldier::SetCameraPosition(TEnumAsByte<ECameraPos> Position)
{
	FVector LocationCam = GetCamera()->GetRelativeLocation();

	if (Position == POS_FirstPerson)
	{
		// Дописать.
	}
	else if (Position == POS_LeftRight) // Камера перемещается на левую или правую сторону юнита.
	{
		GetCamera()->SetRelativeLocation(LocationCam * (-1.0f));
	}
	else // Камера перемещается на центр юнита.
	{
		GetCamera()->SetRelativeLocation(FVector{ 0.0f });
	}
}

// Установить максимальную скорость передвижения.
void AFPSSoldier::SetSpeedMax(float NewSpeed)
{
	GetMovement()->MaxSpeed = abs(NewSpeed);
}

// Включить или отключить ускорение юнита.
void AFPSSoldier::SetSpeedRun(bool bSprint)
{
	//const float _speedMax = GetMovement()->MaxSpeed;
	const float _speedRun = GetMovement()->GetSpeedRun();

	if (!IsCrouchSoldier()) // Если юнит не находится в присяди.
	{
		if (bSprint) // Если спринт включен. 
		{
			// Увеличиваем скорость передвижения.
			SetSpeedMax(_speedRun);

			// Запускаем таймлайн уменьшение стамины.
			GetStamina()->PlayStaminaChange(ST_Reduction);
		}
		else // Если спринт выключен.
		{
			// Записываем стандартную скорость бега.
			const float _speedWalk = GetMovement()->GetSpeedWalk();

			// Возращаем стандартную скорость хотьбы.
			SetSpeedMax(_speedWalk);

			// Запускаем таймлан восстановления стамины.
			GetStamina()->PlayStaminaChange(ST_Regeneration);
		}
	}
}

// Установить приседание.
bool AFPSSoldier::SetCrouch(bool bCrouch)
{
	if (bCrouch) // Если приседание включено.
	{
		Collision->SetCapsuleSize(30.0f, 94.0f); // Размер коллизии уменьшается. (55)
		GetMovement()->NavAgentProps.bCanCrouch = true;

		const float _speedDecrease = 
			GetMovement()->GetSpeedCrouch(); // Уменьшаем скорость .

		SetSpeedMax(_speedDecrease); // Уменьшаем скорость хотьбы.
	}
	else // Если приседание выключено.
	{
		Collision->SetCapsuleSize(30.0f, 94.0f); // Размер коллизии увеличивается.
		GetMovement()->NavAgentProps.bCanCrouch = false;

		const float _speedIncrease = 
			GetMovement()->GetSpeedWalk();

		SetSpeedMax(_speedIncrease); // Возращаем обратно стандартную скорость хотьбы.
	}

	return bCrouch;
}

// Возращает приседание солдата в данный момент.
bool AFPSSoldier::IsCrouchSoldier() const
{
	return GetMovement()->IsCrouching();
}

// Возращает передвигается ли юнит на спринте.
bool AFPSSoldier::IsRunUnit() const
{	
	return GetMovement()->IsRunning();
}

// Возращает перезарежается ли юнит в данный момент.
bool AFPSSoldier::IsReloadUnit() const
{
	// Каст на тип огнестрельного оружия.
	const AFPSFireArms* _fireArms =
		Cast<AFPSFireArms>(_weaponSoldier);

	// Если каст успешный.
	if (_fireArms)
	{
		// Возращаем перезарядку.
		return _fireArms->IsReload();
	}

	// Перезарядка по умолчанию false.
	return false;
}

// Проигрываем анимационный монтаж солдата
float AFPSSoldier::PlayAnimationMontage(UAnimMontage* PlayMontage, bool bStopAllMontage)
{
	if (PlayMontage)
	{
		// Ссылка на аним инстанс.
		UAnimInstance* _animInst = 
			GetSkeletal()->GetAnimInstance();

		if (_animInst) // Проверяем что ссылка есть.
		{
			// Прогрываем анимаю монтажа.
			const float _lenghtAnim = _animInst->Montage_Play(PlayMontage,
				1.0f,
				EMontagePlayReturnType::MontageLength,
				0.f,
				bStopAllMontage);

			// Возращаем время проигрывание монтажа.
			return _lenghtAnim;
		}
	}

	return 0.0f;
}

// Плавно меняет TargetLenhgt у SpringArm, при прицеливание.
void AFPSSoldier::OffsetTargetLenght(float Value)
{
	// Получаем новое значение длины руки.
	float _newLenght = UKismetMathLibrary::Lerp(
		200.0f,
		100.0f,
		Value
	);

	// Получаем новое значение FOV.
	float _newFOV = UKismetMathLibrary::Lerp(
		90.0f,
		75.0f,
		Value
	);

	// Расположение камеры.
	const FVector A = { 0.0f, 0.0f, 25.0f };
	const FVector B = { 0.0f, -15.0f, 25.0f };

	// Интерполируем расположение камеры к плечу.
	FVector _newSocketOffset = UKismetMathLibrary::VLerp(
		A,
		B,
		Value
	);

	// Устанавливаем новую длину руки у камеры.
	SpringArm->TargetArmLength = _newLenght;

	// Устанавливаем новое поле зрение.
	Camera->FieldOfView = _newFOV;

	// Устанавливаем новое расположение камеры.
	SpringArm->SocketOffset = _newSocketOffset;
}

// Отдача при стрельбе, задерает камеру вврех.
void AFPSSoldier::OffsetAimToFire(float Value)
{
	// Приводим текущее оружие к типу огнестрела.
	AFPSFireArms* _weaponFire =
		Cast<AFPSFireArms>(GetWeapon());

	// Оружие валидно.
	if (_weaponFire)
	{
		// Амплитуда.
		float _recoilAmp = 0.2;

		// Время прошедшее с предыдущего кадра.
		const float _deltaSecond = 
			UGameplayStatics::GetWorldDeltaSeconds(GetWorld());

		// Записываем множитель отдачи.
		const float _recoilWeapon = _weaponFire->GetRecoil();

		// Рассчитываем отдачу, с помощью функции расчета разброса.
		const float _newRecoil = _weaponFire->CalculateSpread(_recoilWeapon);

		// Рассчитываем вертикальный разброс.
		const float _addPitch = (Value * _recoilAmp) * _newRecoil * _deltaSecond;

		// Камера задерается вверх.
		AddControllerPitchInput(_addPitch);

	}
}

// Блокировать оружи при подходе к объекту.
void AFPSSoldier::BlockWeaponWall()
{
	// Высчитываем начало трейса.
	const FVector _startTrace = 
		GetSkeletal()->GetSocketLocation(FName("head")) + FVector(
			0.0,       // X
			0.0f,      // Y
			_highTrace  // Z
		);

	// Записывае поворот игрока.
	const FRotator _contrRotat = GetControlRotation();

	// Поворот трейса.
	const float _angle = FMath::ClampAngle(_contrRotat.Pitch,
		-90.0f, // Максимальное значение на которое трейс отпуститься вниз.
		90.0    // Максимальное значение на которое трейс подымиться вверх.
	);

	// Высчитываем направление трейса.
	const FVector _forwardVec = UKismetMathLibrary::GetForwardVector(FRotator(
		_angle, 
		_contrRotat.Yaw,
		_contrRotat.Roll)
	) * _lenghtTrace; // Длина трейса.

	// Высчитывает конец трейса.
	const FVector _endTrace = _startTrace + _forwardVec;

	// Результат трейса.
	FHitResult _hitResult;

	// Трейс сферы.
	const bool bHit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(),
		_startTrace, // Начало трейса.
		_endTrace, // Конец трейса.
		_radiusBlock, // Радиус трейса.
		_traceBlock, // Колизия трейса.
		false, //
		{this}, // Трейс игнорирует этих актеров.
		_drawDebugBlock, // Тип отображения трейса.
		_hitResult, // Результат попадания трейса.
		true // Игнорирует себя.
	);

	// Высчитываем дистанцию от трейса,
	// где он сталкивается до локации трейса где он начинается.
	const float _distanceVec = UKismetMathLibrary::Vector_Distance(
		_hitResult.Location, // Место где вектор сталкивается.
		_startTrace // Начало вектора.
	);

	const float _lerp = UKismetMathLibrary::Lerp(
		_blockAlpha,
		abs(_distanceVec / _lenghtTrace - 1.0f) * 2.0f,
		0.25f
	);

	// Выбераем значение.
	const float _selectFloat = UKismetMathLibrary::SelectFloat(
		_lerp, // Высчитанное значение опускание оружия.
		0.0f, // Бленд анимация проигрываться не будет.
		bHit // Если трейс столкнулся.
	);

	// Дельта секонд.
	const float _deltaSecond = 
		UGameplayStatics::GetWorldDeltaSeconds(GetWorld());

	// Плавно меняем значение.
	_blockAlpha = UKismetMathLibrary::FInterpTo(
		_blockAlpha, // Текущее значение.
		_selectFloat, // Значение которое получаем при столкновение.
		_deltaSecond, // Время в нашем мире.
		25.0f // Плавность.
	);

	// Если альфа больше.
	if (_blockAlpha >= 0.15f)
	{
		// Отключаем стрельбу.
		SetFireEnable(false);
	}
	else // Если меньше.
	{
		// Разрешаем стрельбу.
		SetFireEnable(true);
	}
}