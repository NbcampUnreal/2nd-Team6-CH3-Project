#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Boss_Attack1_Bullet.generated.h"

UCLASS()
class EDMUNDPRJ_API ABoss_Attack1_Bullet : public AActor
{
    GENERATED_BODY()

public:
    ABoss_Attack1_Bullet();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    void FireProjectile(FVector SpawnLocation, FRotator SpawnRotation, FVector Direction);

    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, FVector NormalImpulse,
        const FHitResult& Hit);

    static ABoss_Attack1_Bullet* GetBulletFromPool(UWorld* World, TSubclassOf<ABoss_Attack1_Bullet> BullectClass);

    void ActivateBullet();

    bool IsActive() const { return bIsActive; }

    static TArray<ABoss_Attack1_Bullet*> BulletPool;

private:

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UStaticMeshComponent* BulletMesh;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    USphereComponent* ShpereComp;

    UPROPERTY(EditDefaultsOnly, Category = "Movement")
    float BulletSpeed = 1200.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Lifetime")
    float MaxDistance = 3000.0f;

    float TraveledDistance = 0.0f;

    bool bIsActive = false;
};
