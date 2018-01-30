// Fill out your copyright notice in the Description page of Project Settings.

#include "TestBed.h"
#include "StarfighterMovementComponent.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White,text)
#define min(a, b) ((a < b) ? a : b)

void UStarfighterMovementComponent::UpdateFromInput( InputState input, float value )
{
    switch ( input )
    {
    case UStarfighterMovementComponent::CumulativeForwardAcceleration:
        mDesiredForwardAcceleration += kMaxVelocity * value/100.0f;
        break;
    case UStarfighterMovementComponent::AbsoluteForwardAcceleration:
        mDesiredForwardAcceleration = kMaxVelocity * value/100.0f;
        break;
    case UStarfighterMovementComponent::RightAcceleration:
        mRightAcceleration += value;
        break;
    default:
        break;
    }
}

void UStarfighterMovementComponent::InitializeComponent()
{
    mDesiredForwardAcceleration = 0.0f;
    mActualForwardAcceleration = 0.0f;

    Super::InitializeComponent();
}

float UStarfighterMovementComponent::CurrentAcceleration()
{
    return mActualForwardAcceleration;
}

void UStarfighterMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Make sure that everything is still valid, and that we are allowed to move.
    if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
        return;

    // Get (and then clear) the movement vector that we set in ACollidingPawn::Tick
    if ( mDesiredForwardAcceleration != mActualForwardAcceleration )
    {
        if ( FMath::Abs(mDesiredForwardAcceleration - mActualForwardAcceleration) <= (kMaxDeltaV * DeltaTime) )
        {
            mActualForwardAcceleration = mDesiredForwardAcceleration;
        }
        else
        {
            if ( mDesiredForwardAcceleration > mActualForwardAcceleration )
            {
                mActualForwardAcceleration += kMaxDeltaV * DeltaTime;
            }
            else
            {
                mActualForwardAcceleration += kMaxReverseDeltaV * DeltaTime;
            }
        }

        if ( mActualForwardAcceleration < 0.0f )
        {
            mActualForwardAcceleration = mDesiredForwardAcceleration = 0.0f;
        }

        if ( mActualForwardAcceleration > kMaxVelocity )
        {
            mActualForwardAcceleration = mDesiredForwardAcceleration = kMaxVelocity;
        }
    }

    FVector DesiredMovementThisFrame = ConsumeInputVector().GetClampedToMaxSize(1.0f) * DeltaTime * min(mActualForwardAcceleration, kMaxVelocity);
    if (!DesiredMovementThisFrame.IsNearlyZero())
    {
        FHitResult Hit;
        SafeMoveUpdatedComponent(DesiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, Hit);

        // If we bumped into something, try to slide along it
        if (Hit.IsValidBlockingHit())
        {
            SlideAlongSurface(DesiredMovementThisFrame, 1.f - Hit.Time, Hit.Normal, Hit);
        }
    }
};
