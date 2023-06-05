// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSBullet.generated.h"

UCLASS(Abstract)
class FPSSHOOTER_API AFPSBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Конструктор пули.
	AFPSBullet(const FObjectInitializer& ObjectInitializer);

	// Колизиия.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class UBoxComponent* BoxCollision;

	// Модель пули.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh Component")
	class UStaticMeshComponent* Mesh;

	// Проджектаил компонент.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class UProjectileMovementComponent* Projectile;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	//class UParticleSystemComponent* BulletTrace;

protected:
	// Начало игры.
	virtual void BeginPlay() override;

};
