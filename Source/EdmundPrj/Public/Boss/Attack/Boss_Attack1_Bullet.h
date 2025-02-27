#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Boss_Attack1_Bullet.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UParticleSystemComponent;
class ABoss;

UCLASS()
class EDMUNDPRJ_API ABoss_Attack1_Bullet : public AActor
{
	GENERATED_BODY()

public:
	ABoss_Attack1_Bullet();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	ABoss* BossRef;

	// Hit 이벤트
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// Overlap 이벤트
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 탄환 발사 시 호출되는 함수
	void FireProjectile(FVector SpawnLocation, FRotator SpawnRotation, FVector Direction);

	// 폭발 처리
	void Explode();

	// 탄환 초기화 및 풀에 반환
	void ResetBullet();

	// 총알 풀에서 사용 가능한 탄환을 가져오거나 없으면 새로 생성하는 함수
	static ABoss_Attack1_Bullet* GetBulletFromPool(UWorld* World, TSubclassOf<ABoss_Attack1_Bullet> BulletClass, ABoss* InBossRef);

	// 탄환 활성 여부
	bool bIsActive = false;

	// 탄환 이동 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
	float BulletSpeed = 2000.0f;

	// 최대 이동 거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
	float MaxDistance = 5000.0f;

	// 이동 누적 거리
	float TraveledDistance = 0.0f;

	// 폭발 파티클 재생 후 탄환 리셋까지의 지연 시간 (초)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	float ExplosionDelay = 1.0f;

	// 폭발 파티클 이펙트 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects")
	UParticleSystemComponent* ExplosionEffect;

	// 충돌 처리용 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* CollisionComp;

	// 탄환 메쉬 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* BulletMesh;

	// 탄환 풀
	static TArray<ABoss_Attack1_Bullet*> BulletPool;
};
