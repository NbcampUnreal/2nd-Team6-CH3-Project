#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Boss_Attack1_Bullet.generated.h"

class USphereComponent;
class UBoxComponent;
class UStaticMeshComponent;
class UParticleSystemComponent;
class UProjectileMovementComponent;
class ABoss;

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

    /** 총알이 충돌했을 때 */
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    /** B콜리전으로 추가 데미지 적용 */
    void ApplyBCollisionDamage();

    /** 데미지 적용 */
    void ApplyDamage(AActor* Target, float DamageAmount);

    /** B콜리전 비활성화 */
    void DeactivateBCollision();

    /** 총알 리셋 및 풀링 반환 */
    void ResetBullet();

    /** 풀에서 총알 가져오기 */
    static ABoss_Attack1_Bullet* GetBulletFromPool(UWorld* World, TSubclassOf<ABoss_Attack1_Bullet> BulletClass);

    void FireProjectile(FVector SpawnLocation, FRotator SpawnRotation, FVector Direction);

    /** 총알 풀 (Static 변수) */
    static TArray<ABoss_Attack1_Bullet*> BulletPool;

private:
    /** A콜리전 (기존) */
    UPROPERTY(VisibleAnywhere, Category = "Components")
    USphereComponent* ACollision;

    /** B콜리전 (새롭게 추가) */
    UPROPERTY(VisibleAnywhere, Category = "Components")
    UBoxComponent* BCollision;

    /** 총알 메시 */
    UPROPERTY(VisibleAnywhere, Category = "Components")
    UStaticMeshComponent* BulletMesh;

    /** 폭발 파티클 */
    UPROPERTY(VisibleAnywhere, Category = "Components")
    UParticleSystemComponent* ExplosionEffect;

    /** 발사체 이동 */
    UPROPERTY(VisibleAnywhere, Category = "Movement")
    UProjectileMovementComponent* ProjectileMovement;

    /** 총알이 살아있는지 여부 */
    bool bIsActive;

    /** 최대 이동 거리 */
    UPROPERTY(EditDefaultsOnly, Category = "Bullet")
    float MaxDistance = 3000.0f;

    /** 현재 이동 거리 */
    float TraveledDistance;

    /** 보스 참조 */
    ABoss* BossRef;

    /** B콜리전 비활성화 타이머 */
    FTimerHandle BCollisionDeactivateTimer;

    /** B콜리전 데미지 적용 타이머 */
    FTimerHandle BDamageApplyTimer;



    
};
