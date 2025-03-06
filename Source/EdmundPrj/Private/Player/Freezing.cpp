// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Freezing.h"
#include "Monster\BaseMonster.h"
#include "Kismet\GameplayStatics.h"
void AFreezing::CreateFrozenEffect(int createCount)
{
	if (!FrozenEffectClass) return;

	for (int i = 0; i < createCount; i++)
	{
		TObjectPtr<AActor> frozenEffect = GetWorld()->SpawnActor<AActor>(
			FrozenEffectClass,
			FVector::ZeroVector,
			FRotator::ZeroRotator
		);
		frozenEffect->SetActorHiddenInGame(true);
		frozenEffect->SetActorEnableCollision(false);
		frozenEffect->SetActorTickEnabled(false);
		FrozenEffects.Add(frozenEffect);
	}
}

TObjectPtr<AActor> AFreezing::ActivateFrozenEffect(TObjectPtr<ABaseMonster> monster)
{
	AEdmundGameState* GameState = GetWorld() ? Cast<AEdmundGameState>(GetWorld()->GetGameState()) : nullptr;
	if (GameState != nullptr)
	{
		//GameState->PlayItemSound(AudioComponent, EItemType::HealKit)
	}
	TObjectPtr<AActor> frozen = FindDeactivateFrozenEffect();
	if (frozen == nullptr)
	{
		CreateFrozenEffect(10);
		frozen = FindDeactivateFrozenEffect();
		if (frozen == nullptr) return nullptr;
	}
	frozen->SetActorLocation(monster->GetActorLocation());
	frozen->SetActorHiddenInGame(false);
	frozen->SetActorEnableCollision(true);
	return frozen;
}

TObjectPtr<AActor> AFreezing::FindDeactivateFrozenEffect()
{
	for (auto frozen : FrozenEffects)
	{
		if (frozen->IsHidden())
		{
			return frozen;
		}
	}
	return nullptr;
}

void AFreezing::HitToMonster(TObjectPtr<ABaseMonster> Monster)
{
	Monster->FreezeMonster(FrozenTime);
	UGameplayStatics::ApplyDamage(
		Monster,
		3.f,
		nullptr,
		this,
		UDamageType::StaticClass()
	);
	TWeakObjectPtr<AActor> frozen = ActivateFrozenEffect(Monster);
	if (frozen == nullptr) return;
	FTimerHandle EndFreezingTimer;
	GetWorldTimerManager().SetTimer(
		EndFreezingTimer,
		[frozen, Monster] {
			if (frozen.IsValid())
			{
				frozen->SetActorHiddenInGame(true);
				frozen->SetActorEnableCollision(false);
			}
		},
		FrozenTime,
		false
	);
}

void AFreezing::UpgradeSkill()
{
	DamageMultiplier += DamageMultiplierAmount;
}

