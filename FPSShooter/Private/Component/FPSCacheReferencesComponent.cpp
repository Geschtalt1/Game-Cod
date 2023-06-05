// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/FPSCacheReferencesComponent.h"
#include "Classes/FPSUnitsBase.h"

// �����������.
UFPSCacheReferencesComponent::UFPSCacheReferencesComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// ������ ����.
void UFPSCacheReferencesComponent::BeginPlay()
{
	Super::BeginPlay();

	// ���������� ������ �� ���������.
	_ownerComp = Cast<AFPSUnitsBase>(GetOwner());

	// ...
	
}


// ���.
void UFPSCacheReferencesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

