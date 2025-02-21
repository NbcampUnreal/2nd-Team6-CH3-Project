// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Freezing.h"
#include "Monster\BaseMonster.h"
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
	//Monster Ω∫≈œ
	TWeakObjectPtr<AActor> frozen = ActivateFrozenEffect(Monster);
	if (frozen == nullptr) return;
	FTimerHandle EndFreezingTimer;
	GetWorldTimerManager().SetTimer(
		EndFreezingTimer,
		[frozen] {
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
