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
	// ����������� ����.
	AFPSBullet(const FObjectInitializer& ObjectInitializer);

	// ��������.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class UBoxComponent* BoxCollision;

	// ������ ����.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh Component")
	class UStaticMeshComponent* Mesh;

	// ����������� ���������.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class UProjectileMovementComponent* Projectile;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	//class UParticleSystemComponent* BulletTrace;

protected:
	// ������ ����.
	virtual void BeginPlay() override;

};
