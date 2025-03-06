// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SupportCharacter.generated.h"

class ABaseMonster;
class USphereComponent;
class AEnemyDirectionArrow;

UCLASS()
class EDMUNDPRJ_API ASupportCharacter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASupportCharacter();
protected:
	virtual void BeginPlay() override;
public:
	void CheckMonster();

	void CreateDirectionArrow(int count);
	TObjectPtr<AEnemyDirectionArrow> FindDeactivateDirectionArrow();
	void ActivateDrectionArrow(TObjectPtr<ABaseMonster> Monster);
	void DeactivateArrow(TObjectPtr<AEnemyDirectionArrow> DirectionArrow, TObjectPtr<ABaseMonster> Monster);


	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Setting")
	TObjectPtr<USceneComponent> Scene = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Setting")
	TObjectPtr <USphereComponent> EnemySearchCollision = nullptr;

	TMap<TObjectPtr<ABaseMonster>, bool> Monsters = TMap<TObjectPtr<ABaseMonster>, bool>();

	FTimerHandle CheckMonsterHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float CheckMonsterCycleTime = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	TSubclassOf<AEnemyDirectionArrow> EnemyDirectionArrowClass = nullptr;
	TSet<TObjectPtr<AEnemyDirectionArrow>> EnemyDirectionArrows = TSet<TObjectPtr<AEnemyDirectionArrow>>();
};
