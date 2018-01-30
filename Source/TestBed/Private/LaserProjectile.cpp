// Fill out your copyright notice in the Description page of Project Settings.

#include "TestBed.h"
#include "LaserProjectile.h"


// Sets default values
ALaserProjectile::ALaserProjectile()
{
    mSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
    mLaserMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LaserMesh"));
    mProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>( TEXT( "LaserMovementComponent" ) );

    // Create the Weapon mesh
    mSceneRoot->CreationMethod = EComponentCreationMethod::Native;
    mLaserMesh->CreationMethod = EComponentCreationMethod::Instance;
    mProjectileMovementComponent->CreationMethod = EComponentCreationMethod::Instance;

    mLaserMesh->AttachToComponent( mSceneRoot, FAttachmentTransformRules::KeepRelativeTransform );
    mLaserMesh->BodyInstance.SetCollisionEnabled( ECollisionEnabled::NoCollision );
    mLaserMesh->BodyInstance.bSimulatePhysics = false;
    mLaserMesh->BodyInstance.bEnableGravity = false;

    // Laser Mesh reference: StaticMesh'/Game/Projectiles/Lasers/Meshes/laser01.laser01'
    static ConstructorHelpers::FObjectFinder<UStaticMesh> LaserMeshAsset( TEXT("StaticMesh'/Game/Projectiles/Lasers/Meshes/laser01.laser01'") );
    if ( LaserMeshAsset.Succeeded() )
    {
        mLaserMesh->SetStaticMesh( CastChecked<UStaticMesh>( LaserMeshAsset.Object, ECastCheckedType::NullChecked ) );
        mLaserMesh->RelativeScale3D = FVector( 8.0, 8.0, 8.0 );

        if ( !mLaserMesh->IsTemplate() )
            mLaserMesh->BodyInstance.FixupData( mLaserMesh );
    }

    mProjectileMovementComponent->ProjectileGravityScale = 0.0f;
    mProjectileMovementComponent->InitialSpeed = 98000.0f;

    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
    RootComponent = mSceneRoot;
}

// Called when the game starts or when spawned
void ALaserProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALaserProjectile::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ALaserProjectile::EnableTick( bool enable )
{
    PrimaryActorTick.bCanEverTick = enable;
}