#include "Boss/Attack/Boss_Wind.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/PlayerController.h"

ABoss_Wind::ABoss_Wind()
{
    PrimaryActorTick.bCanEverTick = true;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    RootComponent = CollisionComponent;
    CollisionComponent->InitSphereRadius(200.0f);

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(RootComponent);

    ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
    ParticleSystemComponent->SetupAttachment(RootComponent);

    PushForce = 1000.0f;
    LaunchForce = 500.0f;
    bIsPushForceEnabled = true;
    bIsLaunchForceEnabled = false;
}

void ABoss_Wind::BeginPlay()
{
    Super::BeginPlay();
}

void ABoss_Wind::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    TArray<AActor*> OverlappingActors;
    CollisionComponent->GetOverlappingActors(OverlappingActors);

    for (AActor* Actor : OverlappingActors)
    {
        if (bIsPushForceEnabled)
        {
            ApplyPushForce(Actor);
        }

        if (bIsLaunchForceEnabled)
        {
            ApplyLaunchForce(Actor);
        }
    }
}

void ABoss_Wind::ApplyPushForce(AActor* HitActor)
{
    /*FVector PushDirection = (HitActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
    FVector Impulse = PushDirection * PushForce;

    UPrimitiveComponent* RootPrimitive = Cast<UPrimitiveComponent>(HitActor->GetRootComponent());
    if (RootPrimitive)
    {
        RootPrimitive->AddImpulse(Impulse, NAME_None, true);
    }*/

    FVector PushDirection = (HitActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
    FVector PushAmount = PushDirection * PushForce; 
    if (PushParticleEffect && !ParticleSystemComponent->IsActive())
    {
        ParticleSystemComponent->ActivateSystem();
    }
    HitActor->SetActorLocation(HitActor->GetActorLocation() + PushAmount);
}

// 위로
void ABoss_Wind::ApplyLaunchForce(AActor* HitActor)
{
    //FVector LaunchDirection = FVector(0, 0, 1); 
    //FVector Impulse = LaunchDirection * LaunchForce;

    //UPrimitiveComponent* RootPrimitive = Cast<UPrimitiveComponent>(HitActor->GetRootComponent());
    //if (RootPrimitive)
    //{
    //    RootPrimitive->AddImpulse(Impulse, NAME_None, true);
    //}

    FVector LaunchDirection = FVector(0, 0, 1);
    FVector LaunchAmount = LaunchDirection * LaunchForce;
    if (PushParticleEffect && !ParticleSystemComponent->IsActive())
    {
        ParticleSystemComponent->ActivateSystem();
    }
    HitActor->SetActorLocation(HitActor->GetActorLocation() + LaunchAmount);
}
