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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet|Sound")
	UAudioComponent* CurrentAudioComp;

	AFireBall();
	void SetBulletHidden(bool IsHidden) override;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void OnProjectileOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
	float AttackRadiusMultifler;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet|Particle")
	TObjectPtr<UParticleSystem> FireBallParticle;  // 마법 날아가는 동안 생성될 파티클

	void EndBulletLife() override;

private:
	FTimerHandle ParticleCreateDelayTimerHandle;

	void CreateParticle();
};
