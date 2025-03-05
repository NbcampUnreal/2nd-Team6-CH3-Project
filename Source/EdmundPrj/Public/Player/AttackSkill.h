// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseSkill.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AttackSkill.generated.h"

class USphereComponent;
class ABaseMonster;

UCLASS()
class EDMUNDPRJ_API AAttackSkill : public ABaseSkill
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAttackSkill();
	virtual void ActivateSkill();
	UFUNCTION()
	void BeginOverlaped(
		UPrimitiveComponent* overlappedComp,
		AActor* otherActor,
		UPrimitiveComponent* otherComp,
		int32 otherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	virtual void HitToMonster(TObjectPtr<ABaseMonster> monster);
	void MoveForward();
	virtual void Tick(float deltaTime) override;
	void DeactivateSkill();
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
	float MoveSpeed = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float SkillRange = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float DamageMultiplier = 0;
	FVector CharForwardVector = FVector::ZeroVector;
	FVector FinishPos = FVector::ZeroVector;
};
