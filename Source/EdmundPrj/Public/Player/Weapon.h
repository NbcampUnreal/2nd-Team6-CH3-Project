#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/AudioComponent.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Sound")
	TObjectPtr<USoundBase> FireSound;  // ÃÑ¼Ò¸®

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Particle")
	TObjectPtr<UParticleSystem> FireParticle;  // ÃÑ ÀÌÆåÆ®

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Sound")
	UAudioComponent* CurrentAudioComp;

private:

	void InitializeBulletPool(int32 PoolSize);  // BulletPoolÀ» ÃÊ±âÈ­ÇÏ´Â ÇÔ¼ö
	ABullet* GetBulletFromPool();
	void ReturnBulletToPool(ABullet* Bullet);

	FTimerHandle AttackDelayHandle;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABullet> BulletClass;

	UPROPERTY()
	TArray<ABullet*> BulletPool;  // ÃÑ¾Ë Ç® ¹è¿­

	void ActivateAttack();

	bool IsAttack;
};
