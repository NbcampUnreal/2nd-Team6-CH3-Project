#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseProjectile.generated.h"

class USphereComponent;

UCLASS()
class EDMUNDPRJ_API ABaseProjectile : public AActor
{
	GENERATED_BODY()

public:
	ABaseProjectile();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bullet|Component")
	USphereComponent* Collision;

	bool bIsHidden;

public:
	virtual void SetBulletHidden(bool IsHidden);  // �Ѿ� ���� ���¸� �����ϴ� �Լ�

	virtual void EndBulletLife();  // �Ѿ� ���� ���¸� �����ϴ� �Լ�

	UFUNCTION()
	virtual void OnProjectileOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovementComponent;

	FTimerHandle BulletLifeTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet|Particle")
	TObjectPtr<UParticleSystem> BulletLandParticle;  // ź�� ��ƼŬ

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
	float BulletSpeed;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> StaticMeshComponent;

	FVector PrevProjectileLocation;
};
