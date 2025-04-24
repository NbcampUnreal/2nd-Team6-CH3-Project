// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseSkill.generated.h"

class ABaseCharacter;

UCLASS()
class EDMUNDPRJ_API ABaseSkill : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseSkill();
	void SelecteSkill();
	void SelectedSkillActivate();
	virtual void UpgradeSkill();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TObjectPtr<ABaseCharacter> Character = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	FString SkillName = "defaultSkillName";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	int SkillMaxUpgradeIndex = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	int SkillCurrentUpgradeIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	int SkillIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	bool bIsSkillActivate = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float DamageMultiplierAmount = 0;

	UAudioComponent* AudioComponent = nullptr;
};
