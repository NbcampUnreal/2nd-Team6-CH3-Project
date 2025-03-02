// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ElectricEffectPool.h"
#include "Player/Electric.h"
#include "Components\SplineComponent.h"
// Sets default values for this component's properties
UElectricEffectPool::UElectricEffectPool()
{
	PrimaryComponentTick.bCanEverTick = false;
	
	// ...
}

void UElectricEffectPool::CreateElectricEffect(int createCount)
{
    UWorld* World = GetWorld();
    if (!World || !ElectricEffectClass) return;

    for (int i = 0; i < createCount; i++)
    {
        TObjectPtr<AElectric> electric = World->SpawnActor<AElectric>(ElectricEffectClass);
        if (!electric) continue;

        electric->SetActorHiddenInGame(true);
        electric->SetActorEnableCollision(false);
        electric->SetActorTickEnabled(false);
        electric->ElectricEffectPool = this;

        ElectricEffects.Add(electric);
    }
}

TObjectPtr<AElectric> UElectricEffectPool::FindDeactivateElectricEffect()
{
    for (TObjectPtr<AElectric> electric : ElectricEffects)
    {
        if (electric->IsHidden())
        {
            return electric;
        }
    }
    return nullptr;
}

void UElectricEffectPool::ActivateElectricEffect(FVector monsterLocation, int ElectricCount)
{
    TObjectPtr<AElectric> electric = FindDeactivateElectricEffect();

    if (!electric)
    {
        CreateElectricEffect(10);
        electric = FindDeactivateElectricEffect();
    }
    if (!electric) return;
    electric->ElectricCount = ElectricCount;
    electric->SetActorLocation(monsterLocation);
    electric->Activate();
}

void UElectricEffectPool::BeginPlay()
{
    Super::BeginPlay();
}

void UElectricEffectPool::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    GetWorld()->GetTimerManager().ClearTimer(DeactivateCycleHandler);
}

