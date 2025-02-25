#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

class USphereComponent;

UCLASS()
class EDMUNDPRJ_API ABullet : public AActor
{
	GENERATED_BODY()

public:
	ABullet();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bullet|Component")
	USphereComponent* Collision;

public:
	void SetBulletHidden(bool IsHidden);  // ÃÑ¾Ë ¼û±è »óÅÂ¸¦ ¼³Á¤ÇÏ´Â ÇÔ¼ö

	void EndBulletLife();  // ÃÑ¾Ë ¼û±è »óÅÂ¸¦ ¼³Á¤ÇÏ´Â ÇÔ¼ö

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

	bool bIsHidden;
};
