// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseSkill.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimerSkill.generated.h"

class USphereComponent;
class ABaseMonster;
class UTimerSkillSpawnManagerComponent;

UCLASS()
class EDMUNDPRJ_API ATimerSkill : public ABaseSkill
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
	virtual void HitToMonster(TObjectPtr<ABaseMonster> Monster);
	virtual void Deactivate();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Setting")
	TObjectPtr<USceneComponent> Scene = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Setting")
	TObjectPtr<USphereComponent> EnemySearchCollision = nullptr;
	UPROPERTY()
	TObjectPtr<UTimerSkillSpawnManagerComponent> TimerSkillSpanwManager = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float AttackRadius = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float DamageMultiplier = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float AttackCycleSeconds = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float MoveSpeed = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float DeactivateTime = 0;
};
