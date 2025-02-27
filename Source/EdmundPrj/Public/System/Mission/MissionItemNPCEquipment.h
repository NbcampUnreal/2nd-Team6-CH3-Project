// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/Mission/BaseMissionItem.h"
#include "MissionItemNPCEquipment.generated.h"

/**
 * 
 */
UCLASS()
class EDMUNDPRJ_API AMissionItemNPCEquipment : public ABaseMissionItem
{
	GENERATED_BODY()
	
public:
	virtual void InitMissionItem(AMissionHandle* NewMissionHandle, const FName& Type, const FString& MissionInfo) override;
	virtual void ActionBeginOverlap() override;
	virtual void ActionEndOverlap() override;
	virtual void ActionEventByPressedKey() override;

};
