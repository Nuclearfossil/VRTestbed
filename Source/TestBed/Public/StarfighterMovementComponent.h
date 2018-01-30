// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PawnMovementComponent.h"
#include "StarfighterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class TESTBED_API UStarfighterMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()

/// Constants
private:
const float kMaxVelocity = 5000.0f;
const float kMaxDeltaV = 450.0f;
const float kMaxReverseDeltaV = -450.0f;

private:
    float mDesiredForwardAcceleration;
    float mActualForwardAcceleration;
    float mRightAcceleration;

public:
    enum InputState
    {
        AbsoluteForwardAcceleration = 0,
        CumulativeForwardAcceleration = 1,
        RightAcceleration = 2
    };

    // Non Virtual Methods
    void UpdateFromInput( InputState input, float value );
    float CurrentAcceleration();

    // Virtual Methods
    virtual void InitializeComponent() override;
    virtual void TickComponent( float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction ) override;
	
};
