// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillManager.generated.h"

class ABaseSkill;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EDMUNDPRJ_API USkillManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USkillManager();
	void ActivateSkill(int SkillIndex);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	TArray<TObjectPtr<ABaseSkill>> Skills = TArray<TObjectPtr<ABaseSkill>>();
		
};
