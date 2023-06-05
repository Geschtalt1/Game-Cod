// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/WeaponChild/FPSFireArms.h"
#include "Classes/UnitsChild/FPSSoldier.h"
#include "Component/FPSMovementComponent.h"
#include "FPSPlayerController.h"
#include "Other/FPSBullet.h"
#include "Other/FPSTypename.h"
#include "FPSHUD.h"

#include "Animation/AnimationAsset.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Camera/CameraComponent.h"

#include "Sound/SoundCue.h"
#include "Other/FPSTypename.h"

#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

#include "Blueprint/WidgetLayoutLibrary.h"

#include "Components/SkeletalMeshComponent.h"

// Конструктор огнестрельного оружия.
AFPSFireArms::AFPSFireArms(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Компонент меша предмета удален.
	ItemMesh->DestroyComponent();

	// Компонент скелета огнестрельного оружия.
	SkeletalFireArms = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
	SkeletalFireArms->SetCustomDepthStencilValue(1);
	SkeletalFireArms->SetGenerateOverlapEvents(true);
	SkeletalFireArms->SetCollisionObjectType(CHANNELS_ITEM);
	SkeletalFireArms->SetCollisionResponseToChannel(CHANNELS_ITEM, ECR_Block);
	SkeletalFireArms->SetCollisionResponseToChannel(CHANNELS_ITEM_LINE, ECR_Block);
	SetRootComponent(SkeletalFireArms);

	_fireRate = 0.0f;
	_maxAmmo = 0;
	_recoilWeapon = 0.0f;
	_currentAmmo = 0;
	_bulletSpreadMax = 1000.0f;
	_bulletSpread = 0.0f;

	_currentSpread = 0.0f;

	_bReload = false;
	_bEnableFireMode = true;

	_reloadSound = nullptr;
	_bulletTrace = nullptr;

	_breechMechanism = nullptr;
	_reloadLoadedPawn = nullptr;
	_reloadEmptyPawn = nullptr;
	_selectFireMode = nullptr;

	_fireMode = FM_Full;
}

// Пре инцилизация огнестрельного оружия.
void AFPSFireArms::PostInitProperties()
{
	Super::PostInitProperties();

	// Устанавливаем текущее кол-во патронов, равное максимальному.
	SetAmmoCount(_maxAmmo);
}

// Выстрел из оружия.
void AFPSFireArms::OnFire()
{
	if (IsAmmo()) // Если патроны в оружие есть.
	{
		// Вызываем делегат выстрела.
		OnFireWeapon.Broadcast();

		// Переменная для результата выстрела.
		FHitResult Hit;

		// Рассчитываем на сколько увеичиваеть разброс.
		const float _addSpread = CalculateSpread(GetSpread());
		AddSpreadWeapon(_addSpread);

		// Производим звук стрельбы.
		PlaySoundWeapon(_fireSound, _soundAttenuation);

		// Спавним вспышку выстрела.
		SpawnMuzzleEffect();

		// Спавним луч.
		const bool bHit = SpawTraceFromCamera(Hit,  // Возращаем результат выстрела.
			GetLenghtTrace(), // Длина луча при выстреле.
			GetSpreadCurrent(), // Разброс луча.
			_traceDebug); // Вкл - выкл видимость луча выстрела.

		// Спавн пули.
		SpawnBullet(_bulletTrace, Hit);

		// Анимация затвора.
		GetSkeletal()->PlayAnimation(_breechMechanism, false);

		// Если владелец оружия валиден.
		if (GetWeaponOwner())
		{
			// Вызываем функцую отдачи.
			GetWeaponOwner()->_recoilTiemLine.PlayFromStart();
			
			// Записываем ссылку на HUD.
			AFPSHUD* _hudLocal = GetWeaponOwner()->
				GetPlayerHUD();

			// Проверяем ссылку на HUD.
			if (_hudLocal) 
			{
				// Добавляем отдачу в прицел.
				_hudLocal->OnCrosshireChanged.Broadcast(GetSpread());
			}
		}

		// Высчитываем новое кол-во боеприпасов.
		// От текущего кол-во патронов, отнимаем один.
		const int32 _ammoNew = GetAmmoCurrent() - 1;
		SetAmmoCount(_ammoNew);

		// Обрабатываем место попадания, если луч встретил препятствие.
		if (bHit) {
			OnHandleHit(Hit);
		}

		// Если тип стрельбы выбран автоматический.
		if (GetFireMode() == FM_Full) 
		{			
			// Таймер.
			GetWorld()->GetTimerManager().SetTimer(_thFireMain,
			GetWeaponOwner(),
			&AFPSSoldier::OnFireMainUnit, // Включаем функцию стрельбы.
			GetFireRate(), // Скорострельность.
			true // Зацикливать.
			); // Первый запуск с задержкой.
		}
		else // Если режим огня одиночный.
		{
			// Очищаем разброс.
			StopFire();
		}
	}
	else
	{
		// Если патронов нет, проигрываем звук щелчка.
		PlaySoundWeapon(_noAmmoSound, _soundAttenuation);

		// И останавливаем стрельбу если она была зациклена.
		StopFire();
	}
}

