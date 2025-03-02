#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/AudioComponent.h"
#include "Weapon.generated.h"

class ABaseProjectile;

UCLASS()
class EDMUNDPRJ_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	AWeapon();

protected:
	virtual void BeginPlay() override;

public:
	bool Fire(float NewAttackDelay);

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TObjectPtr<class USkeletalMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TObjectPtr<USceneComponent> MuzzleOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float AttackDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Particle")
	TObjectPtr<UParticleSystem> FireParticle;  // 총 이펙트

private:

	void InitializeBulletPool(int32 PoolSize);  // BulletPool을 초기화하는 함수
	ABaseProjectile* GetBulletFromPool();
	void ReturnBulletToPool(ABaseProjectile* Bullet);

	FTimerHandle AttackDelayHandle;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABaseProjectile> BulletClass;

	UPROPERTY()
	TArray<ABaseProjectile*> BulletPool;  // 총알 풀 배열

	void ActivateAttack();

	bool IsAttack;
};
