// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/UnitsChild/FPSVehicle.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Camera/CameraComponent.h"

#include "GameFramework/SpringArmComponent.h"

AFPSVehicle::AFPSVehicle(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
	bUseControllerRotationYaw = false;

	// Компонент скелета.
	Skeletal = CreateDefaultSubobject<USkeletalMeshComponent>("Skeletal Mesh Component");
	//Skeletal->SetCollisionObjectType(CHANNELS_SOLDIER);
	Skeletal->SetSimulatePhysics(true);
	Skeletal->SetGenerateOverlapEvents(true);
	SetRootComponent(Skeletal);

	// Спринг арм.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Component");
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SetWorldLocation(FVector{ 0.0f, 0.0f, 250.0f });
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetupAttachment(Skeletal);

	// Компонент камеры.
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera Component");
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = true;
}