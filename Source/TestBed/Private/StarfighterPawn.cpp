// Fill out your copyright notice in the Description page of Project Settings.

#include "TestBed.h"
#include "StarfighterPawn.h"

const FName kFocalPointName( TEXT( "Focal" ) );
const FName kUIVehicle( TEXT( "UI_Vehicle" ) );
#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White,text)

PRAGMA_DISABLE_OPTIMIZATION

// Sets default values
AStarfighterPawn::AStarfighterPawn()
{
    // Some data initializations
    mHeadHeight = 25.0f;
    // mHeadHeight = 18.0f;
    mEmitterCount = 0;

    // Stub out the components
    mSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
    mCockpitMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
    mWeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "WeaponMesh" ) );
    mBackgroundLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Backlight"));
    mChairNode = CreateDefaultSubobject<USceneComponent>(TEXT("chair"));
    mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    // mDebugReticle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DebugReticleMesh"));
    RootComponent = mSceneRoot;

    // Create the scene root + renderable mesh
    mSceneRoot->CreationMethod = EComponentCreationMethod::Native;

    mCockpitMesh->CreationMethod = EComponentCreationMethod::Native;
    mCockpitMesh->AttachToComponent(mSceneRoot, FAttachmentTransformRules::KeepRelativeTransform );
    mCockpitMesh->BodyInstance.SetCollisionProfileName(FName(TEXT("PhysicsActor")));
    mCockpitMesh->BodyInstance.bSimulatePhysics = false; // If we use physics, we need to send impulses, not move the object directly.
    mCockpitMesh->BodyInstance.bEnableGravity = false;
    mCockpitMesh->BodyInstance.LinearDamping = 1.000000f;
    mCockpitMesh->BodyInstance.AngularDamping = 1.000000f;

    // Create the Weapon mesh
    mWeaponMesh->CreationMethod = EComponentCreationMethod::Native;
    mWeaponMesh->AttachToComponent( mSceneRoot, FAttachmentTransformRules::KeepRelativeTransform );
    mWeaponMesh->BodyInstance.SetCollisionEnabled( ECollisionEnabled::NoCollision );
    mWeaponMesh->BodyInstance.bSimulatePhysics = false;
    mWeaponMesh->BodyInstance.bEnableGravity = false;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CockpitMeshAsset( TEXT("StaticMesh'/Game/Vehicles/Meshes/xwin-1.xwin-1'") );
    if ( CockpitMeshAsset.Succeeded() )
    {
        mCockpitMesh->SetStaticMesh( CastChecked<UStaticMesh>( CockpitMeshAsset.Object, ECastCheckedType::NullChecked ) );
        mCockpitMesh->RelativeScale3D = FVector( 8.0, 8.0, 8.0 );

        if ( !mCockpitMesh->IsTemplate() )
            mCockpitMesh->BodyInstance.FixupData( mCockpitMesh );
    }

    static ConstructorHelpers::FObjectFinder<UStaticMesh> WeaponMeshAsset( TEXT( "StaticMesh'/Game/Vehicles/Meshes/wings.wings'" ) );
    if ( WeaponMeshAsset.Succeeded() )
    {
        mWeaponMesh->SetStaticMesh( CastChecked<UStaticMesh>( WeaponMeshAsset.Object, ECastCheckedType::NullChecked ) );
        mWeaponMesh->RelativeScale3D = FVector( 8.0, 8.0, 8.0 );

        if ( mWeaponMesh->IsTemplate() )
            mWeaponMesh->BodyInstance.FixupData( mWeaponMesh );
    }

    // Chair represents where we're sitting
    // We will need to customize this a bit more between VR and FPS
    mChairNode->CreationMethod = EComponentCreationMethod::Native;
    mChairNode->AttachToComponent(mCockpitMesh, FAttachmentTransformRules::KeepRelativeTransform );
    mChairNode->RelativeLocation = FVector(-3.000000, 0.000000, mHeadHeight);
    mChairNode->RelativeScale3D = FVector(0.125000, 0.125000, 0.125000);
    mCamera->CreationMethod = EComponentCreationMethod::Native;
    mCamera->AttachToComponent(mChairNode, FAttachmentTransformRules::KeepRelativeTransform );
    mCamera->RelativeScale3D = FVector(2.000000, 2.000000, 2.000000);

    // Light our mesh, we are in space after all
    mBackgroundLight->CreationMethod = EComponentCreationMethod::Native;
    mBackgroundLight->AttachToComponent(mCockpitMesh, FAttachmentTransformRules::KeepRelativeTransform );
    mBackgroundLight->AttenuationRadius = 766.115967f;
    mBackgroundLight->LightGuid = FGuid(0x4BB0381F, 0x4ADD00DE, 0x489BF2A8, 0x1740F677);
    mBackgroundLight->LightColor = FColor(74, 99, 99, 255);
    mBackgroundLight->RelativeLocation = FVector(-17.500000, 7.125000, 26.750000);
    mBackgroundLight->RelativeScale3D = FVector(0.125000, 0.125000, 0.125000);

    // This is our mover.
    mMovementComponent = CreateDefaultSubobject<UStarfighterMovementComponent>( TEXT( "CustomMovementComponent" ) );
    mMovementComponent->UpdatedComponent = RootComponent;

    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Find all the weapon mounts - they're sockets on the wing mesh
    TArray<FComponentSocketDescription> sockets;
    mWeaponMesh->QuerySupportedSockets( sockets );

    mTargetPoint = nullptr;
    for ( size_t index = 0; index < sockets.Num(); index++ )
    {
        if ( sockets[index].Name == kFocalPointName )
            mTargetPoint = mWeaponMesh->GetSocketByName( kFocalPointName );
        else
            mLaserEmmiters.Add( mWeaponMesh->GetSocketByName( sockets[index].Name ) );
    }

    mEmitterCount = mLaserEmmiters.Num();
    mLastEmitterUsed = 0;

    FTransform targetLocation;
    mTargetPoint->GetSocketTransform( targetLocation, mWeaponMesh );
    mActualTargetPosition = targetLocation.GetLocation();

    // load up the laser BP
    // static ConstructorHelpers::FObjectFinder<UBlueprint> LaserBP( TEXT( "Blueprint'/Game/Projectiles/Lasers/Blueprints/LaserBolt.LaserBolt'" ) );
    // if ( LaserBP.Succeeded() )
    // {
    //     LaserProjectile = LaserBP.Object->GeneratedClass;
    // }

    // Debug Reticle
    // static ConstructorHelpers::FObjectFinder<UStaticMesh> DebugReticleMeshAsset( TEXT( "StaticMesh'/Game/UI/Reticle/Diamond01.Diamond01'" ) );
    // if ( DebugReticleMeshAsset.Succeeded() )
    // {
    //     mDebugReticle->SetStaticMesh( CastChecked<UStaticMesh>( DebugReticleMeshAsset.Object, ECastCheckedType::NullChecked ) );
    //     mDebugReticle->RelativeScale3D = FVector( 8.0, 8.0, 8.0 );
    //     if ( mDebugReticle->IsTemplate() )
    //         mDebugReticle->BodyInstance.FixupData( mDebugReticle );
    // }

    // mDebugReticle->AttachToComponent( mSceneRoot, FAttachmentTransformRules::KeepRelativeTransform );
    // mDebugReticle->BodyInstance.SetCollisionEnabled( ECollisionEnabled::NoCollision );
    // mDebugReticle->BodyInstance.bSimulatePhysics = false;
    // mDebugReticle->BodyInstance.bEnableGravity = false;
    // mDebugReticle->SetWorldLocation( mActualTargetPosition );

}

