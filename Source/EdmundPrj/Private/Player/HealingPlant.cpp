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
	//»ç¿îµå
	AEdmundGameState* GameState = GetWorld() ? Cast<AEdmundGameState>(GetWorld()->GetGameState()) : nullptr;
	if (GameState != nullptr)
	{
		//GameState->PlayItemSound(AudioComponent, EItemType::HealKit)
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
			FVector myPos = GetActorLocation();
			FVector randPos;;
			randPos.X = FMath::RandRange(myPos.X - 3, myPos.X + 3);
			randPos.Y = FMath::RandRange(myPos.Y - 3, myPos.Y + 3);
			randPos.Z = myPos.Z + 10;
			SetActorLocation(randPos);
			if (myPos.Z >= StartPos.Z + SpawnPosZ)
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

void AHealingPlant::Deactivate()
{
	GetWorldTimerManager().ClearTimer(HealCycleHandle);
	Super::Deactivate();
}

void AHealingPlant::UpgradeSkill()
{
	HealAmount += DamageMultiplierAmount;
}
