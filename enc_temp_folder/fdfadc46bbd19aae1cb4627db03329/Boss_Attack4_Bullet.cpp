#include "Boss/Attack/Boss_Attack4_Bullet.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h" // 추가
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h" // ACharacter 사용
#include "Engine/World.h"
#include "TimerManager.h"

TArray<ABoss_Attack4_Bullet*> ABoss_Attack4_Bullet::Bullet4Pool;

ABoss_Attack4_Bullet::ABoss_Attack4_Bullet()
{
    PrimaryActorTick.bCanEverTick = true;

    // 충돌 컴포넌트 생성 및 설정
    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
    CollisionComp->InitSphereRadius(10.0f);
    CollisionComp->SetCollisionProfileName(TEXT("BlockAll"));
    // 불릿끼리 충돌하지 않도록, 예: 불릿 전용 채널은 Ignore 처리 (프로젝트 설정에 따라 다름)
    CollisionComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
    // Pawn 채널은 Overlap 처리하여 캐릭터와 충돌 시 폭발하도록 함
    CollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    CollisionComp->BodyInstance.bUseCCD = true;
    RootComponent = CollisionComp;

    // OnHit 및 OnOverlapBegin 바인딩
    CollisionComp->OnComponentHit.AddDynamic(this, &ABoss_Attack4_Bullet::OnHit);
    CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ABoss_Attack4_Bullet::OnOverlapBegin);

    // 총알 메쉬 컴포넌트 생성 및 설정
    BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
    BulletMesh->SetupAttachment(RootComponent);
    BulletMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // 폭발 파티클 이펙트 컴포넌트 생성 및 설정
    ExplosionEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ExplosionEffect"));
    ExplosionEffect->SetupAttachment(RootComponent);
    ExplosionEffect->bAutoActivate = false;

    // ProjectileMovementComponent 생성 및 기본 설정 (내장 호밍 사용)
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    if (ProjectileMovement)
    {
        ProjectileMovement->UpdatedComponent = CollisionComp;
        ProjectileMovement->InitialSpeed = 2000.f;
        ProjectileMovement->MaxSpeed = 2000.f;
        ProjectileMovement->bRotationFollowsVelocity = true;
        ProjectileMovement->bIsHomingProjectile = false;
        ProjectileMovement->HomingAccelerationMagnitude = 6000.f;
    }

    // 기본 변수 초기화
    bIsActive = false;
    BulletSpeed = 2000.f;
    MaxDistance = 5000.f;
    ExplosionDelay = 1.0f;
    HomingDuration = 5.0f;
    HomingTarget = nullptr;
    bIsHoming = false;
    HomingStartTime = 0.0f;
    TraveledDistance = 0.0f;

    // 초기 상태: 숨김 및 충돌 비활성화
    SetActorHiddenInGame(true);
    SetActorEnableCollision(false);
}

void ABoss_Attack4_Bullet::BeginPlay()
{
    Super::BeginPlay();
    Bullet4Pool.Add(this);
}

void ABoss_Attack4_Bullet::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsActive)
    {
        // 만약 ProjectileMovementComponent의 호밍 기능이 활성화되었으면 내장 업데이트에 맡기고,
        // 그렇지 않으면 직접 위치 업데이트 (직선 이동)
        if (!ProjectileMovement->bIsHomingProjectile)
        {
            FVector NewLocation = GetActorLocation() + GetActorForwardVector() * BulletSpeed * DeltaTime;
            SetActorLocation(NewLocation, true);
            TraveledDistance += BulletSpeed * DeltaTime;
            if (TraveledDistance >= MaxDistance)
            {
                Explode();
            }
        }
    }
}

