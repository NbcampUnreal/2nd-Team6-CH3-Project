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

    //�Ѿ� �߻� �Լ�
    void FireProjectile(FVector SpawnLocation, FRotator SpawnRotation, FVector Direction);

    // �浹 �̺�Ʈ ó��
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, FVector NormalImpulse,
        const FHitResult& Hit);

private:
    // ����ü�� �޽�
    UPROPERTY(VisibleAnywhere, Category = "Components")
    UStaticMeshComponent* BulletMesh;

    // ĸ�� ������Ʈ
    UPROPERTY(VisibleAnywhere, Category = "Components")
    USphereComponent* ShpereComp;

    // ����ü�� �ӵ�
    UPROPERTY(EditDefaultsOnly, Category = "Movement")
    float BulletSpeed = 1200.0f;

    // ����ü�� �ִ� �̵� �Ÿ�
    UPROPERTY(EditDefaultsOnly, Category = "Lifetime")
    float MaxDistance = 3000.0f;  // ���� �Ÿ� �̻� �̵� �� �Ҹ�

    // ���� �̵��� �Ÿ�
    float TraveledDistance = 0.0f;

    // ������Ʈ Ǯ���� ��� ����
    bool bIsActive = false;
};
