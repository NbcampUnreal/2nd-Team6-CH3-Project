// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TimerSkillSpawnVolumComponent.generated.h"

class USphereComponent;
class ATimerSkill;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EDMUNDPRJ_API UTimerSkillSpawnVolumComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTimerSkillSpawnVolumComponent();
	UFUNCTION()
	void BeginOverlaped(
		UPrimitiveComponent* overlappedComp,
		AActor* otherActor,
		UPrimitiveComponent* otherComp,
		int32 otherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	UFUNCTION()
	void EndOverlaped(
		UPrimitiveComponent* overlappedComponent,
		AActor* otherActor,
		UPrimitiveComponent* otherComp,
		int32 otherBodyIndex);
	FVector GetRandomMonsterLocation();
	void ActivateTimerSkill(TSubclassOf<ATimerSkill> timerSkill);
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Setting")
	TObjectPtr<USphereComponent> EnemySearchCollision = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float AttackRadius = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float DamageMultiplier = 0;
};
