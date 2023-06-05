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
	// �����������.
	UFPSCacheReferencesComponent(const FObjectInitializer& ObjectInitializer);

	// ���.
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// ��������� ������ ����������.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Cache References Component")
	FORCEINLINE AFPSUnitsBase* GetPlayerComponent() const { return _ownerComp; }

protected:
	// ������ ����.
	virtual void BeginPlay() override;

private:

	// Variable.

	// ������ �� ��������� �����������.
	class AFPSUnitsBase* _ownerComp;
};
