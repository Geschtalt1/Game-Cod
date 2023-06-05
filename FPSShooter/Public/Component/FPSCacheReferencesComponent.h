// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FPSCacheReferencesComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPSSHOOTER_API UFPSCacheReferencesComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Конструктор.
	UFPSCacheReferencesComponent(const FObjectInitializer& ObjectInitializer);

	// Тик.
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Возращает игрока компонента.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Cache References Component")
	FORCEINLINE AFPSUnitsBase* GetPlayerComponent() const { return _ownerComp; }

protected:
	// Начало игры.
	virtual void BeginPlay() override;

private:

	// Variable.

	// Ссылка на владельца компонентом.
	class AFPSUnitsBase* _ownerComp;
};