// Перезарядка оружия.
void AFPSFireArms::OnReload()
{
	// Если владелец существует.
	if (GetWeaponOwner())
	{
		// Проверяем, что магазин с патронами не полный.
		if (GetAmmoCurrent() != GetAmmoMax()) 
		{
			// Если магазин не полный, включаем перезарядку.
			_bReload = true;
		}
		else 
		{
			// Если магазин полный, завершаем работу функции.
			return;
		}

		// Отключаем стрельбу.
		StopFire();

		// Время проигрывания монтажа.
		float _timeMontage;

		// Таймер задержки запуска функции EndReload.
		FTimerHandle TH_Reload;

		// Если кол-во патронов, не равно нулю, 
		// анимация перезарядки будет не полной.
		if (GetAmmoCurrent() != 0)
		{
			// Включаем аним монтаж не полной перезарядки.
			_timeMontage = GetWeaponOwner()->
				PlayAnimationMontage(_reloadLoadedPawn);

			// Запускаем функцию конца перезарядки по истечению времени монтажа.
			GetWorld()->GetTimerManager().SetTimer(
				TH_Reload,               // Таймер.
				this,                    // В каком классе вызывается.
				&AFPSFireArms::EndReload,// Какая функция вызывается.
				_timeMontage,            // Через скока секунд вызовиться метод.
				false                    // Зацикливание.
			);

		}
		else // Если кол-во равно нулю, анимация перезарядки будет полной.
		{
			// Включам аним монтаж полной перезарядки.
			_timeMontage = GetWeaponOwner()->
				PlayAnimationMontage(_reloadEmptyPawn);

			// Запускаем функцию конца перезарядки по истечению времени монтажа.
			GetWorld()->GetTimerManager().SetTimer(
				TH_Reload,               // Таймер.
				this,                    // В каком классе вызывается.
				&AFPSFireArms::EndReload,// Какая функция вызывается.
				_timeMontage,            // Через скока секунд вызовиться метод.
				false                    // Зацикливание.
			);
		}
	}
	else // Если владельца нет, заполняем патроны без анимаций перезарядки.
	{
		SetAmmoCount(GetAmmoMax());

		// И останавливаем стрельбу если она была зациклена.
		StopFire();
	}
}

bool AFPSFireArms::SpawTraceToMuzzle(FHitResult& Result, float Lenght, bool bDebugLine) const
{
	// Имя сокета где спавниться луч.
	const FName _socketName = GetMuzzleSocket();

	// Высчитываем длину луча и начало трейса.
	const FVector _lenghtTraceLoc = SkeletalFireArms->GetForwardVector() * Lenght;
	const FVector _startTrace = SkeletalFireArms->GetSocketLocation(_socketName);

	// Высчитываем конец трейса, складываем длину и начало луча.
	const FVector _endTrace = _lenghtTraceLoc + _startTrace;

	// Настройки луча.
	FCollisionQueryParams _traceParams;

	// Спавним луч.
	const bool bHit = GetWorld()->LineTraceSingleByChannel(
		Result, // Возращаем результат попадания луча. 
		_startTrace, // Начало луча.
		_endTrace, // Конец луча.
		CHANNELS_BULLET_LINE, // Коллизия луча.
		_traceParams // Параметры коллизии.
	);

	if (bDebugLine) // Показывает линию лайн трейса, если дебаг включен.
	{
		// Показываем линию.
		DrawDebugLine(GetWorld(), _startTrace, _endTrace, FColor::Red, false, 2.0f);

		// Если трейс во что-то попал, на месте попадание появляется квадрат.
		if (bHit) {
			DrawDebugBox(GetWorld(), Result.ImpactPoint, FVector(5, 5, 5), FColor::Emerald, false, 2.0f);
		}
	}

	return bHit;
}

