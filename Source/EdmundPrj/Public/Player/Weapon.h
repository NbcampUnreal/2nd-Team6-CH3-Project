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
	void Fire();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USkeletalMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> MuzzleOffset;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAnimMontage> FireMontage;

private:

	void InitializeBulletPool(int32 PoolSize);  // BulletPool을 초기화하는 함수
	ABullet* GetBulletFromPool();
	void ReturnBulletToPool(ABullet* Bullet);

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABullet> BulletClass;

	UPROPERTY()
	TArray<ABullet*> BulletPool;  // 총알 풀 배열
};
