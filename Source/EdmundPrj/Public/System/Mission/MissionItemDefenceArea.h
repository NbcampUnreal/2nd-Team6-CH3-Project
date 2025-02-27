// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/Mission/BaseMissionItem.h"
#include "MissionItemDefenceArea.generated.h"

/**
 * 
 */
UCLASS()
class EDMUNDPRJ_API AMissionItemDefenceArea : public ABaseMissionItem
{
	GENERATED_BODY()
	
public:
	virtual void InitMissionItem(AMissionHandle* NewMissionHandle, const FName& Type, const FString& MissionInfo) override;
	virtual void SetIsActive(bool Value) override;
	AMissionItemDefenceArea();

private:
	virtual void Tick(float DeltaTime) override;
	virtual void ActionBeginOverlap() override;
	virtual void ActionEndOverlap() override;
	virtual void CompleteProgress() override;

private:
	float CurrentTime = 0.0f;
	float TargetTime = 0.1f;
};
