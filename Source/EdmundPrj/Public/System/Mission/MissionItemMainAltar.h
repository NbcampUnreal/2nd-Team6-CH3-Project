// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/Mission/BaseMissionItem.h"
#include "MissionItemMainAltar.generated.h"


UCLASS()
class EDMUNDPRJ_API AMissionItemMainAltar : public ABaseMissionItem
{
	GENERATED_BODY()
	
public:
	AMissionItemMainAltar();

	virtual void InitMissionItem(AMissionHandle* NewMissionHandle, const FName& Type) override;
	virtual void ActionEventByPressedKey() override;

private:
	virtual void ActionBeginOverlap() override;
	virtual void ActionEndOverlap() override;
	virtual void Tick(float DeltaTime) override;
	virtual void CompleteProgress() override;

private:
	float CurrentTime = 0.0f;
	float TargetTime = 0.01f;
};
