// Fill out your copyright notice in the Description page of Project Settings.


#include "Other/FPSBullet.h"

#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"

#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AFPSBullet::AFPSBullet(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	// Коллизия.
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	SetRootComponent(BoxCollision);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	Mesh->SetupAttachment(BoxCollision);

	/* Bullet line trace */
	//BulletTrace = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trace"));
	//BulletTrace->SetupAttachment(BoxCollision);

	/* Projectile Movement */
	Projectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Bullet Trace"));

	InitialLifeSpan = 0.7f;

}

// Called when the game starts or when spawned
void AFPSBullet::BeginPlay()
{
	Super::BeginPlay();
	
}
