// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Thunder.h"
#include "Monster\BaseMonster.h"
#include "Player\TimerSkillSpawnManagerComponent.h"
#include "Player\ElectricEffectPool.h"
#include "Kismet\GameplayStatics.h"
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
	UGameplayStatics::ApplyDamage(
		Monster,
		30.0f,
		nullptr,
		this,
		UDamageType::StaticClass()
	);
	if (Monster)
	{
		ElectricEffectPool->ActivateElectricEffect(Monster->GetActorLocation());
	}
}

void AThunder::Deactivate()
{
	Super::Deactivate();
}


