#include "Boss/Attack/Boss_Skill3_Wall.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"

TArray<ABoss_Skill3_Wall*> ABoss_Skill3_Wall::WallPool;

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

    bIsActive = false;

    // 초기에는 숨기고 충돌 비활성화
    SetActorHiddenInGame(true);
    SetActorEnableCollision(false);
}

void ABoss_Skill3_Wall::BeginPlay()
{
    Super::BeginPlay();

    // 풀에 등록 (이미 등록되어 있지 않으면 추가)
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
    // 사용 중이지 않은 벽을 반환
    for (ABoss_Skill3_Wall* Wall : WallPool)
    {
        if (Wall && !Wall->bIsActive)
        {
            return Wall;
        }
    }
    // 없으면 새로 생성
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
    // 초기화: 중력 효과 시작
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

    // 지면에 도달하면 위치를 고정하고 중력 시뮬레이션을 멈춥니다.
    if (NewLocation.Z <= 0.f)
    {
        NewLocation.Z = 0.f;
        SetActorLocation(NewLocation);
        GetWorldTimerManager().ClearTimer(GravityTimerHandle);
        // DeactivateWall() 대신 벽을 그대로 유지
        return;
    }
    SetActorLocation(NewLocation);
}

