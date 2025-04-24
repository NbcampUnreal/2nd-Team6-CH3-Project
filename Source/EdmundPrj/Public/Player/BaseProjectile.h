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

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bullet|Component")
	USphereComponent* Collision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet|Particle")
	TObjectPtr<UParticleSystem> BulletLandParticle;  // ź�� ��ƼŬ

	bool bIsHidden;

	FTimerHandle BulletLifeTimerHandle;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet", meta = (AllowPrivateAccess = "true"))
	float BulletSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet", meta = (AllowPrivateAccess = "true"))
	float BulletDuraion;

	FVector PrevProjectileLocation;
};
