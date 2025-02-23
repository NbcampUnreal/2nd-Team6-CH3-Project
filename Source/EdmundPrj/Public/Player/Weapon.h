#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class ABullet;

UCLASS()
class EDMUNDPRJ_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	AWeapon();

protected:
	virtual void BeginPlay() override;

public:
	bool Fire();

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TObjectPtr<class USkeletalMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TObjectPtr<USceneComponent> MuzzleOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TObjectPtr<class UAnimMontage> FireMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float AttackDelay;

private:

	void InitializeBulletPool(int32 PoolSize);  // BulletPool을 초기화하는 함수
	ABullet* GetBulletFromPool();
	void ReturnBulletToPool(ABullet* Bullet);

	FTimerHandle AttackDelayHandle;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABullet> BulletClass;

	UPROPERTY()
	TArray<ABullet*> BulletPool;  // 총알 풀 배열

	void ActivateAttack();

	bool IsAttack;
};
