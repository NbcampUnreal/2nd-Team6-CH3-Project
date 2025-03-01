// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TimerSkill.h"
#include "Components\SphereComponent.h"
#include "Monster\BaseMonster.h"
#include "Player\TimerSkillSpawnManagerComponent.h"

// Sets default values
ATimerSkill::ATimerSkill()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;

	EnemySearchCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	EnemySearchCollision->SetCollisionProfileName(TEXT("OverlapAll"));
	EnemySearchCollision->SetupAttachment(RootComponent);
	EnemySearchCollision->OnComponentBeginOverlap.AddDynamic(this, &ATimerSkill::BeginOverlaped);
}

void ATimerSkill::BeginOverlaped(
	UPrimitiveComponent* overlappedComp, 
	AActor* otherActor, 
	UPrimitiveComponent* otherComp, 
	int32 otherBodyIndex, 
	bool bFromSweep, const 
	FHitResult& SweepResult)
{
	if (otherActor && otherActor->ActorHasTag("Monster"))
	{
		if (TObjectPtr<ABaseMonster> Monster = Cast<ABaseMonster>(otherActor))
		{
			HitToMonster(Monster);
		}
	}
	if (otherActor)
	{
		HitToGround();
	}
}

void ATimerSkill::HitToMonster(TObjectPtr<ABaseMonster> Monster)
{
}

void ATimerSkill::HitToGround()
{
}

void ATimerSkill::Deactivate()
{
	GetWorldTimerManager().ClearTimer(DestroyHandler);
	if (!IsValid(TimerSkillSpanwManager)) return;
	TimerSkillSpanwManager->DeactivateTimerSkill(this);

}

void ATimerSkill::SpawnTimerSkill()
{
	GetWorld()->GetTimerManager().SetTimer(DestroyHandler,
		this,
		&ATimerSkill::Deactivate,
		DeactivateTime,
		false);
}

// Called when the game starts or when spawned
void ATimerSkill::BeginPlay()
{
	Super::BeginPlay();
	
}


