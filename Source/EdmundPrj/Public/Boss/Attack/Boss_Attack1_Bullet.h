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

    //총알 발사 함수
    void FireProjectile(FVector SpawnLocation, FRotator SpawnRotation, FVector Direction);

    // 충돌 이벤트 처리
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, FVector NormalImpulse,
        const FHitResult& Hit);

private:
    // 투사체의 메시
    UPROPERTY(VisibleAnywhere, Category = "Components")
    UStaticMeshComponent* BulletMesh;

    // 캡슐 컴포넌트
    UPROPERTY(VisibleAnywhere, Category = "Components")
    USphereComponent* ShpereComp;

    // 투사체의 속도
    UPROPERTY(EditDefaultsOnly, Category = "Movement")
    float BulletSpeed = 1200.0f;

    // 투사체의 최대 이동 거리
    UPROPERTY(EditDefaultsOnly, Category = "Lifetime")
    float MaxDistance = 3000.0f;  // 일정 거리 이상 이동 시 소멸

    // 현재 이동한 거리
    float TraveledDistance = 0.0f;

    // 오브젝트 풀에서 사용 여부
    bool bIsActive = false;
};
