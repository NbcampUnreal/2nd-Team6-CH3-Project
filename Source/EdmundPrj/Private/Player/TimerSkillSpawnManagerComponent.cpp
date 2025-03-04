// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TimerSkillSpawnManagerComponent.h"
#include "Components\SphereComponent.h"
#include "Monster\BaseMonster.h"
#include "Player\BaseCharacter.h"
#include "Player\TimerSkill.h"
#include "System\EnumSet.h"
#include "Player\SupportCharacter.h"
// Sets default values for this component's properties
UTimerSkillSpawnManagerComponent::UTimerSkillSpawnManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

FVector UTimerSkillSpawnManagerComponent::GetRandomMonsterLocation()
{
	if (Character == nullptr)
	{
		Character = Cast<ABaseCharacter>(GetOwner());
	}
	Character->SupportCharInstance->CheckMonster();
	TSet<TObjectPtr<ABaseMonster>> Monsters; 
	for (auto& Monster : Character->SupportCharInstance->Monsters)
	{
		Monsters.Add(Monster.Key);
	}
	int randomIndex = FMath::RandRange(0, Monsters.Num() - 1);
	int currentIndex = 0;
	for (ABaseMonster* Monster : Monsters)
	{
		if (currentIndex == randomIndex)
		{
			return Monster->GetActorLocation();
		}
		currentIndex++;
	}
	return FVector::ZeroVector;
}

FVector UTimerSkillSpawnManagerComponent::SummonSkillLocation(FVector randomPos)
{
	randomPos.X = FMath::RandRange(randomPos.X - 100, randomPos.X + 100);
	randomPos.Y = FMath::RandRange(randomPos.Y - 100, randomPos.Y + 100);
	return randomPos;
}

void UTimerSkillSpawnManagerComponent::SetSkillTimer(ETimerSkillType skillType)
{
	if (!TimerSkillClassMap.Contains(skillType) || !IsValid(TimerSkillClassMap[skillType]))
	{
		UE_LOG(LogTemp, Error, TEXT("TimerSkillClassMap[%d] is NULL!!!"), (int32)skillType);
		return;
	}

	FTimerHandle& SkillTimer = SkillTimerMap.FindOrAdd(skillType);
	if (!IsValid(TimerSkillClassMap[skillType])) return;
	CreateTimerSkill(TimerSkillClassMap[skillType], skillType, 10);
	
	TObjectPtr<ATimerSkill> skill = FindDeactivateTimerSkill(skillType);
	if (skill == nullptr) return;
	GetWorld()->GetTimerManager().SetTimer(
		SkillTimer,
		[this, skillType] {

			ActivateTimerSkill(skillType);
		},
		skill->AttackCycleSeconds,
		true
	);
}

void UTimerSkillSpawnManagerComponent::ClearSkillTimer()
{
	for (auto timerHandle : SkillTimerMap)
	{
		GetWorld()->GetTimerManager().ClearTimer(timerHandle.Value);
	}
}



// Called when the game starts
void UTimerSkillSpawnManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	Character = Cast<ABaseCharacter>(GetOwner());
}

void UTimerSkillSpawnManagerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	for (auto& timerHandle : SkillTimerMap)
	{
		GetWorld()->GetTimerManager().ClearTimer(timerHandle.Value);
	}
}

void UTimerSkillSpawnManagerComponent::ActivateTimerSkill(ETimerSkillType skillType)
{
	TWeakObjectPtr<ATimerSkill> skill = nullptr;
	skill = FindDeactivateTimerSkill(skillType);
	if (!skill.IsValid())
	{
		CreateTimerSkill(TimerSkillClassMap[skillType], skillType, 10);
		skill = FindDeactivateTimerSkill(skillType);
	}
	if (!skill.IsValid()) return;
	
	FVector skillLocation = GetRandomMonsterLocation();
	if (skillLocation == FVector::ZeroVector)
	{
		UE_LOG(LogTemp, Warning, TEXT("Target is Null"));
		return;
	}
	if (skillType == ETimerSkillType::AttackPlants || skillType == ETimerSkillType::HealPlants)
	{
		skillLocation = SummonSkillLocation(skillLocation);
	}

	skillLocation.Z = skill->SpawnPosZ;
	if (skill->TimerSkillSpanwManager == nullptr)
	{
		skill->TimerSkillSpanwManager = this;
	}
	skill->SetActorLocation(skillLocation);
	skill->SetActorEnableCollision(true);
	skill->SetActorHiddenInGame(false);
	skill->SetActorTickEnabled(true);
	skill->SpawnTimerSkill();
}

void UTimerSkillSpawnManagerComponent::DeactivateTimerSkill(TObjectPtr<ATimerSkill> timerSkill)
{;
	TObjectPtr<ATimerSkill>& skill = timerSkill;
	skill->SetActorHiddenInGame(true);
}

void UTimerSkillSpawnManagerComponent::CreateTimerSkill(TSubclassOf<ATimerSkill> timerSkill, ETimerSkillType skillType, int createCount)
{
	if (!timerSkill) return;
	if (Character == nullptr)
	{
		Character = Cast<ABaseCharacter>(GetOwner());
	}
	for (int i = 0; i < createCount; i++)
	{
		if (!IsValid(GetWorld())) return;
		TObjectPtr<ATimerSkill> skill = GetWorld()->SpawnActor<ATimerSkill>(timerSkill);

		if (!skill) continue;
		skill->DamageMultiplier = Character->AttackDamage;
		skill->Character = Character;
		skill->SetActorHiddenInGame(true);
		skill->SetActorEnableCollision(false);
		skill->SetActorTickEnabled(false);
		skill->TimerSkillSpanwManager = this;
		TimerSkillMap.FindOrAdd(skillType).Add(skill);
	}
}

TObjectPtr<ATimerSkill> UTimerSkillSpawnManagerComponent::FindDeactivateTimerSkill(ETimerSkillType skillType)
{
	if (!TimerSkillMap.Find(skillType) || TimerSkillMap[skillType].Num() == 0) return nullptr;
	for (TObjectPtr<ATimerSkill>& skill : TimerSkillMap[skillType])
	{

		if (skill->IsHidden())
		{
			return skill;
		}
	}
	return nullptr;
}



