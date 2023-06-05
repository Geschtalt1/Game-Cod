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
	// Конструктор компонента здоровья.
	UFPSHealthComponent(const FObjectInitializer& ObjectInitializer);

	// Делегат гибили юнита.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, BlueprintAssignable, BlueprintCallable, meta = (DisplayName = "On Dead", Category = "Default", MultiLine = "true", OverrideNativeName = "OnDead"))
	FOnDeadMulticastSignature OnDead;

	// Получение урона.
	UFUNCTION(BlueprintCallable, Category = "Dammage Component")
	virtual float AnyDammage(float DamageAmount);

	// Setters.

	// Установить максимальное здоровье юниту.
	UFUNCTION(BlueprintCallable, Category = "Health Component")
	virtual void SetHealthMax(float NewMax);
		
	// Установить текущее здоровье юниту.
	UFUNCTION(BlueprintCallable, Category = "Health Component")
	virtual float SetHealthAmmount(float NewHealth);

	// Getters.

	// Возращает максимальное кол-во ХП.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Health Component")
	FORCEINLINE float GetHealthMax() { return _healthMax; }

	// Возращает текущее кол-во ХП.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Health Component")
	FORCEINLINE float GetHealthCount() { return _healthAmmount; }

protected:
	// Начало игры.
	virtual void BeginPlay() override;

	// Пре инцилизация компонента здоровья.
	virtual void PostInitProperties() override;

	// Variable.

	// Максимальное кол-во хп.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (DisplayName = "Health Max", ClampMin = 0), Category = "Health")
	float _healthMax;

	// Востановление ХП в секунду.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (DisplayName = "Health Recovery Per Second", ClampMin = 0), Category = "Health")
	float _recoveryHealth;

private:

	// Variable.
	  
	// Кол-во на данный момент здоровья (диапозон от 0.0 до 100.0).
	float _healthAmmount;

};
