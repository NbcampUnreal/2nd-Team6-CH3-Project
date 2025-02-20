#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossAttack1Bullet.generated.h"

UCLASS()
class EDMUNDPRJ_API ABossAttack1Bullet : public AActor
{
    GENERATED_BODY()

public:
    ABossAttack1Bullet();

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(VisibleAnywhere, Category = "Components")
    UStaticMeshComponent* BulletMesh;

    UPROPERTY(EditAnywhere, Category = "Movement")
    float Speed = 1000.0f;

    FVector Direction;

    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    UFUNCTION()
    void MoveBullet();

    void OnDestroyTimerExpired();

    FTimerHandle MoveTimerHandle;
    FTimerHandle DestroyTimerHandle;

public:
    void Initialize(FVector TargetLocation);
};
