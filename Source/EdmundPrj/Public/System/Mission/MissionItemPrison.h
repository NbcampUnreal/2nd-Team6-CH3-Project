// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/Mission/BaseMissionItem.h"
#include "MissionItemPrison.generated.h"


UCLASS()
class EDMUNDPRJ_API AMissionItemPrison : public ABaseMissionItem
{
	GENERATED_BODY()
	
public:
	virtual void InitMissionItem(AMissionHandle* NewMissionHandle, const FName& Type) override;
	virtual void ActionEventByPressedKey() override;

	AMissionItemPrison();

private:
	virtual void ActionBeginOverlap() override;
	virtual void ActionEndOverlap() override;
	virtual void Tick(float DeltaTime) override;
	virtual void CompleteProgress() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MissionInfo)
	float TargetTime = 5.0f;

private:
	float CurrentTime = 0.0f;
	float DeltaTargetTime = 0.0f;
	float UpdateTime = 0.01f;
};
