// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseMissionItem.generated.h"

class UBoxComponent;
class AMissionHandle;

UCLASS()
class EDMUNDPRJ_API ABaseMissionItem : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseMissionItem();
	void InitMissionItem(AMissionHandle* NewMissionHandle);

protected:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	virtual void BeginPlay() override;
	
	virtual void ActionBeginOverlap();
	virtual void ActionEndOverlap();
	virtual void ActionOnHit();

	void ApplyOverlapCollision(bool bIsBlockedMesh);
	void ApplyBlockCollision();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBoxComponent> CollisionComp = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> MeshComp = nullptr;

private:
	TObjectPtr<AMissionHandle> MissionHandle;
};
