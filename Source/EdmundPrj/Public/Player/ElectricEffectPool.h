// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ElectricEffectPool.generated.h"

class AElectric;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EDMUNDPRJ_API UElectricEffectPool : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UElectricEffectPool();
	void CreateElectricEffect(int createCount);
	TObjectPtr<AElectric> FindDeactivateElectricEffect();
	void ActivateElectricEffect(FVector monsterLocation, int ElectricCount);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	TSubclassOf<AElectric> ElectricEffectClass = nullptr;
	TArray<TObjectPtr<AElectric>> ElectricEffects = TArray<TObjectPtr<AElectric>>();
	FTimerHandle DeactivateCycleHandler;
};
