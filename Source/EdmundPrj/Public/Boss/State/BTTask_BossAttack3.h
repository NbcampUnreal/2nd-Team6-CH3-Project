#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_BossAttack3.generated.h"

class ABoss;

UCLASS()
class EDMUNDPRJ_API UBTTask_BossAttack3 : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_BossAttack3();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UFUNCTION(BlueprintCallable)
	void Attack3_ActivateMeleeCollision_Check1();

	UFUNCTION(BlueprintCallable)
	void Attack3_DeactivateCollision_Check1();

	UFUNCTION()
	void OnMeleeCollisionOverlap_Check1(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void Attack3_ActivateMeleeCollision_Check2();

	UFUNCTION(BlueprintCallable)
	void Attack3_DeactivateCollision_Check2();

	UFUNCTION()
	void OnMeleeCollisionOverlap_Check2(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void Attack3_RangedAttackNotify();

	UPROPERTY(BlueprintReadOnly)
	class ABoss* BossRef;

	UFUNCTION(BlueprintCallable)
	void OnAttack1Notify();
	UFUNCTION(BlueprintCallable)
	void OnAttack2Notify();

	UFUNCTION(BlueprintCallable)
	void OnAttack3Notify();

	UFUNCTION(BlueprintCallable)
	void FinishComboAttack();

	void ExecuteMeleeAttack();
	void PlayAttack3Montage();
	
	void FireSingleBullet();

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	int32 GetComboPhase() { return ComboPhase; }
	
	UPROPERTY()
	UBehaviorTreeComponent* CachedOwnerComp;

	int32 ComboPhase;

	FTimerHandle CollisionDisableTimerHandle_Check1;
	FTimerHandle CollisionDisableTimerHandle_Check2;

private:
	FRotator CurrentRotation;
	FVector SpawnLocation;
	FTimerHandle BulletFireTimerHandle;
	float AccumulatedDeltaTime;
	int32 FiredBulletCount;

	FVector GetAdjustedSpawnLocation(const FVector& Offset) const;

	bool bIsDashing;
	FVector DashStartLocation;
	FVector DashTargetLocation;
	FVector DashCurrentVelocity;
	float DashFrequency;
	float DashDamping;

};
