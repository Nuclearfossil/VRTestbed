// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "StarfighterMovementComponent.h"
#include "LaserProjectile.h"
#include "UIVelocityComponent.h"
#include "StarfighterPawn.generated.h"

class USceneComponent;
class UCameraComponent;
class UPointLightComponent;
class UStaticMeshComponent;

UCLASS()
class TESTBED_API AStarfighterPawn : public APawn
{
	GENERATED_BODY()

private:
    float mYaw;
    float mPitch;
    float mRoll;
    float mHeadHeight;

    int mEmitterCount;
    int mLastEmitterUsed;

    const UStaticMeshSocket* mTargetPoint;
    TArray<const UStaticMeshSocket*> mLaserEmmiters;
    UClass* LaserProjectile;

    FTransform mLastTargetTransform;
    FVector mActualTargetPosition;

public:
    // Normally I don't indent properties, but in this case, I want to in order to illustrate the intended hierarchy.
    USceneComponent* mSceneRoot;
        UStaticMeshComponent* mCockpitMesh;
        UStaticMeshComponent* mWeaponMesh;
        UPointLightComponent* mBackgroundLight;
        USceneComponent* mChairNode;
            UCameraComponent* mCamera;
        // UStaticMeshComponent* mDebugReticle;

    class UStarfighterMovementComponent* mMovementComponent;

    void MoveForward( float AxisValue );
    void MoveRight( float AxisValue );
    void Turn( float AxisValue );
    void LookUp( float AxisValue );
    void SetThrottleStop();
    void SetThrottleQuarter();
    void SetThrottleHalf();
    void SetThrottleThreeQuarter();
    void SetThrottleFull();
    void MainWeaponFire();

public:
	// Sets default values for this pawn's properties
	AStarfighterPawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	virtual void PostLoadSubobjects(FObjectInstancingGraph* OuterInstanceGraph) override;

    virtual UPawnMovementComponent* GetMovementComponent() const override;

public:
    UFUNCTION(BlueprintCallable, BlueprintPure, Category="Movement")
        bool SetAcceleration( float percent );

    UFUNCTION(BlueprintCallable, BlueprintPure, Category="Movement")
        float GetAcceleration( );

    UFUNCTION( BlueprintCallable, Category = "VR" )
        bool SetHeadHeight( float height );
};
