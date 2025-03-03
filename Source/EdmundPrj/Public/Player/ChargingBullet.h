#pragma once

#include "CoreMinimal.h"
#include "Player/BaseProjectile.h"
#include "ChargingBullet.generated.h"

class APlayerCharacterWraith;

UCLASS()
class EDMUNDPRJ_API AChargingBullet : public ABaseProjectile
{
	GENERATED_BODY()
	
public:
	AChargingBullet();
	virtual void SetBulletScale();

	virtual void OnProjectileOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;

	void SetBulletDamage(float NewDamage);

protected:
	void BeginPlay();

private:
	float BulletDamage;
};
