// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ElectricEffect.generated.h"

class ABaseMonster;
class USphereComponent;
class UStaticMeshComponent;
class USplineComponent;

UCLASS()
class EDMUNDPRJ_API AElectricEffect : public AActor
{
	GENERATED_BODY()

public:
	//함수
	AElectricEffect();
	UFUNCTION()
	void FindMonster(
		UPrimitiveComponent* overlappedComp,
		AActor* otherActor,
		UPrimitiveComponent* otherComp,
		int32 otherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	void MoveToMonster(ABaseMonster* monster);
	void Move();
	void Attack(ABaseMonster* monster);
	void Deactivate();
	void ResetSplineRotation();
	//void SpawnNiagaraEffect(UWorld* World, UNiagaraSystem* NiagaraEffect, FVector Location, FRotator Rotation);

	void AddSplinePoint(FVector NewPoint);
	void DeleteSplinePoint();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	//변수
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Setting")
	TObjectPtr<USceneComponent> Scene = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Setting")
	TObjectPtr<UStaticMeshComponent> Mesh = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Setting")
	TObjectPtr<USphereComponent> EnemySearchCollision = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Setting")
	TObjectPtr<USplineComponent> SplineComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float AttackRadius = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float DamageMultiplier = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	int ElectricCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float Speed = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float DestroyTime = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float PointDestroyTime = 0;
	int CurrentElectricCount = 0;
	TObjectPtr<ABaseMonster> TargetMonster;

	FVector FirstEnemyPos = FVector::ZeroVector;
	TArray<FVector> EnemyPosArray = TArray<FVector>();
	bool isFirst = false;
	FTimerHandle MoveTimer;
	FTimerHandle DeactivateTimer;
};
