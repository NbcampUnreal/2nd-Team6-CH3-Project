// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System\EnumSet.h"
#include "Components/SceneComponent.h"
#include "ActiveSkillSpawnManager.generated.h"

class AAttackSkill;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EDMUNDPRJ_API UActiveSkillSpawnManager : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActiveSkillSpawnManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	void UpgradeActiveSkill(EActiveSkillType skillType);
	void ActivateProbCalculate();
	void CreateActiveSkill(TSubclassOf<AAttackSkill> attackSkill, EActiveSkillType skillType, int createCount);
	void ActivateActiveSkill(EActiveSkillType skillType, FVector ForwardVector);
	TObjectPtr<AAttackSkill> FindDeactivateActiveSkill(EActiveSkillType skillType);

public:
	float ActivateProb = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	TMap<EActiveSkillType, TSubclassOf<AAttackSkill>> ActiveSkillClassMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	TMap<EActiveSkillType, int> ActiveSkillProb;

	TSet<EActiveSkillType> ActivateSkillList;

	TMap<EActiveSkillType, TArray<TObjectPtr<AAttackSkill>>> ActiveSkillMap;

	TMap<EActiveSkillType, int> ActiveSkillUpgradeCount;
};
