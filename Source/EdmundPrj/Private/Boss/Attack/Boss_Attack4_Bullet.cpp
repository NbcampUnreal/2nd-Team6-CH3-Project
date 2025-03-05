#include "Boss/Attack/Boss_Attack4_Bullet.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h" 
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/World.h"
#include "Boss/Boss.h"
#include "Player/BaseCharacter.h"
#include "TimerManager.h"

TArray<ABoss_Attack4_Bullet*> ABoss_Attack4_Bullet::Bullet4Pool;

ABoss_Attack4_Bullet::ABoss_Attack4_Bullet()
{
    PrimaryActorTick.bCanEverTick = true;

    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
    CollisionComp->InitSphereRadius(10.0f);
    CollisionComp->SetCollisionProfileName(TEXT("IgnoreAll"));
    CollisionComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
    CollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    CollisionComp->BodyInstance.bUseCCD = true;
    RootComponent = CollisionComp;
    CollisionComp->OnComponentHit.AddDynamic(this, &ABoss_Attack4_Bullet::OnHit);
    CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ABoss_Attack4_Bullet::OnOverlapBegin);

    BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
    BulletMesh->SetupAttachment(RootComponent);
    BulletMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    ExplosionEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ExplosionEffect"));
    ExplosionEffect->SetupAttachment(RootComponent);
    ExplosionEffect->bAutoActivate = false;

    Attack4Niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Attack4Niagara"));
    Attack4Niagara->SetupAttachment(RootComponent);
    Attack4Niagara->bAutoActivate = false;

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

    bIsActive = false;
    BulletSpeed = 2000.f;
    MaxDistance = 5000.f;
    ExplosionDelay = 1.0f;
    HomingDuration = 5.0f;
    HomingTarget = nullptr;
    bIsHoming = false;
    HomingStartTime = 0.0f;
    TraveledDistance = 0.0f;

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
        ProjectileMovement->Velocity = Direction * BulletSpeed;
        ProjectileMovement->Activate(true);

        AActor* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
        if (Player)
        {
            ACharacter* PlayerCharacter = Cast<ACharacter>(Player);
            if (PlayerCharacter && PlayerCharacter->GetCapsuleComponent())
            {
                ProjectileMovement->bIsHomingProjectile = true;
                ProjectileMovement->HomingTargetComponent = PlayerCharacter->GetCapsuleComponent();
            }
            else
            {
                ProjectileMovement->bIsHomingProjectile = false;
            }
        }
        else
        {
            ProjectileMovement->bIsHomingProjectile = false;
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

void ABoss_Attack4_Bullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (!bIsActive) return;

    if (OtherActor && OtherActor != this && !OtherActor->IsA(ABoss_Attack4_Bullet::StaticClass()))
    {
        if (OtherActor->ActorHasTag("Player") || OtherActor->ActorHasTag("NPC") || OtherActor->ActorHasTag("Ground"))
        {
            Explode();
        }
    }
}


void ABoss_Attack4_Bullet::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!bIsActive) return;

    if (OtherActor->ActorHasTag("NPC") || OtherActor->ActorHasTag("Player") || OtherActor->ActorHasTag("Ground"))
    {
        if (OtherActor->ActorHasTag("Player"))
        {
            AActor* LocalOwner = OverlappedComp->GetOwner();
            ABoss_Attack4_Bullet* Bullet = Cast<ABoss_Attack4_Bullet>(LocalOwner);

            if (Bullet)
            {
                ABoss* BossRef = Cast<ABoss>(GetOwner());
                if (BossRef)
                {
                    float DamageValue = BossRef->GetMonsterAttackDamage() * BossRef->GetAttack4Multiplier();
                    UGameplayStatics::ApplyDamage(OtherActor, DamageValue, nullptr, Bullet, UDamageType::StaticClass());
                }
            }
        }
        Explode();
    }
}
