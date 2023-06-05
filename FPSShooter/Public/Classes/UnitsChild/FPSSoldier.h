
#pragma once

#include "CoreMinimal.h"
#include "Classes/FPSUnitsBase.h"
#include "Engine/EngineTypes.h"
#include "Components/TimelineComponent.h"
#include "Kismet/KismetSystemLibrary.h" // � ������� �������.
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
	// �����������.
	AFPSSoldier(const FObjectInitializer& ObjectInitializer);

	// ���.
	virtual void Tick(float DeltaTime) override;

	// ����� ������ ���������� Item �������.
	UFUNCTION()
		virtual void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp,
			class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult&
			SweepResult);

	// ����� ����� ���������� Item �������.
	UFUNCTION()
		virtual void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp,
			class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);

	// ���� �������� �� �������� ������.
	void OnFireMainUnit() override;

	// ����� �������������� � ���������.
	virtual void OnInteractionTrace_Implementation() override;

	// ���� ��������������.
	void OnReloadUnit() override;

	// �������� � ��������� ������������.
	void OnAimUnit(bool bActive) override;

	// ���� ��������� ������ ��� �����.
	void OnMoveForward(float Value) override;

	// ���� ��������� ������ ��� �����.
	void OnMoveRight(float Value) override;

	// ���������� ��������.
	void OnStopFire() override;

	// ����������� ����� ��� ������� � �������.
	void BlockWeaponWall();

	// Setters.

	// ���������� ������������ �������� ������������.
	void SetSpeedMax(float NewSpeed) override;

	// �������� - ��������� ������.
	void SetSpeedRun(bool bSprint) override;
	
	// ������� ������������ ������.
	void SetCameraPosition(TEnumAsByte<ECameraPos> Position) override;

	// ���������� ����������.
	UFUNCTION(BlueprintCallable, Category = "Soldier Crouch")
	virtual bool SetCrouch(bool bCrouch);

	// Getters.

	// ��������� �������� �������.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Soldier Component")
	FORCEINLINE UCapsuleComponent* GetCollision() const { return Collision; }

	// ��������� ������ �������.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Soldier Component")
	FORCEINLINE USkeletalMeshComponent* GetSkeletal() const { return Skeletal; }

	// ��������� �������� ��������� ������.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Soldier Component")
	FORCEINLINE UChildActorComponent* GetChildWeapon() const { return WeaponChildComponent; }

	// ��������� ������ �������.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Soldier Component")
	FORCEINLINE UCameraComponent* GetCamera() const { return Camera; }

	// ��������� ��������� �����������.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Soldier Component")
	FORCEINLINE UFPSStaminaComponent* GetStamina() const { return Stamina; }

	// ��������� ��������� �������� �������. 
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Soldier Component")
	FORCEINLINE UFPSMovementComponent* GetMovement() const { return Movement; }

	// ��������� ������ �� ������.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Soldier Weapon")
	FORCEINLINE AFPSWeaponBase* GetWeapon() const { return _weaponSoldier; };

	// ��������� ��������� ����������.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Soldier Component")
	FORCEINLINE UFPSEquipmentComponent* GetEquipment() const { return Equipment; }

	// ��������� ���� �����.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Soldier Component")
	FORCEINLINE float GetBlockAlpha() const { return _blockAlpha; }

	// ��������� ���������� ������� � ������ ������.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Soldier Crouch")
	bool IsCrouchSoldier() const;

	// ��������� �������������� �� ���� � ������ ������.
	bool IsReloadUnit() const override;

	// ��������� ������������� �� ���� �� �������.
	bool IsRunUnit() const override;

	// Varieble.
	// ������� �������� ���������.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class UCapsuleComponent* Collision;

	// Item ��������.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class UCapsuleComponent* ItemCollision;

	// ������ �������.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class USkeletalMeshComponent* Skeletal;

	// �������� ��������� ������.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class UChildActorComponent* WeaponChildComponent;

	// ������ ���.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class USpringArmComponent* SpringArm;

	// ������ �������.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class UCameraComponent* Camera;

	// ��������� ������������.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class UFPSStaminaComponent* Stamina;

	// ��������� ������������.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class UFPSMovementComponent* Movement;

	// ��������� ����������.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class UFPSEquipmentComponent* Equipment;

protected:
	// ������ ����.
	virtual void BeginPlay() override;

	// ��� ����������� �����.
	virtual void PostInitProperties() override;

	// ����������� ������������ ������ �������.
	UFUNCTION(BlueprintCallable, Category = "Soldier Animation")
	float PlayAnimationMontage(UAnimMontage* PlayMontage, bool bStopAllMontage = false);

	// Setters.

	// Getters.

	// Varieble.

	// ������ ��� �������� ������������.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Aim"), Category = "Soldier|Animation|Curve")
	class UCurveFloat* _curveAim;

	// ������ ��� ������� ������.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Recoil"), Category = "Soldier|Animation|Curve")
	class UCurveFloat* _curveRecoil;

	// ������������ ������ ������ ����� ��� ��������.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Recoil When Shooting"), Category = "Soldier|Animation")
	UAnimMontage* _recoilWhenShooting;

	///////////////////////////////////////////////////

	// ������ ������� ����������.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Radius Block"), Category = "Soldier|Block Wall")
	float _radiusBlock;

	// ����� �����.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Lenght Block"), Category = "Soldier|Block Wall")
	float _lenghtTrace;

	// ������ �������.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "High Block"), Category = "Soldier|Block Wall")
	float _highTrace;
		
	// ����� �����.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Trace Block"), Category = "Soldier|Block Wall")
	TEnumAsByte<ETraceTypeQuery> _traceBlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Draw Debug"), Category = "Soldier|Block Wall")
	TEnumAsByte<EDrawDebugTrace::Type> _drawDebugBlock;

	/////////////////////////////////////////////////////////

private:

	// ������ ������ TargetLenhgt � SpringArm, ��� ������������.
	// ������������� � ��������� _aimTiemLine.
	UFUNCTION()
	void OffsetTargetLenght(float Value);

	// ������ ��� ��������.
	// ��� �������� �������� ������ �����.
	// ������������� � ��������� _recoilTiemLine.
	UFUNCTION()
	void OffsetAimToFire(float Value);


	// Setters.

	// Getters.

	// Varieble.

	// �������� ������������.
	FTimeline _aimTiemLine;

	// ������ ������.
	FTimeline _recoilTiemLine;

	// ������ �� ������ �������.
	UPROPERTY()
	class AFPSWeaponBase* _weaponSoldier;

	// ����� ���������� ������ ����� �����.
	UPROPERTY()
	float _blockAlpha;
};
