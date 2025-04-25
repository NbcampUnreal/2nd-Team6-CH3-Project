#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class ABaseProjectile;

UCLASS()
class EDMUNDPRJ_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	AWeapon();

	bool Fire(float NewAttackDelay);

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TObjectPtr<class USkeletalMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TObjectPtr<USceneComponent> MuzzleOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float AttackDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Particle")
	TObjectPtr<UParticleSystem> FireParticle;  // �� ����Ʈ

protected:
	virtual void BeginPlay() override;

private:
	void InitializeBulletPool(int32 PoolSize);  // BulletPool�� �ʱ�ȭ�ϴ� �Լ�
	ABaseProjectile* GetBulletFromPool();
	void ReturnBulletToPool(ABaseProjectile* Bullet);

	FTimerHandle AttackDelayHandle;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABaseProjectile> BulletClass;

	UPROPERTY()
	TArray<ABaseProjectile*> BulletPool;  // �Ѿ� Ǯ �迭

	void DelayAttack();

	bool IsDelay;
};
