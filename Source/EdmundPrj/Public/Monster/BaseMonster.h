// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseMonster.generated.h"

UCLASS()
class EDMUNDPRJ_API ABaseMonster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseMonster();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	float MonsterHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	float MonsterMaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	float MonsterAttackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	float MonsterArmor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	float MonsterMoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	float MonsterAttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Components")
	USceneComponent* SceneComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Components")
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Components")
	UCapsuleComponent* CapsuleComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Effects")
	UParticleSystem* AttackParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Effects")
	UParticleSystem* TakeDamageParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Effects")
	USoundBase* AttackSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Effects")
	USoundBase* TakeDamageSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	FName MonsterType;
};
