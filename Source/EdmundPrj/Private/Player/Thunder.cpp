// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Thunder.h"
#include "Monster\BaseMonster.h"
#include "Player\TimerSkillSpawnManagerComponent.h"
#include "Player\ElectricEffectPool.h"
#include "Kismet\GameplayStatics.h"
#include "System\EdmundGameState.h"
AThunder::AThunder()
{
	PrimaryActorTick.bCanEverTick = false;
}
void AThunder::BeginPlay()
{
	Super::BeginPlay();
	if (!IsValid(GetWorld())) return;
	if (TObjectPtr<ACharacter> character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		TObjectPtr<UElectricEffectPool> electricEffectPool = character->FindComponentByClass<UElectricEffectPool>();
		if (!IsValid(electricEffectPool)) return;
		ElectricEffectPool = electricEffectPool;
	}
}
void AThunder::HitToMonster(TObjectPtr<ABaseMonster> Monster)
{
	if (IsValid(Monster))
	{
		UGameplayStatics::ApplyDamage(
			Monster,
			30.0f,
			nullptr,
			this,
			UDamageType::StaticClass());
		AEdmundGameState* GameState = GetWorld() ? Cast<AEdmundGameState>(GetWorld()->GetGameState()) : nullptr;
		if (GameState != nullptr)
		{
			GameState->PlayItemSound(AudioComponent, EItemType::Thunder, ESoundType::Attack);
		}
		ElectricEffectPool->ActivateElectricEffect(Monster->GetActorLocation(), ElectricCount);
	}
}

void AThunder::Deactivate()
{
	Super::Deactivate();
}

void AThunder::SpawnTimerSkill()
{
	Super::SpawnTimerSkill();
}

void AThunder::UpgradeSkill()
{
	ElectricCount++;
	DamageMultiplier += DamageMultiplierAmount;
}


