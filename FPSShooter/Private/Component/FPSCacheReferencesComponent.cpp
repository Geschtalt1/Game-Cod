// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/FPSCacheReferencesComponent.h"
#include "Classes/FPSUnitsBase.h"

// Конструктор.
UFPSCacheReferencesComponent::UFPSCacheReferencesComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Начало игры.
void UFPSCacheReferencesComponent::BeginPlay()
{
	Super::BeginPlay();

	// Записываем ссылку на владельца.
	_ownerComp = Cast<AFPSUnitsBase>(GetOwner());

	// ...
	
}


// Тик.
void UFPSCacheReferencesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

