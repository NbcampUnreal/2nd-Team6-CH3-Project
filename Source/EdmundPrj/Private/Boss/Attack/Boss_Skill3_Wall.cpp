#include "Boss/Attack/Boss_Skill3_Wall.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"

ABoss_Skill3_Wall::ABoss_Skill3_Wall()
{
    PrimaryActorTick.bCanEverTick = true;

    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
    RootComponent = MeshComp;

    MeshComp->SetSimulatePhysics(false);
    MeshComp->SetEnableGravity(false);
    MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    MeshComp->SetCollisionObjectType(ECC_PhysicsBody);
    MeshComp->SetCollisionResponseToAllChannels(ECR_Block);

    GravityAcceleration = 980.f;
    CurrentVelocity = FVector::ZeroVector;
}

void ABoss_Skill3_Wall::BeginPlay()
{
    Super::BeginPlay();

    GetWorldTimerManager().SetTimer(GravityTimerHandle, this, &ABoss_Skill3_Wall::SimulateGravity, 0.02f, true);
}

void ABoss_Skill3_Wall::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABoss_Skill3_Wall::SimulateGravity()
{
    const float DeltaTime = 0.02f;

    CurrentVelocity.Z -= GravityAcceleration * DeltaTime;

    FVector NewLocation = GetActorLocation() + CurrentVelocity * DeltaTime;

    if (NewLocation.Z <= 0.f)
    {
        NewLocation.Z = 0.f;
        GetWorldTimerManager().ClearTimer(GravityTimerHandle);
    }

    SetActorLocation(NewLocation);
}
