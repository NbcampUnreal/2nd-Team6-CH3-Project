// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/HealingPlant.h"
#include "Player\BaseCharacter.h"
#include "Components\SphereComponent.h"
#include "System\EdmundGameState.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"

AHealingPlant::AHealingPlant()
{
	CheckCharacterCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	CheckCharacterCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	CheckCharacterCollision->SetupAttachment(Scene);

	HealingEffectNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("HealingEffectNiagara"));
	HealingEffectNiagara->SetupAttachment(RootComponent);
	HealingEffectNiagara->bAutoActivate = false;
}


void AHealingPlant::HealingCharacter()
{
	AEdmundGameState* GameState = GetWorld() ? Cast<AEdmundGameState>(GetWorld()->GetGameState()) : nullptr;
	if (GameState != nullptr)
	{
		GameState->PlayItemSound(AudioComponent, EItemType::HealPlants, ESoundType::Attack);
	}
	HealingEffectNiagara->Deactivate();
	HealingEffectNiagara->Activate();
	TArray<AActor*> overlappingActors;
	CheckCharacterCollision->GetOverlappingActors(overlappingActors);
	for (AActor* activator : overlappingActors)
	{
		if (activator && activator->ActorHasTag("Player"))
		{
			Character = Cast<ABaseCharacter>(activator);
			Character->AmountHP(HealAmount);
		}
	}
}

void AHealingPlant::SpawnTimerSkill()
{
	FVector StartPos = GetActorLocation();
	GetWorldTimerManager().SetTimer(SpawnShakeHandle,
		[this, StartPos] {
			FVector MyPos = GetActorLocation();
			FVector RandPos = FVector::ZeroVector;
			SetActorLocation(ShakeMove(RandPos, MyPos, 1));
			if (MyPos.Z >= StartPos.Z + SpawnPosZ)
			{
				Super::SpawnTimerSkill();
				GetWorldTimerManager().ClearTimer(SpawnShakeHandle);
				GetWorldTimerManager().SetTimer(HealCycleHandle,
					this,
					&AHealingPlant::HealingCharacter,
					HealCycleTime,
					true);
			}
		},
		0.1f,
		true);
}

FVector AHealingPlant::ShakeMove(FVector RandPos, const FVector& MyPos, int32 AddOrSub)
{
	RandPos.X = FMath::RandRange(MyPos.X - 3, MyPos.X + 3);
	RandPos.Y = FMath::RandRange(MyPos.Y - 3, MyPos.Y + 3);
	RandPos.Z = MyPos.Z + (10 * AddOrSub);
	return RandPos;
}

void AHealingPlant::Deactivate()
{
	GetWorldTimerManager().ClearTimer(HealCycleHandle);
	FVector StartPos = GetActorLocation();
	GetWorldTimerManager().SetTimer(SpawnShakeHandle,
		[this, StartPos] {
			FVector MyPos = GetActorLocation();
			FVector RandPos = FVector::ZeroVector;
			SetActorLocation(ShakeMove(RandPos, MyPos, -1));
			if (MyPos.Z < StartPos.Z - SpawnPosZ)
			{
				Super::Deactivate();
				GetWorldTimerManager().ClearTimer(SpawnShakeHandle);
			}
		},
		0.1f,
		true);
}

void AHealingPlant::UpgradeSkill()
{
	HealAmount += DamageMultiplierAmount;
}

void AHealingPlant::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorldTimerManager().ClearTimer(HealCycleHandle);
	GetWorldTimerManager().ClearTimer(SpawnShakeHandle);
}
