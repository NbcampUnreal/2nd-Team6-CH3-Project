// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "TimerSkillSpawnManagerComponent.generated.h"

enum class ETimerSkillType : uint8;
class USphereComponent;
class ATimerSkill;
class ABaseCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EDMUNDPRJ_API UTimerSkillSpawnManagerComponent : public USceneComponent
{
	
	GENERATED_BODY()

	public:
		// Sets default values for this component's properties
		UTimerSkillSpawnManagerComponent();

		FVector GetRandomMonsterLocation();
		FVector SummonSkillLocation(FVector randomPos);

		void SetSkillTimer(ETimerSkillType skillType);
		void ClearSkillTimer();
		void CreateTimerSkill(TSubclassOf<ATimerSkill> timerSkill, ETimerSkillType skillType, int createCount);
		void ActivateTimerSkill(ETimerSkillType skillType);
		void DeactivateTimerSkill(TObjectPtr<ATimerSkill> timerSkill);
		TObjectPtr<ATimerSkill> FindDeactivateTimerSkill(ETimerSkillType skillType);
	protected:
		// Called when the game starts
		virtual void BeginPlay() override;
		virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);
	public:
		UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Setting")
		TObjectPtr<ABaseCharacter> Character = nullptr;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
		float AttackRadius = 0;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
		float DamageMultiplier = 0;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
		TMap<ETimerSkillType, TSubclassOf<ATimerSkill>> TimerSkillClassMap;

		TMap<ETimerSkillType, TArray<TObjectPtr<ATimerSkill>>> TimerSkillMap;
		UPROPERTY()
		TMap<ETimerSkillType, FTimerHandle> SkillTimerMap;

		TMap<ETimerSkillType, int> TimerSkillUpgardeCountMap;
	};