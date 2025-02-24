// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/RangedMonsterBullet.h"

// Sets default values
ARangedMonsterBullet::ARangedMonsterBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARangedMonsterBullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARangedMonsterBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

