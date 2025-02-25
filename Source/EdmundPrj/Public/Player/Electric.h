// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Electric.generated.h"

class ABaseMonster;
class USphereComponent;
class UStaticMeshComponent;
class USplineComponent;
class UElectricEffectPool;

UCLASS()
class EDMUNDPRJ_API AElectric : public AActor
{
	GENERATED_BODY()
	
public:	
	AElectric();
	virtual void BeginPlay() override;
	// Sets default values for this actor's properties
	UFUNCTION()
	void FindMonster(
		UPrimitiveComponent* overlappedComp,
		AActor* otherActor,
		UPrimitiveComponent* otherComp,
		int32 otherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	UFUNCTION()
	void MoveToMonster(ABaseMonster* monster);
	UFUNCTION()
	void Move();
	UFUNCTION()
	void Attack(ABaseMonster* monster);
	UFUNCTION()
	void Activate();
	UFUNCTION()
	void Deactivate();
	UFUNCTION()
	void ResetSplineRotation();
	UFUNCTION()
	void AddSplinePoint(FVector NewPoint);
	UFUNCTION()
	void DeleteSplinePoint();
public:
	//º¯¼ö
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
	float PointDestroyTime = 0;
	int CurrentElectricCount = 0;
	TObjectPtr<ABaseMonster> TargetMonster;
	FVector FirstEnemyPos = FVector::ZeroVector;
	TArray<FVector> EnemyPosArray = TArray<FVector>();
	bool isFirst = false;
	FTimerHandle MoveTimer;
	FTimerHandle DeactivateTimer;
	TObjectPtr<UElectricEffectPool> ElectricEffectPool = nullptr;
};
