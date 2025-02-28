// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System\EnumSet.h"
#include "Components/ActorComponent.h"
#include "SkillManager.generated.h"

class UActiveSkillSpawnManager;
class UTimerSkillSpawnManagerComponent;
class ABaseSkill;
struct FPlayerSkillRow;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EDMUNDPRJ_API USkillManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USkillManager();
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void ActivateSkill(FPlayerSkillRow& row);
	void ActivateTimerSkill(ETimerSkillType SkillType);
	void ActivateActiveSkill(EActiveSkillType SkillType);
	void ActivatePassiveSkill(EPassiveSkillType SkillType);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TObjectPtr<UTimerSkillSpawnManagerComponent> TimerSkillManager;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TObjectPtr<UActiveSkillSpawnManager> ActiveSkillManager;
};
