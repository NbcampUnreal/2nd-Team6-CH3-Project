#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Boss_Attack4_Bullet.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UParticleSystemComponent;
class UProjectileMovementComponent;

UCLASS()
class EDMUNDPRJ_API ABoss_Attack4_Bullet : public AActor
{
    GENERATED_BODY()

public:
    ABoss_Attack4_Bullet();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Bullet")
    virtual void FireProjectile(FVector SpawnLocation, FRotator SpawnRotation, FVector Direction);

    UFUNCTION(BlueprintCallable, Category = "Bullet")
    virtual void Explode();

    UFUNCTION(BlueprintCallable, Category = "Bullet")
    virtual void ResetBullet();

    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    static ABoss_Attack4_Bullet* GetBulletFromPool(UWorld* World, TSubclassOf<ABoss_Attack4_Bullet> BulletClass);
    static TArray<ABoss_Attack4_Bullet*> Bullet4Pool;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    bool bIsActive;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
    float BulletSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
    float MaxDistance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    float ExplosionDelay;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USphereComponent* CollisionComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* BulletMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects")
    UParticleSystemComponent* ExplosionEffect;

    float TraveledDistance;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    UProjectileMovementComponent* ProjectileMovement;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack4|Homing")
    float HomingDuration;

    float HomingStartTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack4|Homing")
    AActor* HomingTarget;

    bool bIsHoming;

private:
    void StopHomingAndExplode();
};