void AStarfighterPawn::PostLoadSubobjects( FObjectInstancingGraph* OuterInstanceGraph )
{
    Super::PostLoadSubobjects(OuterInstanceGraph);

    if (mSceneRoot != nullptr)
        mSceneRoot->CreationMethod = EComponentCreationMethod::Native;
    
    if (mCockpitMesh != nullptr)
        mCockpitMesh->CreationMethod = EComponentCreationMethod::Native;
    
    if (mBackgroundLight != nullptr)
        mBackgroundLight->CreationMethod = EComponentCreationMethod::Native;
    
    if (mChairNode != nullptr)
        mChairNode->CreationMethod = EComponentCreationMethod::Native;
    
    if (mCamera != nullptr)
        mCamera->CreationMethod = EComponentCreationMethod::Native;

    mYaw = 0.0f;
    mPitch = 0.0f;
    mRoll = 0.0f;
}

void AStarfighterPawn::MoveForward( float AxisValue )
{
    if ( mMovementComponent &&
        mMovementComponent->UpdatedComponent == RootComponent &&
        AxisValue != 0.0f )
    {
        mMovementComponent->UpdateFromInput( UStarfighterMovementComponent::CumulativeForwardAcceleration, AxisValue );
    }
}

bool AStarfighterPawn::SetAcceleration( float percent )
{
    bool result = false;
    if ( mMovementComponent &&
        mMovementComponent->UpdatedComponent == RootComponent )
    {
        mMovementComponent->UpdateFromInput( UStarfighterMovementComponent::AbsoluteForwardAcceleration, percent );
        result = true;
    }

    return result;
}