void ABoss_Attack4_Bullet::FireProjectile(FVector SpawnLocation, FRotator SpawnRotation, FVector Direction)
{
    // 초기 위치 및 회전 설정
    SetActorLocation(SpawnLocation);
    SetActorRotation(SpawnRotation);
    bIsActive = true;
    TraveledDistance = 0.0f;
    SetActorHiddenInGame(false);
    SetActorEnableCollision(true);

    if (ProjectileMovement)
    {
        ProjectileMovement->StopMovementImmediately();
        ProjectileMovement->SetUpdatedComponent(CollisionComp);
        ProjectileMovement->InitialSpeed = BulletSpeed;
        ProjectileMovement->MaxSpeed = BulletSpeed;
        // 설정한 방향에 따른 초기 속도
        ProjectileMovement->Velocity = Direction * BulletSpeed;
        ProjectileMovement->Activate(true);
        UE_LOG(LogTemp, Log, TEXT("Attack4Bullet: Velocity set to %s"), *ProjectileMovement->Velocity.ToString());

        // 플레이어 캐릭터의 CapsuleComponent를 호밍 타겟으로 설정
        AActor* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
        if (Player)
        {
            ACharacter* PlayerCharacter = Cast<ACharacter>(Player);
            if (PlayerCharacter && PlayerCharacter->GetCapsuleComponent())
            {
                ProjectileMovement->bIsHomingProjectile = true;
                ProjectileMovement->HomingTargetComponent = PlayerCharacter->GetCapsuleComponent();
                UE_LOG(LogTemp, Log, TEXT("Attack4Bullet: Homing target set to %s (CapsuleComponent)"), *Player->GetName());
            }
            else
            {
                ProjectileMovement->bIsHomingProjectile = false;
                UE_LOG(LogTemp, Warning, TEXT("Attack4Bullet: Player's CapsuleComponent not found"));
            }
        }
        else
        {
            ProjectileMovement->bIsHomingProjectile = false;
            UE_LOG(LogTemp, Warning, TEXT("Attack4Bullet: Player is NULL"));
        }
        bIsHoming = ProjectileMovement->bIsHomingProjectile;
        HomingStartTime = GetWorld()->GetTimeSeconds();
    }
}

void ABoss_Attack4_Bullet::Explode()
{
    if (!bIsActive)
        return;

    bIsActive = false;
    SetActorHiddenInGame(true);
    SetActorEnableCollision(false);

    if (ExplosionEffect->Template)
    {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect->Template, GetActorLocation());
    }
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ABoss_Attack4_Bullet::ResetBullet, ExplosionDelay, false);
}

void ABoss_Attack4_Bullet::ResetBullet()
{
    bIsActive = false;
    TraveledDistance = 0.0f;
    SetActorHiddenInGame(true);
    SetActorEnableCollision(false);
    if (ProjectileMovement)
    {
        ProjectileMovement->StopMovementImmediately();
        ProjectileMovement->Deactivate();
        ProjectileMovement->bIsHomingProjectile = false;
        ProjectileMovement->HomingTargetComponent = nullptr;
    }
    if (!Bullet4Pool.Contains(this))
    {
        Bullet4Pool.Add(this);
    }
}

ABoss_Attack4_Bullet* ABoss_Attack4_Bullet::GetBulletFromPool(UWorld* World, TSubclassOf<ABoss_Attack4_Bullet> BulletClass)
{
    for (ABoss_Attack4_Bullet* Bullet4 : Bullet4Pool)
    {
        if (Bullet4 && !Bullet4->bIsActive)
        {
            return Bullet4;
        }
    }
    if (World)
    {
        ABoss_Attack4_Bullet* NewBullet = World->SpawnActor<ABoss_Attack4_Bullet>(BulletClass);
        if (NewBullet)
        {
            Bullet4Pool.Add(NewBullet);
            return NewBullet;
        }
    }
    return nullptr;
}

void ABoss_Attack4_Bullet::StopHomingAndExplode()
{
    if (ProjectileMovement)
    {
        ProjectileMovement->bIsHomingProjectile = false;
        ProjectileMovement->HomingTargetComponent = nullptr;
    }
    bIsHoming = false;
    if (bIsActive)
    {
        Explode();
    }
}

// Collision 콜백 추가: 캐릭터나 액터와 충돌 시 폭발 처리하고, 불릿끼리의 충돌은 무시
void ABoss_Attack4_Bullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (!bIsActive) return;

    // 만약 OtherActor가 유효하고, 자신과 같은 불릿이 아니며 (예를 들어, 클래스가 다름) 
    // 그리고 OtherActor가 플레이어나 다른 유효한 대상이면 폭발 처리
    if (OtherActor && OtherActor != this && !OtherActor->IsA(ABoss_Attack4_Bullet::StaticClass()))
    {
        Explode();
    }
}

void ABoss_Attack4_Bullet::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!bIsActive) return;

    if (OtherActor && OtherActor != this && !OtherActor->IsA(ABoss_Attack4_Bullet::StaticClass()))
    {
        Explode();
    }
}
