#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Boss_Attack13_Bullet.generated.h"

class ABoss;

UCLASS()
class EDMUNDPRJ_API ABoss_Attack13_Bullet : public AActor
{
	GENERATED_BODY()

public:
	ABoss_Attack13_Bullet();
	virtual void BeginPlay() override;

protected:
	// 루트 컴포넌트
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	USceneComponent* RootComp;

	// 스태틱 메시 컴포넌트
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	class UStaticMeshComponent* MeshComp;

	// Hit 콜리전 컴포넌트
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	class USphereComponent* HitCollision;

	// 폭발 콜리전 컴포넌트
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	class USphereComponent* ExplosionCollision;

	// 파티클 컴포넌트 (블루프린트에서 선택 가능)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UParticleSystemComponent* ParticleComp;

protected:
	// 오버랩 발생 시 호출되는 함수
	UFUNCTION()
	void OnHitOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	// 파티클 재생이 끝났을 때 호출되는 델리게이트 함수
	UFUNCTION()
	void OnParticleSystemFinished(class UParticleSystemComponent* PSystem);

	ABoss* BossRef;
};
