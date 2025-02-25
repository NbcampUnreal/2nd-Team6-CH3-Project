// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Mjolnir.h"
#include "Components\SphereComponent.h"
#include "Monster\BaseMonster.h"
#include "Player\ElectricEffectPool.h"
#include "Kismet\GameplayStatics.h"
AMjolnir::AMjolnir()
{

}

void AMjolnir::HitToMonster(TObjectPtr<ABaseMonster> monster)
{

	ElectricEffectPool->ActivateElectricEffect(monster->GetActorLocation());
}

void AMjolnir::BeginPlay()
{
	if (!IsValid(GetWorld())) return;
	if (TObjectPtr<ACharacter> character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		TObjectPtr<UElectricEffectPool> electricEffectPool = character->FindComponentByClass<UElectricEffectPool>();
		if (!IsValid(electricEffectPool)) return;
		ElectricEffectPool = electricEffectPool;
	}
}






