// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SupportCharacter.h"
#include "Components\SphereComponent.h"
#include "Monster\BaseMonster.h"
#include "Player\EnemyDirectionArrow.h"
// Sets default values
ASupportCharacter::ASupportCharacter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Scene = CreateDefaultSubobject<USphereComponent>(TEXT("Scene"));
	SetRootComponent(Scene);
	EnemySearchCollision = CreateDefaultSubobject<USphereComponent>(TEXT("EnemySearchCollision"));
	EnemySearchCollision->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASupportCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(
		CheckMonsterHandle,
		this,
		&ASupportCharacter::CheckMonster,
		CheckMonsterCycleTime,
		true
	);
}

void ASupportCharacter::CheckMonster()
{
	if (!IsValid(EnemySearchCollision)) return;
	TArray<AActor*> Activators;
	EnemySearchCollision->GetOverlappingActors(Activators);
	for (AActor* Activator : Activators)
	{
		if (Activator && Activator->ActorHasTag("Monster"))
		{
			if (TObjectPtr<ABaseMonster> Monster = Cast<ABaseMonster>(Activator))
			{
				Monsters.FindOrAdd(Monster, false);
				float Distance = FVector::Distance(GetActorLocation(), Monster->GetActorLocation());
				if (Distance < EnemySearchCollision->GetScaledSphereRadius() && !Monsters[Monster])
				{
					ActivateDrectionArrow(Monster);
					Monsters[Monster] = true;
				}
			}
		}
	}
}

void ASupportCharacter::CreateDirectionArrow(int count)
{
	if (!IsValid(EnemyDirectionArrowClass)) return;
	for (int i = 0; i < count; i++)
	{
		TObjectPtr<AEnemyDirectionArrow> DirectionArrow = GetWorld()->SpawnActor<AEnemyDirectionArrow>(EnemyDirectionArrowClass);
		DirectionArrow->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
		DirectionArrow->SetActorHiddenInGame(true);
		DirectionArrow->SetActorTickEnabled(false);
		DirectionArrow->SetActorEnableCollision(false);
		DirectionArrow->SupportChar = this;
		DirectionArrow->CollisionRadius = EnemySearchCollision->GetScaledSphereRadius();
		EnemyDirectionArrows.Add(DirectionArrow);
	}
}

TObjectPtr<AEnemyDirectionArrow> ASupportCharacter::FindDeactivateDirectionArrow()
{
	if (EnemyDirectionArrows.IsEmpty()) return nullptr;
	for (TObjectPtr<AEnemyDirectionArrow> DirectionArrow : EnemyDirectionArrows)
	{
		if (DirectionArrow->IsHidden())
		{
			return DirectionArrow;
		}
	}
	return nullptr;
}

void ASupportCharacter::ActivateDrectionArrow(TObjectPtr<ABaseMonster> Monster)
{
	int count = 0;
	for (TObjectPtr<AEnemyDirectionArrow> DirectionArrow : EnemyDirectionArrows)
	{
		if (DirectionArrow->IsHidden())
		{
			count++;
		}
	}
	TObjectPtr<AEnemyDirectionArrow> DirectionArrow = FindDeactivateDirectionArrow();
	if (DirectionArrow == nullptr)
	{
		CreateDirectionArrow(10);
		DirectionArrow = FindDeactivateDirectionArrow();
		if (DirectionArrow == nullptr) return;
	}
	DirectionArrow->Monster = Monster;
	DirectionArrow->SetActorTickEnabled(true);
	DirectionArrow->SetActorHiddenInGame(false);
	EnemyDirectionArrows.Remove(DirectionArrow);
}

void ASupportCharacter::DeactivateArrow(TObjectPtr<AEnemyDirectionArrow> DirectionArrow, TObjectPtr<ABaseMonster> Monster)
{
	if (IsValid(Monster))
	{
		Monsters[Monster] = false;
	}
	DirectionArrow->SetActorTickEnabled(false);
	DirectionArrow->SetActorHiddenInGame(true);
	EnemyDirectionArrows.Add(DirectionArrow);
}

void ASupportCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorldTimerManager().ClearTimer(CheckMonsterHandle);
	Monsters.Empty();
}

