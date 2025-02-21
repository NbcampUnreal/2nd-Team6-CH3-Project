// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Mjolnir.h"
#include "Player/ElectricEffect.h"
#include "Components\StaticMeshComponent.h"
#include "Components\SphereComponent.h"
#include "Monster\BaseMonster.h"
#include "Kismet\KismetMathLibrary.h"
#include "Components\SplineComponent.h"

AMjolnir::AMjolnir()
{
	ElectricEffectClass = nullptr;
	ElectricEffects.SetNum(0);
}

void AMjolnir::CreateElectricEffect(int createCount)
{
	if (!ElectricEffectClass) return;
	
	for (int i = 0; i < createCount; i++)
	{
		TObjectPtr<AElectricEffect> electricEffect = GetWorld()->SpawnActor<AElectricEffect>(
			ElectricEffectClass,
			FVector::ZeroVector,
			FRotator::ZeroRotator
		);
		electricEffect->Deactivate();
		ElectricEffects.Add(electricEffect);
	}

}

void AMjolnir::ActivateElectricEffect(TObjectPtr<ABaseMonster> monster)
{
	TObjectPtr<AElectricEffect> electric = FindDeactivateElectricEffect();
	if (electric == nullptr)
	{
		CreateElectricEffect(10);
		electric = FindDeactivateElectricEffect();
	}
	electric->SetActorLocation(monster->GetActorLocation());
	electric->SetActorHiddenInGame(false);
	electric->SetActorEnableCollision(true);
}

TObjectPtr<AElectricEffect> AMjolnir::FindDeactivateElectricEffect()
{
	for (auto electric : ElectricEffects)
	{
		if (electric->IsHidden())
		{
			return electric;
		}
	}
	return nullptr;
}

void AMjolnir::HitToMonster(TObjectPtr<ABaseMonster> monster)
{
	ActivateElectricEffect(monster);
}






