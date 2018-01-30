// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "LaserProjectile.generated.h"

UCLASS()
class TESTBED_API ALaserProjectile : public AActor
{
	GENERATED_BODY()

public:
    USceneComponent* mSceneRoot;
        UStaticMeshComponent* mLaserMesh;
        UProjectileMovementComponent* mProjectileMovementComponent;

public:	
	// Sets default values for this actor's properties
	ALaserProjectile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

    void EnableTick( bool enable );
	
};
