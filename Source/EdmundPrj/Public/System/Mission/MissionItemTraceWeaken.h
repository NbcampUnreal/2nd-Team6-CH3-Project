// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/Mission/BaseMissionItem.h"
#include "MissionItemTraceWeaken.generated.h"


UCLASS()
class EDMUNDPRJ_API AMissionItemTraceWeaken : public ABaseMissionItem
{
	GENERATED_BODY()
	
public:
	virtual void InitMissionItem(AMissionHandle* NewMissionHandle, const FName& Type) override;
	virtual void ActionEventByPressedKey() override;

private:
	virtual void ActionBeginOverlap() override;
	virtual void ActionEndOverlap() override;

	void FindTraceByPlayer();
};
