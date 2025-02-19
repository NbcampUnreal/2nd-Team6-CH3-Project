// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ElectricEffect.generated.h"

class ABaseMonster;
class USphereComponent;
class UStaticMeshComponent;

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
	TObjectPtr<AActor> ActivateElectric(FVector monsterPos, FRotator direction);
	void DeactivateElectric(TObjectPtr<AActor> electric);
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	TSubclassOf<AActor> ElectricClass = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Setting")
	TArray<TObjectPtr<AActor>> Electrics = TArray<TObjectPtr<AActor>>();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float AttackRadius = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float DamageMultiplier = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	int ElectricCount = 0;

	int CurrentElectricCount = 0;
	TObjectPtr<ABaseMonster> TargetMonster;

	FTimerHandle MoveTimer;
};
