// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TimerSkill.h"
#include "Components\SphereComponent.h"
#include "Monster\BaseMonster.h"
// Sets default values
ATimerSkill::ATimerSkill()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;

	EnemySearchCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	EnemySearchCollision->SetCollisionProfileName(TEXT("OverlapAll"));
	EnemySearchCollision->SetupAttachment(RootComponent);
	EnemySearchCollision->OnComponentBeginOverlap.AddDynamic(this, &ATimerSkill::BeginOverlaped);
	EnemySearchCollision->OnComponentEndOverlap.AddDynamic(this, &ATimerSkill::EndOverlaped);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(EnemySearchCollision);
}

void ATimerSkill::BeginOverlaped(
	UPrimitiveComponent* overlappedComp, 
	AActor* otherActor, 
	UPrimitiveComponent* otherComp, 
	int32 otherBodyIndex, 
	bool bFromSweep, const 
	FHitResult& SweepResult)
{

}

void ATimerSkill::EndOverlaped(
	UPrimitiveComponent* 
	overlappedComponent, 
	AActor* otherActor, 
	UPrimitiveComponent* otherComp, 
	int32 otherBodyIndex)
{

}

// Called when the game starts or when spawned
void ATimerSkill::BeginPlay()
{
	Super::BeginPlay();
	
}