float AStarfighterPawn::GetAcceleration( )
{
    float result = 0.0f;
    if ( mMovementComponent &&
        mMovementComponent->UpdatedComponent == RootComponent )
    {
        result = mMovementComponent->CurrentAcceleration();
    }
    return result;
}

bool AStarfighterPawn::SetHeadHeight( float height )
{
    bool retval = false;
    mHeadHeight = height;

    if ( mChairNode != nullptr )
    {
        mChairNode->RelativeLocation = FVector(-3.000000, 0.000000, mHeadHeight);
        retval = true;
    }
    return retval;
}

void AStarfighterPawn::MoveRight( float AxisValue )
{
    if ( mMovementComponent &&
        mMovementComponent->UpdatedComponent == RootComponent &&
        AxisValue != 0.0f )
    {
        mMovementComponent->UpdateFromInput( UStarfighterMovementComponent::RightAcceleration, AxisValue );
    }
}

void AStarfighterPawn::LookUp( float AxisValue )
{
    if ( mMovementComponent &&
        mMovementComponent->UpdatedComponent == RootComponent &&
        AxisValue != 0.0f )
    {
        mPitch -= AxisValue;
    }
}

void AStarfighterPawn::Turn( float AxisValue )
{
    if ( mMovementComponent &&
        mMovementComponent->UpdatedComponent == RootComponent &&
        AxisValue != 0.0f )
    {
        mRoll -= AxisValue;
    }
}

