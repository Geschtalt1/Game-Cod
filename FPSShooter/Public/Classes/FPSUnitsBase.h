
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Kismet/KismetMathLibrary.h"

#include "Other/FPSEnumeration.h"
#include "Interface/FPSInterfaceControll.h"

#include "FPSUnitsBase.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFireMulticastDelegate);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Abstract)
class FPSSHOOTER_API AFPSUnitsBase : public APawn, public IFPSInterfaceControll
{
	GENERATED_BODY()

public:
	// �����������.
	AFPSUnitsBase(const FObjectInitializer& ObjectInitializer);

	// ���.
	virtual void Tick(float DeltaTime) override;

	// ��������� �����.
	float TakeDamage(float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser) override;

	// Function Interface.

	// ����� �������������� � ���������.
	virtual void OnInteractionTrace_Implementation() override;

	// ���� �������� �� �������� ������.
	UFUNCTION(BlueprintCallable, Category = "Unit Action")
	virtual void OnFireMainUnit();

	// ���� ��������������.
	UFUNCTION(BlueprintCallable, Category = "Unit Action")
	virtual void OnReloadUnit();

	// ���� �������.
	UFUNCTION(BlueprintCallable, Category = "Unit Action")
	virtual void OnAimUnit(bool bActive);

	// ���� ��������� ������ ��� �����.
	UFUNCTION(BlueprintCallable, Category = "Unit Action")
	virtual void OnMoveForward(float Value);

	// ���� ��������� ������ ��� �����.
	UFUNCTION(BlueprintCallable, Category = "Unit Action")
	virtual void OnMoveRight(float Value);

	// ���������� ��������.
	UFUNCTION(BlueprintCallable, Category = "Unit Action")
	virtual void OnStopFire();

	// Setters.

	// ���������� ������������ �������� ������������.
	UFUNCTION(BlueprintCallable, Category = "Unit Speed")
	virtual void SetSpeedMax(float NewSpeed);

	// ���������� ���������� �������� ������������.
	UFUNCTION(BlueprintCallable, Category = "Unit Speed")
	virtual void SetSpeedRun(bool bSprint);

	// ������� ������������ ������.
	UFUNCTION(BlueprintCallable, Category = "Unit Setting")
	virtual void SetCameraPosition(TEnumAsByte<ECameraPos> Position);

	// �������� ��� ��������� �������� �� ������.
	UFUNCTION(BlueprintCallable, Category = "Unit Setting")
	virtual void SetFireEnable(bool bEnable);

	// Getters.

	// ��������� ��� ��� ����� ���� � ������ ������.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Unit Alive")
	bool GetAlive() const;

	// ��������� �������������� �� ���� � ������ ������.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Unit Reload")
	virtual bool IsReloadUnit() const;

	// ��������� ������������� �� ���� �� �������.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Unit Speed")
	virtual bool IsRunUnit() const;

	// ��������� ������������� �� ����.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Unit Aim")
	virtual bool IsAimUnit() const;

	// ��������� ����� �� ����� ��������.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Unit Action")
	FORCEINLINE bool GetFireEnable() const { return (bool)_bFireEnable; };

	// ��������� �������� �����.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Unit Speed")
	FORCEINLINE float GetSpeedUnit() const { 
		return UKismetMathLibrary::VSize(GetVelocity()); }

	// ��������� ��������� ��������.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Unit Speed")
	FORCEINLINE UFPSHealthComponent* GetHealth() const { return Health; }

	// ��������� ������ �� HUD ������.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Unit Widget")
	FORCEINLINE AFPSHUD* GetPlayerHUD() const { return _hudPlayer; }
	
	// Varieble.

	// ��������� ��������.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class UFPSHealthComponent* Health;

	// ������� ��������.
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "On Fire", Category = "Default", MultiLine = "true", OverrideNativeName = "OnFire"))
	//FOnFireMulticastDelegate OnFireUnit;

protected:
	// ������ ����.
	virtual void BeginPlay() override;

	// ��� ����������� �����.
	virtual void PostInitProperties() override;

	// ����� �����.
	UFUNCTION()
	virtual void OnKillUnit();

	// Setters.

	// Getters.

	// Varieble.

	// ������������� �� ����.
	uint8 _bAim;

private:
	// Setters.

	// Getters.

	// Varieble.

	// ���������� �� �����, ��������.
	uint8 _bFireEnable;

	// ������ �� HUD ������.
	class AFPSHUD* _hudPlayer;

};
