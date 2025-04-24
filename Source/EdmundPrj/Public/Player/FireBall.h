#pragma once

#include "CoreMinimal.h"
#include "Player/Bullet.h"
#include "FireBall.generated.h"

class UParticleSystemComponent;

UCLASS()
class EDMUNDPRJ_API AFireBall : public ABaseProjectile
{
	GENERATED_BODY()

public:
	AFireBall();

	void EndBulletLife() override;

	void SetBulletHidden(bool IsHidden) override;

	virtual void OnProjectileOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet", meta = (AllowPrivateAccess = "true"))
	float AttackRadiusMultifler;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet|Particle", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UParticleSystem> FireBallParticle;

	UAudioComponent* CurrentAudioComp;
	FTimerHandle ParticleCreateDelayTimerHandle;

	void CreateParticle();
};
