// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimerSkill.generated.h"

class USphereComponent;
class ABaseMonster;

UCLASS()
class EDMUNDPRJ_API ATimerSkill : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATimerSkill();
	UFUNCTION()
	void BeginOverlaped(
		UPrimitiveComponent* overlappedComp,
		AActor* otherActor,
		UPrimitiveComponent* otherComp,
		int32 otherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	UFUNCTION()
	void EndOverlaped(
		UPrimitiveComponent* overlappedComponent,
		AActor* otherActor,
		UPrimitiveComponent* otherComp,
		int32 otherBodyIndex);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Setting")
	TObjectPtr<USceneComponent> Scene = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Setting")
	TObjectPtr<UStaticMeshComponent> Mesh = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Setting")
	TObjectPtr<USphereComponent> EnemySearchCollision = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float AttackRadius = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float DamageMultiplier = 0;

};
