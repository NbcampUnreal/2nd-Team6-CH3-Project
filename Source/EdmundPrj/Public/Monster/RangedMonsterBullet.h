// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "RangedMonsterBullet.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class EDMUNDPRJ_API ARangedMonsterBullet : public AActor
{
	GENERATED_BODY()
	
public:	

	ARangedMonsterBullet();

	float DamageValue = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Componenets")
	USphereComponent* CollisionComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Components")
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Components")
	UProjectileMovementComponent* ProjectileMovementComp;

	void SetMonsterBulletHidden(bool isHidden);

	void EndMonsterBulletLife();

	void SetDamage(float Damage);

	UFUNCTION()
	virtual void OnMonsterBulletOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	FTimerHandle MonsterBulletLifeTimerHandle;

};