// Called when the game starts or when spawned
void AStarfighterPawn::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void AStarfighterPawn::Tick( float DeltaTime )
{
    Super::Tick( DeltaTime );

    // Here's where we construct our movement
    FRotationMatrix rotationMatrix(mSceneRoot->RelativeRotation);

    FVector forward;
    FVector right;
    FVector up;

    // We probably don't need to normalize our vectors here, but I'm being safe
    rotationMatrix.GetScaledAxes( forward, right, up );
    forward.Normalize();
    right.Normalize();

    // Clamp mPitch and mRoll
    mPitch = FMath::Clamp( mPitch, -250.f, 250.f );
    mRoll = FMath::Clamp( mRoll, -250.f, 250.f );

    if ( FMath::IsNearlyZero(mPitch, 0.15f) )
    {
        mPitch = 0.0f;
    }
    if ( FMath::IsNearlyZero(mRoll, 0.15f) )
    {
        mRoll = 0.0f;
    }

    forward = forward.RotateAngleAxis( mPitch * .004f, right );
    right = right.RotateAngleAxis( mRoll * .004f, forward );
    up = FVector::CrossProduct( forward, right );
    up.Normalize();

    FMatrix rotMatrix( forward, right, up, FVector::ZeroVector );
    FRotator currentRotation = rotMatrix.Rotator();

    mSceneRoot->SetRelativeRotation( currentRotation );

    // Been debating on if this is the right way to pass in the input vector or 
    // if we just bind the sceen root to the movement component.
    // Using this for now.
    mMovementComponent->AddInputVector( currentRotation.Vector() );

    // Update the target position
    FTransform targetTransform;
    mTargetPoint->GetSocketTransform( targetTransform, mWeaponMesh );
    FVector targetLocation = targetTransform.GetLocation();
    FVector lastTargetLocation = mLastTargetTransform.GetLocation();
    
    FVector Velocity = (targetLocation - lastTargetLocation) * 0.3f / DeltaTime;
    float actualVelocity = Velocity.Size();
    actualVelocity = FMath::Clamp( actualVelocity, -1500.0f, 1500.0f );
    Velocity.Normalize();
    Velocity = Velocity * actualVelocity;
    mActualTargetPosition = targetLocation + Velocity;
    mLastTargetTransform = targetTransform;

    // mDebugReticle->SetWorldLocation( mActualTargetPosition );

    // Now that we've consumed the stored values, zero them all out.
    mYaw = 0.0f;
}

void AStarfighterPawn::SetThrottleStop()
{
    SetAcceleration( 0.0f );
}

void AStarfighterPawn::SetThrottleQuarter()
{
    SetAcceleration( 25.0f );
}

void AStarfighterPawn::SetThrottleHalf()
{
    SetAcceleration( 50.0f );
}

void AStarfighterPawn::SetThrottleThreeQuarter()
{
    SetAcceleration( 75.0f );
}

void AStarfighterPawn::SetThrottleFull()
{
    SetAcceleration( 100.0f );
}

void AStarfighterPawn::MainWeaponFire()
{
    FTransform emitterTransform;

    mLaserEmmiters[mLastEmitterUsed++]->GetSocketTransform( emitterTransform, mWeaponMesh );

    FVector emitterPosition = emitterTransform.GetLocation();
    FRotator emitterRotation = (mActualTargetPosition - emitterPosition).Rotation();

    ALaserProjectile* SpawnedLaserProjectile = (ALaserProjectile*)GetWorld()->SpawnActor( ALaserProjectile::StaticClass(), &emitterPosition, &emitterRotation );
    SpawnedLaserProjectile->EnableTick( true );

    if ( mLastEmitterUsed >= mEmitterCount )
        mLastEmitterUsed = 0;
}

// Called to bind functionality to input
void AStarfighterPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
    Super::SetupPlayerInputComponent(InputComponent);

    InputComponent->BindAxis( "MoveForward", this, &AStarfighterPawn::MoveForward );
    InputComponent->BindAxis( "MoveRight", this, &AStarfighterPawn::MoveRight );
    
    InputComponent->BindAxis( "LookUp", this, &AStarfighterPawn::LookUp );
    InputComponent->BindAxis( "Turn", this, &AStarfighterPawn::Turn );

    InputComponent->BindAction( "Throttle_0", IE_Released, this, &AStarfighterPawn::SetThrottleStop );
    InputComponent->BindAction( "Throttle_25", IE_Released, this, &AStarfighterPawn::SetThrottleQuarter );
    InputComponent->BindAction( "Throttle_50", IE_Released, this, &AStarfighterPawn::SetThrottleHalf );
    InputComponent->BindAction( "Throttle_75", IE_Released, this, &AStarfighterPawn::SetThrottleThreeQuarter );
    InputComponent->BindAction( "Throttle_100", IE_Released, this, &AStarfighterPawn::SetThrottleFull );

    InputComponent->BindAction( "MainWeaponFire", IE_Released, this, &AStarfighterPawn::MainWeaponFire );
}

UPawnMovementComponent* AStarfighterPawn::GetMovementComponent() const
{
    return mMovementComponent;
}

PRAGMA_ENABLE_OPTIMIZATION