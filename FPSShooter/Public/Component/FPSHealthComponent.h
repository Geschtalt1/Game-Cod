// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/FPSCacheReferencesComponent.h"
#include "FPSHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeadMulticastSignature);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPSSHOOTER_API UFPSHealthComponent : public UFPSCacheReferencesComponent
{
	GENERATED_BODY()

public:	
	// ����������� ���������� ��������.
	UFPSHealthComponent(const FObjectInitializer& ObjectInitializer);

	// ������� ������ �����.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "On Dead", Category = "Default", MultiLine = "true", OverrideNativeName = "OnDead"))
	FOnDeadMulticastSignature OnDead;

	// ��������� �����.
	UFUNCTION(BlueprintCallable, Category = "Dammage Component")
	virtual float AnyDammage(float DamageAmount);

	// Setters.

	// ���������� ������������ �������� �����.
	UFUNCTION(BlueprintCallable, Category = "Health Component")
	virtual void SetHealthMax(float NewMax);
		
	// ���������� ������� �������� �����.
	UFUNCTION(BlueprintCallable, Category = "Health Component")
	virtual float SetHealthAmmount(float NewHealth);

	// Getters.

	// ��������� ������������ ���-�� ��.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Health Component")
	FORCEINLINE float GetHealthMax() { return _healthMax; }

	// ��������� ������� ���-�� ��.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Health Component")
	FORCEINLINE float GetHealthCount() { return _healthAmmount; }

protected:
	// ������ ����.
	virtual void BeginPlay() override;

	// ��� ����������� ���������� ��������.
	virtual void PostInitProperties() override;

	// Variable.

	// ������������ ���-�� ��.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (DisplayName = "Health Max", ClampMin = 0), Category = "Health")
	float _healthMax;

	// ������������� �� � �������.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (DisplayName = "Health Recovery Per Second", ClampMin = 0), Category = "Health")
	float _recoveryHealth;

private:

	// Variable.
	  
	// ���-�� �� ������ ������ �������� (�������� �� 0.0 �� 100.0).
	float _healthAmmount;

};
