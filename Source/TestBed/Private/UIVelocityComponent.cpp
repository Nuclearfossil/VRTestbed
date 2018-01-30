// Fill out your copyright notice in the Description page of Project Settings.

#include "TestBed.h"
#include "UIVelocityComponent.h"

PRAGMA_DISABLE_OPTIMIZATION

// Sets default values for this component's properties
UUIVelocityComponent::UUIVelocityComponent()
{
    mSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
    mDrawable = CreateDefaultSubobject<USkeletalMeshComponent>( TEXT( "UIVelocity" ) );

    mDrawable->CreationMethod = EComponentCreationMethod::Native;
    mDrawable->AttachToComponent( mSceneRoot, FAttachmentTransformRules::KeepRelativeTransform );

    // Load up the skeletal mesh
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshContainer( TEXT( "SkeletalMesh'/Game/UI/InGame/VelocityUI.VelocityUI'" ) );
    if ( MeshContainer.Succeeded() )
    {
        static ConstructorHelpers::FObjectFinder<USkeleton> Skeleton( TEXT( "Skeleton'/Game/UI/InGame/VelocityUI_Skeleton.VelocityUI_Skeleton'" ) );
        if (Skeleton.Succeeded())
        {
            MeshContainer.Object->Skeleton = Skeleton.Object;
        }
        mDrawable->SetSkeletalMesh( CastChecked<USkeletalMesh>( MeshContainer.Object, ECastCheckedType::NullChecked ) );
    }

    // And the Animation component
    static ConstructorHelpers::FObjectFinder<UAnimSequence> anim( TEXT( "AnimSequence'/Game/UI/InGame/VelocityUI_Anim.VelocityUI_Anim'" ) );
    if ( anim.Succeeded() )
    {
        mAnim = anim.Object;
    }

    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UUIVelocityComponent::BeginPlay()
{
    Super::BeginPlay();

    if ( mAnim != nullptr )
    {
        mDrawable->SetAnimation( mAnim );
        mDrawable->SetPlayRate( 1.0f );
        mDrawable->Play( true );
    }
}


// Called every frame
void UUIVelocityComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
    Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
    mDrawable->TickPose( DeltaTime, false );
}

PRAGMA_ENABLE_OPTIMIZATION
