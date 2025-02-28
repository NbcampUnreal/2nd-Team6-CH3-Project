// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TimerSkillSpawnManagerComponent.h"
#include "Components\SphereComponent.h"
#include "Monster\BaseMonster.h"
#include "Player\TimerSkill.h"
#include "System\EnumSet.h"
// Sets default values for this component's properties
UTimerSkillSpawnManagerComponent::UTimerSkillSpawnManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	EnemySearchCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SkllTimerCollision"));
	EnemySearchCollision->SetCollisionProfileName(TEXT("OverlapAll"));
	EnemySearchCollision->OnComponentBeginOverlap.AddDynamic(this, &UTimerSkillSpawnManagerComponent::BeginOverlaped);
	EnemySearchCollision->OnComponentEndOverlap.AddDynamic(this, &UTimerSkillSpawnManagerComponent::EndOverlaped);
	EnemySearchCollision->SetupAttachment(this);
}

TSet<TObjectPtr<ABaseMonster>> InsideMonster;

void UTimerSkillSpawnManagerComponent::BeginOverlaped(
	UPrimitiveComponent* overlappedComp,
	AActor* otherActor,
	UPrimitiveComponent* otherComp,
	int32 otherBodyIndex,
	bool bFromSweep, const
	FHitResult& SweepResult)
{
	if (otherActor && otherActor->ActorHasTag("Monster"))
	{
		if (TObjectPtr<ABaseMonster> Monster = Cast<ABaseMonster>(otherActor))
		{
			InsideMonster.Add(Monster);
		}
	}
}

void UTimerSkillSpawnManagerComponent::EndOverlaped(
	UPrimitiveComponent*
	overlappedComponent,
	AActor* otherActor,
	UPrimitiveComponent* otherComp,
	int32 otherBodyIndex)
{
	if (otherActor && otherActor->ActorHasTag("Monster"))
	{
		if (TObjectPtr<ABaseMonster> monster = Cast<ABaseMonster>(otherActor))
		{
			if (InsideMonster.Contains(monster))
			{
				InsideMonster.Remove(monster);
			}
		}
	}
}

FVector UTimerSkillSpawnManagerComponent::GetRandomMonsterLocation()
{
	// ���� ����Ʈ�� ����ִ��� Ȯ��
	if (InsideMonster.IsEmpty()) return FVector::ZeroVector;

	// `InsideMonster`�� `TArray`�� ��ȯ
	TArray<TObjectPtr<ABaseMonster>> MonsterArray = InsideMonster.Array();
	MonsterArray.RemoveAll([](TWeakObjectPtr<ABaseMonster> Monster) {
		return !Monster.IsValid(); // `IsValid()`�� false�� ����
		});
	// ��ȯ�� �迭�� ��� �ִ��� Ȯ��
	if (MonsterArray.IsEmpty() || MonsterArray.Num() == 0) return FVector::ZeroVector;

	// ������ �ε��� ����
	int RandomIndex = FMath::RandRange(0, MonsterArray.Num() - 1);

	if (MonsterArray.IsValidIndex(RandomIndex)) // �迭�� ��ȿ�� �ε������� Ȯ��
	{
		TWeakObjectPtr<ABaseMonster> RandomMonster = MonsterArray[RandomIndex];
		if (TObjectPtr<ABaseMonster> MonsterPtr = RandomMonster.Get())
		{
			if (IsValid(MonsterPtr)) 
			{
				FVector MonsterLocation = MonsterPtr->GetActorLocation();
				RandomMonster.Reset();
				return MonsterLocation;
			}
		}
	}

	// ��� ������ �����ϸ� �⺻�� ��ȯ
	return FVector::ZeroVector;
}

void UTimerSkillSpawnManagerComponent::SetSkillTimer(ETimerSkillType skillType)
{
	FTimerHandle& SkillTimer = SkillTimerMap.FindOrAdd(skillType);
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
}

void UTimerSkillSpawnManagerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	for (auto& timerHandle : SkillTimerMap)
	{
		GetWorld()->GetTimerManager().ClearTimer(timerHandle.Value);
	}
	InsideMonster.Reset();
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
	if (skillType != ETimerSkillType::Plants)
	{
		skillLocation.Z += 1000;
	}
	if (skill->TimerSkillSpanwManager == nullptr)
	{
		skill->TimerSkillSpanwManager = this;
	}
	FTimerHandle DestroyHandler;
	GetWorld()->GetTimerManager().SetTimer(DestroyHandler,
		[skill] {
			if (!skill.IsValid()) return;
			skill->Deactivate();
		},
		skill->DeactivateTime,
		false);
	skill->SetActorLocation(skillLocation);
	skill->SetActorEnableCollision(true);
	skill->SetActorHiddenInGame(false);
	skill->SetActorTickEnabled(true);
}

void UTimerSkillSpawnManagerComponent::DeactivateTimerSkill(TObjectPtr<ATimerSkill> timerSkill)
{
	FString skillName = FString::Printf(TEXT("DeactivateThunder"));
	TObjectPtr<ATimerSkill>& skill = timerSkill;
	skill->SetActorLabel(skillName);
	skill->SetActorHiddenInGame(true);
	if (!skill->IsHidden())
	{
		UE_LOG(LogTemp, Warning, TEXT("ThisActorIsNotHidden!"));
	}
}

void UTimerSkillSpawnManagerComponent::CreateTimerSkill(TSubclassOf<ATimerSkill> timerSkill, ETimerSkillType skillType, int createCount)
{
	if (!timerSkill) return;
	for (int i = 0; i < createCount; i++)
	{
		if (!IsValid(GetWorld())) return;
		TObjectPtr<ATimerSkill> skill = GetWorld()->SpawnActor<ATimerSkill>(timerSkill);

		if (!skill) continue;
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



