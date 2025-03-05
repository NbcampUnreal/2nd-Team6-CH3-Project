// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/Mission/BaseMissionItem.h"
#include "MissionItemDimensionPortal.generated.h"

/**
 * 
 */
UCLASS()
class EDMUNDPRJ_API AMissionItemDimensionPortal : public ABaseMissionItem
{
	GENERATED_BODY()
	
public:
	AMissionItemDimensionPortal();

	virtual void InitMissionItem(AMissionHandle* NewMissionHandle, const FName& Type) override;
	virtual void ActionEventByPressedKey() override;

private:
	virtual void Tick(float DeltaTime) override;
	virtual void ActionBeginOverlap() override;
	virtual void ActionEndOverlap() override;
	virtual void CompleteProgress() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> GateMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MissionInfo)
	float TargetTime = 3.0f;

private:
	FVector MeshStartPos;
	FVector AddPos;
	float CurrentTime = 0;
	float DeltaTargetTime = 0.0f;
	float UpdateTime = 0.01f;
};
