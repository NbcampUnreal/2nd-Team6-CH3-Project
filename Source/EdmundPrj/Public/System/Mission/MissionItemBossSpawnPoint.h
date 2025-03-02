// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/Mission/BaseMissionItem.h"
#include "System/EnumSet.h"
#include "MissionItemBossSpawnPoint.generated.h"

class ABoss;
class UBehaviorTree;

UCLASS()
class EDMUNDPRJ_API AMissionItemBossSpawnPoint : public ABaseMissionItem // º¸½º¶û delegate?
{
	GENERATED_BODY()
	
public:
	virtual void InitMissionItem(AMissionHandle* NewMissionHandle, const FName& Type) override;
	virtual void SetIsActive(bool Value) override;

	void SpawnMonster() const;
	void RequestMoveToNextPattern();
	void ClearBoss();

private:
	void SpawnBoss();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TSubclassOf<ABoss> BossClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TObjectPtr<UBehaviorTree> BossBt;

	TObjectPtr<ABoss> BossPawn;
};
