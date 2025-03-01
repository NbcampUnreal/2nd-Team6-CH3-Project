// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NPCAttack.generated.h"

class UBoxComponent;

UCLASS()
class EDMUNDPRJ_API ANPCAttack : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANPCAttack();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|Components")
	USceneComponent* SceneComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|Components")
	UBoxComponent* BoxComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC|Value")
	float DamageValue = 10.0f;

	FTimerHandle NPCAttackTimerHandle;

	UFUNCTION()
	virtual void OnOverlapAttackBegin(
		UPrimitiveComponent* OverlappedComp, //동전
		AActor* OtherActor, //플레이어
		UPrimitiveComponent* OtherComp, //플레이어의 Collision(Capsule Component)
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	void DestroyMe();
};
