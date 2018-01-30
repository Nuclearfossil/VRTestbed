// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "UIVelocityComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTBED_API UUIVelocityComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USceneComponent* mSceneRoot;
        USkeletalMeshComponent* mDrawable;

    UAnimSequence* mAnim;

    // Sets default values for this component's properties
    UUIVelocityComponent();

    // Called when the game starts
    virtual void BeginPlay() override;

    // Called every frame
    virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;
};
