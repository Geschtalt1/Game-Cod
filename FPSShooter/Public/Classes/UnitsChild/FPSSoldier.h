
#pragma once

#include "CoreMinimal.h"
#include "Classes/FPSUnitsBase.h"
#include "Engine/EngineTypes.h"
#include "Components/TimelineComponent.h"
#include "Kismet/KismetSystemLibrary.h" // В будущем удалить.
#include "FPSSoldier.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FPSSHOOTER_API AFPSSoldier : public AFPSUnitsBase
{
	GENERATED_BODY()

	friend class AFPSFireArms;
	
public:
	// Конструктор.
	AFPSSoldier(const FObjectInitializer& ObjectInitializer);

	// Тик.
	virtual void Tick(float DeltaTime) override;

	// Эвент начало перекрытия Item капсулы.
	UFUNCTION()
		virtual void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp,
			class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult&
			SweepResult);

	// Эвент конец перекрытия Item капсулы.
	UFUNCTION()
		virtual void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp,
			class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);

	// Юнит стреляет из главного оружия.
	void OnFireMainUnit() override;

	// Евент взаимодействия с предметом.
	virtual void OnInteractionTrace_Implementation() override;

	// Юнит перезарежается.
	void OnReloadUnit() override;

	// Включает и отключает прицеливание.
	void OnAimUnit(bool bActive) override;

	// Юнит движеться вперед или назад.
	void OnMoveForward(float Value) override;

	// Юнит движеться вправо или влево.
	void OnMoveRight(float Value) override;

	// Остановить стрельбу.
	void OnStopFire() override;

	// Блокировать оружи при подходе к объекту.
	void BlockWeaponWall();

	// Setters.

	// Установить максимальную скорость передвижения.
	void SetSpeedMax(float NewSpeed) override;

	// Включить - отключить спринт.
	void SetSpeedRun(bool bSprint) override;
	
	// Выбрать расположение камеры.
	void SetCameraPosition(TEnumAsByte<ECameraPos> Position) override;

	// Установить приседание.
	UFUNCTION(BlueprintCallable, Category = "Soldier Crouch")
	virtual bool SetCrouch(bool bCrouch);

	// Getters.

	// Возращает коллизию солдата.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Soldier Component")
	FORCEINLINE UCapsuleComponent* GetCollision() const { return Collision; }

	// Возращает скелет солдата.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Soldier Component")
	FORCEINLINE USkeletalMeshComponent* GetSkeletal() const { return Skeletal; }

	// Возращает дочерний компонент оружия.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Soldier Component")
	FORCEINLINE UChildActorComponent* GetChildWeapon() const { return WeaponChildComponent; }

	// Возращает камеру солдата.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Soldier Component")
	FORCEINLINE UCameraComponent* GetCamera() const { return Camera; }

	// Возращает компонент выноливости.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Soldier Component")
	FORCEINLINE UFPSStaminaComponent* GetStamina() const { return Stamina; }

	// Возращает компонент движения солдата. 
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Soldier Component")
	FORCEINLINE UFPSMovementComponent* GetMovement() const { return Movement; }

	// Возращает ссылку на оружие.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Soldier Weapon")
	FORCEINLINE AFPSWeaponBase* GetWeapon() const { return _weaponSoldier; };

	// Возращает компонент экипировки.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Soldier Component")
	FORCEINLINE UFPSEquipmentComponent* GetEquipment() const { return Equipment; }

	// Возращает блок альфу.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Soldier Component")
	FORCEINLINE float GetBlockAlpha() const { return _blockAlpha; }

	// Возращает приседание солдата в данный момент.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Soldier Crouch")
	bool IsCrouchSoldier() const;

	// Возращает перезарежается ли юнит в данный момент.
	bool IsReloadUnit() const override;

	// Возращает передвигается ли юнит на спринте.
	bool IsRunUnit() const override;

	// Varieble.
	// Капсула коллизии персонажа.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class UCapsuleComponent* Collision;

	// Item колайдер.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class UCapsuleComponent* ItemCollision;

	// Скелет солдата.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class USkeletalMeshComponent* Skeletal;

	// Дочерний компонент оружия.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class UChildActorComponent* WeaponChildComponent;

	// Спринг арм.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class USpringArmComponent* SpringArm;

	// Камера солдата.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class UCameraComponent* Camera;

	// Компонент выносливости.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class UFPSStaminaComponent* Stamina;

	// Компонент передвижения.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class UFPSMovementComponent* Movement;

	// Компонент экипировки.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class UFPSEquipmentComponent* Equipment;

protected:
	// Начало игры.
	virtual void BeginPlay() override;

	// Пре инцилизация юнита.
	virtual void PostInitProperties() override;

	// Проигрываем анимационный монтаж солдата.
	UFUNCTION(BlueprintCallable, Category = "Soldier Animation")
	float PlayAnimationMontage(UAnimMontage* PlayMontage, bool bStopAllMontage = false);

	// Setters.

	// Getters.

	// Varieble.

	// Кривая для плавного прицеливания.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Aim"), Category = "Soldier|Animation|Curve")
	class UCurveFloat* _curveAim;

	// Кривая для плавной отдачи.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Recoil"), Category = "Soldier|Animation|Curve")
	class UCurveFloat* _curveRecoil;

	// Анимационный монтаж отдачи юнита при стрельбе.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Recoil When Shooting"), Category = "Soldier|Animation")
	UAnimMontage* _recoilWhenShooting;

	///////////////////////////////////////////////////

	// Радиус капсулы блокировки.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Radius Block"), Category = "Soldier|Block Wall")
	float _radiusBlock;

	// Длина блока.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Lenght Block"), Category = "Soldier|Block Wall")
	float _lenghtTrace;

	// Высота капсулы.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "High Block"), Category = "Soldier|Block Wall")
	float _highTrace;
		
	// Трейс блока.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Trace Block"), Category = "Soldier|Block Wall")
	TEnumAsByte<ETraceTypeQuery> _traceBlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Draw Debug"), Category = "Soldier|Block Wall")
	TEnumAsByte<EDrawDebugTrace::Type> _drawDebugBlock;

	/////////////////////////////////////////////////////////

private:

	// Плавно меняет TargetLenhgt у SpringArm, при прицеливание.
	// Привязывается к таймлайну _aimTiemLine.
	UFUNCTION()
	void OffsetTargetLenght(float Value);

	// Отдача при стрельбе.
	// При стрельбе задерает камеру вверх.
	// Привязывается к таймлайну _recoilTiemLine.
	UFUNCTION()
	void OffsetAimToFire(float Value);


	// Setters.

	// Getters.

	// Varieble.

	// Таймлайн прицелевания.
	FTimeline _aimTiemLine;

	// Таймер отдачи.
	FTimeline _recoilTiemLine;

	// Ссылка на оружие солдата.
	UPROPERTY()
	class AFPSWeaponBase* _weaponSoldier;

	// Альфа блокировки оружия возле стены.
	UPROPERTY()
	float _blockAlpha;
};
