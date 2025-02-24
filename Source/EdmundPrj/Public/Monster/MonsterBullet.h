#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MonsterBullet.generated.h"

class USphereComponent;

UCLASS()
class EDMUNDPRJ_API AMonsterBullet : public AActor
{
	GENERATED_BODY()

public:
	AMonsterBullet();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bullet|Component")
	USphereComponent* Collision;

public:
	void SetBulletHidden(bool IsHidden);  // �Ѿ� ���� ���¸� �����ϴ� �Լ�

	void EndBulletLife();  // �Ѿ� ���� ���¸� �����ϴ� �Լ�

	UFUNCTION()
	virtual void OnBulletOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovementComponent;

	FTimerHandle BulletLifeTimerHandle;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> StaticMeshComponent;
};
