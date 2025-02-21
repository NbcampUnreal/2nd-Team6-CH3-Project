// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TimerSkillSpawnVolumComponent.h"
#include "Components\SphereComponent.h"
#include "Monster\BaseMonster.h"
#include "Player\TimerSkill.h"
// Sets default values for this component's properties
UTimerSkillSpawnVolumComponent::UTimerSkillSpawnVolumComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	EnemySearchCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	EnemySearchCollision->SetCollisionProfileName(TEXT("OverlapAll"));
	EnemySearchCollision->OnComponentBeginOverlap.AddDynamic(this, &UTimerSkillSpawnVolumComponent::BeginOverlaped);
	EnemySearchCollision->OnComponentEndOverlap.AddDynamic(this, &UTimerSkillSpawnVolumComponent::EndOverlaped);
}

TSet<TObjectPtr<ABaseMonster>> InsideMonster;

void UTimerSkillSpawnVolumComponent::BeginOverlaped(
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

void UTimerSkillSpawnVolumComponent::EndOverlaped(
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

FVector UTimerSkillSpawnVolumComponent::GetRandomMonsterLocation()
{
	if (InsideMonster.IsEmpty()) return FVector::ZeroVector;
	TArray<TObjectPtr<ABaseMonster>> MonsterArray = InsideMonster.Array();
	int RandomIndex = FMath::RandRange(0, MonsterArray.Num() - 1);
	if (MonsterArray[RandomIndex] != nullptr)
	{
		FVector MonsterLocation = MonsterArray[RandomIndex]->GetActorLocation();
		return MonsterLocation;
	}
	return FVector::ZeroVector;
}

void UTimerSkillSpawnVolumComponent::ActivateTimerSkill(TSubclassOf<ATimerSkill> timerSkill)
{
	
}

// Called when the game starts
void UTimerSkillSpawnVolumComponent::BeginPlay()
{
	Super::BeginPlay();

	if (TObjectPtr<AActor> Owner = GetOwner())
	{
		EnemySearchCollision->AttachToComponent(Owner->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	}
	
}


// Called every frame
void UTimerSkillSpawnVolumComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