bool AFPSFireArms::SpawTraceFromCamera(FHitResult& Result, float Lenght, float Spread, bool bDebugLine) const
{
	// Ссылка на камеру игрока.
	const UCameraComponent* _cameraComp = 
		GetWeaponOwner()->GetCamera();

	// Если владелец действителен.
	if (GetWeaponOwner())
	{
		// Разброс луча.
		FVector _spray = FVector(UKismetMathLibrary::RandomFloatInRange(Spread * (-1), Spread), // X
			UKismetMathLibrary::RandomFloatInRange(Spread * (-1), Spread), // Y
			UKismetMathLibrary::RandomFloatInRange(Spread * (-1), Spread)); // Z

		// Координаты луча.
		const FVector _startTrace = _cameraComp->GetComponentLocation();
		const FVector _endTrace = ((_cameraComp->GetForwardVector() * Lenght) +
			_startTrace) + _spray;

		// Настройки луча.
		FCollisionQueryParams _traceParams;

		// Спавним луч.
		const bool bHit = GetWorld()->LineTraceSingleByChannel(
			Result, // Возращаем результат попадания луча. 
			_startTrace, // Начало луча.
			_endTrace, // Конец луча.
			CHANNELS_BULLET_LINE, // Коллизия луча.
			_traceParams // Параметры коллизии.
		);

		if (bDebugLine) // Показывает линию лайн трейса, если дебаг включен.
		{
			// Показываем линию.
			DrawDebugLine(GetWorld(), _startTrace, _endTrace, FColor::Red, false, 2.0f);

			// Если трейс во что-то попал, на месте попадание появляется квадрат.
			if (bHit) {
				DrawDebugBox(GetWorld(), Result.ImpactPoint, FVector(5, 5, 5), FColor::Emerald, false, 2.0f);
			}
		}

		return bHit;
	}

	return false;
}

// Установить режим огня.
void AFPSFireArms::SetFireMode(TEnumAsByte<EFireMode> FireMode)
{
	// Проверяем если смена режима огня разрешена.
	if (_bEnableFireMode)
	{
		// Меняем режим огня.
		_fireMode = FireMode;

		// Производим звук смены режима огня.
		PlaySoundWeapon(_selectFireMode, _soundAttenuation);
	}
}

// Установить максимальное кол-во патронов.
void AFPSFireArms::SetAmmoMax(int32 NewMax)
{
	_maxAmmo = abs(NewMax);
}

// Установить текущее кол-во патронов.
int32 AFPSFireArms::SetAmmoCount(int32 NewCount)
{
	_currentAmmo = FMath::Clamp(NewCount, // Новое значение текущих патронов.
		NULL, // Минимальное значение которое может быть в магазине.
		GetAmmoMax()); // Максимальное значение которое может быть в магазине.

	return _currentAmmo;
}

// Спавн пули при выстреле.
void AFPSFireArms::SpawnBullet(TSubclassOf<AFPSBullet> Bullet, FHitResult Hit)
{
	if (Bullet)
	{
		// Записывам сокет на котором будет спавниться пуля.
		const FName _socketBullet = GetMuzzleSocket();

		// Записываем расположение сокета где спавниться пуля.
		const FVector _locationSpawn = GetSkeletal()->GetSocketLocation(_socketBullet);

		// Выбирается вектор для поиска вращения спавна, в зависимости попал или не попал луч во что нибудь.
		const FVector _locationTrace = UKismetMathLibrary::SelectVector(
			Hit.ImpactPoint, // Контакт луча с объектом на пути.
			Hit.TraceEnd, // Конец трейса.
			Hit.bBlockingHit // Встретил или не встретил луч препятсвий.
		);

		// Находим направление спавна пули из дула.
		const FRotator _directionSpawn = UKismetMathLibrary::FindLookAtRotation(
			_locationSpawn,
			_locationTrace
		);

		// Спавним пулю.
		GetWorld()->SpawnActor<AFPSBullet>(
			Bullet, // Класс пули.
			_locationSpawn, // Место спавна.
			_directionSpawn); // Направление полета пули.
	}
}

