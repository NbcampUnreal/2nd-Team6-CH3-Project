#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossAttack4Bullet.generated.h"

UCLASS()
class EDMUNDPRJ_API ABossAttack4Bullet : public AActor
{
    GENERATED_BODY()

public:
    ABossAttack4Bullet();

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(VisibleAnywhere, Category = "Components")
    UStaticMeshComponent* BulletMesh;

    UPROPERTY(EditAnywhere, Category = "Movement")
    float Speed = 500.0f;

    UPROPERTY(EditAnywhere, Category = "Tracking")
    float TrackingStrength = 2.0f;

    FTimerHandle TrackingTimerHandle;
    FTimerHandle LifeSpanHandle;

    void MoveTowardsPlayer();
    void DestroyBullet();

    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
