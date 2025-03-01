// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System\EnumSet.h"
#include "Components/ActorComponent.h"
#include "PassiveSkillManager.generated.h"

DECLARE_DELEGATE(FOnHPChangedDelegate);

class ABaseCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EDMUNDPRJ_API UPassiveSkillManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPassiveSkillManager();
	void ActivatePassiveSkill(EPassiveSkillType passiveSkillType);
	void BerserkerSkill();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	TObjectPtr<ABaseCharacter> Character;
	TMap<EPassiveSkillType, int> PassiveSkillMap;

	FOnHPChangedDelegate OnHPChanged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Berserker")
	float MinHealthMultiplier = 0;
};