// Спавн вспышки.
void AFPSFireArms::SpawnMuzzleEffect()
{
	if (_muzzleEffect)
	{
		// Записываем имя сокета на стволе.
		const FName _socketMuzzle = GetMuzzleSocket();
		
		// Берем координаты расположения сокета на стволе.
		const FVector _socketLocation = 
			GetSkeletal()->GetSocketLocation(_socketMuzzle);

		// Берем вращения сокета на стволе.
		const FRotator _socketRotation = 
			GetSkeletal()->GetSocketRotation(_socketMuzzle);

		// Спавним ээфект выстрела.
		UNiagaraFunctionLibrary::SpawnSystemAttached(
			_muzzleEffect, // Эффект вспышки.
			GetSkeletal(), // К чему крепится эффект.
			FName{}, // 
			_socketLocation, // Место спавна.
			_socketRotation, // Поворот вспышки.
			EAttachLocation::KeepWorldPosition, // Тип крепления к скелету.
			true // Автоматическое уничтожение.
		);
	}
}

// Расчитать разброс.
// Установить множитель разброса.
// Установить процент увелечения разброса при движение.
// Установить процент увелечения разброса при не приседание.
// Установить процент увелечения разброса при не прицеливание.
float AFPSFireArms::CalculateSpread(float Spread, float PercentMove, float PercentNotCrouch, float PercentNotAim)
{
	if (GetWeaponOwner())
	{
		// Ссылка на юнита.
		const AFPSSoldier& _soldier = *GetWeaponOwner();

		// Переменные для проверки состояния юнита в данный момент.
		const bool _bMove = _soldier.GetMovement()->IsMovingOnGround();
		const bool _bCrouch = _soldier.IsCrouchSoldier();
		const bool _bAim = _soldier.IsAimUnit();

		if (_bMove) // Если юнит двигается, увеличиваем разброс.
		{
			Spread += (Spread / 100.0f) * PercentMove;
		}

		if (!_bCrouch) // Если юнит не приседает, увеличиваем разброс.  
		{
			Spread += (Spread / 100.0f) * PercentNotCrouch;
		}

		if (!_bAim) // Если юнит не прицеливается, увеличиваем разброс.
		{
			Spread += (Spread / 100.0f) * PercentNotAim;
		}

		return Spread; // Возращаем разброс.
	}

	return 0.0f;
}

// Сбрасываем текущий разброс.
void AFPSFireArms::ResetSpread()
{
	_currentSpread = 0.0f;
}

// Добавить разбросс.
float AFPSFireArms::AddSpreadWeapon(float Value)
{
	// Зажимае разброс.
	_currentSpread = FMath::Clamp(
		GetSpreadCurrent() + Value, // Прибавляем к текущему разбросу.
		0.0f, // Минимальный разброс.
		_bulletSpreadMax // Максимальный разброс.
	);
	
	// Возращаем текущий разброс.
	return _currentSpread;
}

// Конец перезарядки.
void AFPSFireArms::EndReload()
{
	// Отключаем перезарядку.
	_bReload = false;

	// Обновляем патроны в магазине.
	SetAmmoCount(GetAmmoMax());
}

// Остановить стрельбу.
void AFPSFireArms::StopFire()
{
	// Если режим огня полный.
	if (GetFireMode() == FM_Full) 
	{
		// Очищаем таймер.
		GetWorld()->GetTimerManager().ClearTimer(_thFireMain);
	}

	// Сбрасываем разброс.
	ResetSpread();
}

// Вызывается когда игрок посмотрел на предмет.
void AFPSFireArms::OnBeginLookItem_Implementation(const AFPSSoldier* Soldier)
{
	// Включаем обводку оружия.
	SkeletalFireArms->SetRenderCustomDepth(true);
}

// Вызывается когда игрок закончил смотреть на предмет.
void AFPSFireArms::OnEndLookItem_Implementation(const AFPSSoldier* Soldier)
{
	// Отключаем обводку оружия.
	SkeletalFireArms->SetRenderCustomDepth(false);
}