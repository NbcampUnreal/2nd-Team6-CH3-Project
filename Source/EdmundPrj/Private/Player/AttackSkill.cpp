// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AttackSkill.h"
#include "Components\SphereComponent.h"
#include "Monster\BaseMonster.h"
// Sets default values
AAttackSkill::AAttackSkill()
{
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;

	EnemySearchCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	EnemySearchCollision->SetCollisionProfileName(TEXT("OverlapAll"));
	EnemySearchCollision->SetupAttachment(RootComponent);
	EnemySearchCollision->OnComponentBeginOverlap.AddDynamic(this, &AAttackSkill::BeginOverlaped);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(EnemySearchCollision);
}

void AAttackSkill::ActivateProbCalculate()
{
	int RandomInt = FMath::RandRange(1, 100);
	if (RandomInt > ActivateProb)
	{
		ActivateSkill();
	}
}
void AAttackSkill::MoveForward()
{
	FVector NewLocation = GetActorLocation() + GetActorForwardVector() * MoveSpeed;
	SetActorLocation(NewLocation);

	float distance = FVector::Distance(NewLocation, FinishPos);
	if (distance < 10)
	{
		DeactivateSkill();
	}
}
void AAttackSkill::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
	MoveForward();
}
void AAttackSkill::DeactivateSkill()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
}
void AAttackSkill::ActivateSkill()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);
	FinishPos = GetActorLocation() + (GetActorForwardVector() * SkillRange);
}

void AAttackSkill::BeginOverlaped(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (otherActor && otherActor->ActorHasTag("Monster"))
	{
		TObjectPtr<ABaseMonster> Monster = Cast<ABaseMonster>(otherActor);
		if (Monster != nullptr)
		{
			HitToMonster(Monster);
		}
		
	}
	else if (otherActor && otherActor->ActorHasTag("Ground"))
	{
		DeactivateSkill();
	}
}

void AAttackSkill::HitToMonster(TObjectPtr<ABaseMonster> monster)
{
	
}

// Called when the game starts or when spawned
void AAttackSkill::BeginPlay()
{
	Super::BeginPlay();
	FinishPos = GetActorLocation() + (GetActorForwardVector() * SkillRange);
}


