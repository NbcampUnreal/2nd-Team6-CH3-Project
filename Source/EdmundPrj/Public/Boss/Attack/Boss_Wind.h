#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Boss_Wind.generated.h"

UCLASS()
class EDMUNDPRJ_API ABoss_Wind : public AActor
{
    GENERATED_BODY()

public:
    ABoss_Wind();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere)
    class USphereComponent* CollisionComponent;

    UPROPERTY(VisibleAnywhere)
    class UStaticMeshComponent* MeshComponent;

    UPROPERTY(VisibleAnywhere)
    class UParticleSystemComponent* ParticleSystemComponent;

    UPROPERTY(EditAnywhere, Category = "Effects")
    class UParticleSystem* PushParticleEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Forces")
    float PushForce;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Forces")
    float LaunchForce;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Forces")
    bool bIsPushForceEnabled;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Forces")
    bool bIsLaunchForceEnabled;


    void ApplyPushForce(AActor* HitActor);
    void ApplyLaunchForce(AActor* HitActor);
};
