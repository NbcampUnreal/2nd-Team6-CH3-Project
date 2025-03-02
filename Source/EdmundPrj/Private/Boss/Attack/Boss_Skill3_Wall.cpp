#include "Boss/Attack/Boss_Skill3_Wall.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"

TArray<ABoss_Skill3_Wall*> ABoss_Skill3_Wall::WallPool;

ABoss_Skill3_Wall::ABoss_Skill3_Wall()
{
    PrimaryActorTick.bCanEverTick = true;
    Tags.Add("Skill3Wall");
    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
    RootComponent = MeshComp;

    MeshComp->SetSimulatePhysics(false);
    MeshComp->SetEnableGravity(false);
    MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    MeshComp->SetCollisionObjectType(ECC_PhysicsBody);
    MeshComp->SetCollisionResponseToAllChannels(ECR_Block);

    GravityAcceleration = 980.f;
    CurrentVelocity = FVector::ZeroVector;

    bIsActive = false;

    SetActorHiddenInGame(true);
    SetActorEnableCollision(false);
}

void ABoss_Skill3_Wall::BeginPlay()
{
    Super::BeginPlay();

    if (!WallPool.Contains(this))
    {
        WallPool.Add(this);
    }
}

void ABoss_Skill3_Wall::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

ABoss_Skill3_Wall* ABoss_Skill3_Wall::GetWallFromPool(UWorld* World, TSubclassOf<ABoss_Skill3_Wall> WallClass)
{
    for (ABoss_Skill3_Wall* Wall : WallPool)
    {
        if (Wall && !Wall->bIsActive)
        {
            return Wall;
        }
    }
    if (World)
    {
        ABoss_Skill3_Wall* NewWall = World->SpawnActor<ABoss_Skill3_Wall>(WallClass);
        if (NewWall)
        {
            WallPool.Add(NewWall);
            return NewWall;
        }
    }
    return nullptr;
}

void ABoss_Skill3_Wall::ActivateWall(FVector SpawnLocation, FRotator SpawnRotation)
{
    bIsActive = true;
    SetActorLocation(SpawnLocation);
    SetActorRotation(SpawnRotation);
    SetActorHiddenInGame(false);
    SetActorEnableCollision(true);
    CurrentVelocity = FVector::ZeroVector;
    if (!GetWorldTimerManager().IsTimerActive(GravityTimerHandle))
    {
        GetWorldTimerManager().SetTimer(GravityTimerHandle, this, &ABoss_Skill3_Wall::SimulateGravity, 0.02f, true);
    }
}

void ABoss_Skill3_Wall::DeactivateWall()
{
    bIsActive = false;
    SetActorHiddenInGame(true);
    SetActorEnableCollision(false);
    GetWorldTimerManager().ClearTimer(GravityTimerHandle);
    CurrentVelocity = FVector::ZeroVector;
}

void ABoss_Skill3_Wall::SimulateGravity()
{
    const float DeltaTime = 0.02f;
    CurrentVelocity.Z -= GravityAcceleration * DeltaTime;
    FVector NewLocation = GetActorLocation() + CurrentVelocity * DeltaTime;
    if (NewLocation.Z <= 0.f)
    {
        NewLocation.Z = 0.f;
        SetActorLocation(NewLocation);
        GetWorldTimerManager().ClearTimer(GravityTimerHandle);
        return;
    }
    SetActorLocation(NewLocation);
}

