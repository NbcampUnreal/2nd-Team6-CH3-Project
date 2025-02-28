// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/Mission/BaseMissionItem.h"
#include "IndicatorParts.generated.h"

/**
 * 
 */
UCLASS()
class EDMUNDPRJ_API AIndicatorParts : public ABaseMissionItem
{
	GENERATED_BODY()
	
public:
	AIndicatorParts();
	virtual void InitMissionItem(AMissionHandle* NewMissionHandle, const FName& Type) override;

private:
	virtual void Tick(float DeltaTime) override;
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	float MoveUpSpeed = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	float MoveForwardSpeed = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	float TargetHeightAmount = 300;

private:
	TObjectPtr<ABaseMissionItem> Target;
	FVector Direction;
	float StartHeight;
	bool bIsMove = false;
};
