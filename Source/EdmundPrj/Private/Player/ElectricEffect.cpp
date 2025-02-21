// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ElectricEffect.h"
#include "Components\SphereComponent.h"
#include "Monster\BaseMonster.h"
#include "Kismet\KismetMathLibrary.h"
#include "Components\SplineComponent.h"

AElectricEffect::AElectricEffect()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;

	EnemySearchCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	EnemySearchCollision->SetCollisionProfileName(TEXT("OverlapAll"));
	EnemySearchCollision->SetupAttachment(RootComponent);
	EnemySearchCollision->OnComponentBeginOverlap.AddDynamic(this, &AElectricEffect::FindMonster);

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	SplineComponent->SetupAttachment(RootComponent);
	SplineComponent->SetClosedLoop(false);
}

void AElectricEffect::BeginPlay()
{
	Super::BeginPlay();
	EnemySearchCollision->SetSphereRadius(AttackRadius);
}

void AElectricEffect::FindMonster(
	UPrimitiveComponent* overlappedComp,
	AActor* otherActor,
	UPrimitiveComponent* otherComp,
	int32 otherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (CurrentElectricCount >= ElectricCount) return;
	if (otherActor && otherActor->ActorHasTag("Monster"))
	{
		if (TObjectPtr<ABaseMonster> monster = Cast<ABaseMonster>(otherActor))
		{
			if (TargetMonster != nullptr && TargetMonster == monster) return;

			TargetMonster = monster;
			if (!isFirst)
			{
				isFirst = true;
				FirstEnemyPos = TargetMonster->GetActorLocation();
				SplineComponent->SetWorldLocation(FirstEnemyPos);
				SplineComponent->ClearSplinePoints(true);
				SplineComponent->UpdateSpline();
			}
			EnemyPosArray.Add(TargetMonster->GetActorLocation());
			MoveToMonster(TargetMonster);
		}
	}
}


void AElectricEffect::MoveToMonster(ABaseMonster* monster)
{
	FRotator direction = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), monster->GetActorLocation());
	AddSplinePoint(monster->GetActorLocation());
	CurrentElectricCount++;
	EnemySearchCollision->SetWorldRotation(direction);
	
	GetWorldTimerManager().SetTimer(
		MoveTimer,
		this,
		&AElectricEffect::Move,
		0.1f,
		true);
	Attack(monster);
}

void AElectricEffect::Move()
{
	if (!TargetMonster || !EnemySearchCollision)
	{
		GetWorldTimerManager().ClearTimer(MoveTimer);
		return;
	}

	FVector NewLocation = EnemySearchCollision->GetComponentLocation() + EnemySearchCollision->GetForwardVector() * Speed;
	EnemySearchCollision->SetWorldLocation(NewLocation);

	float Distance = FVector::Distance(EnemySearchCollision->GetComponentLocation(), TargetMonster->GetActorLocation());
	if (Distance < 10.0f)
	{
		GetWorldTimerManager().ClearTimer(MoveTimer);
	}
}

void AElectricEffect::Attack(ABaseMonster* monster)
{
	/*
	몬스터에게 데미지 입히기 구현
	*/

	if (CurrentElectricCount >= ElectricCount)
	{
		GetWorldTimerManager().ClearTimer(MoveTimer);
	}
}

void AElectricEffect::Deactivate()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
	CurrentElectricCount = 0;
	TargetMonster = nullptr;
}

void AElectricEffect::AddSplinePoint(FVector NewPoint)
{
	if (!SplineComponent) return;

	SplineComponent->AddSplinePoint(FirstEnemyPos, ESplineCoordinateSpace::World, true);

	int32 NumPoints = SplineComponent->GetNumberOfSplinePoints();
	for (int i = 0; i < NumPoints; i++)
	{
		SplineComponent->SetLocationAtSplinePoint(i, EnemyPosArray[i], ESplineCoordinateSpace::World, true);
	}
	FTimerHandle SplinePointDeleteTimer;
	GetWorldTimerManager().SetTimer(
		SplinePointDeleteTimer,
		this,
		&AElectricEffect::DeleteSplinePoint,
		PointDestroyTime,
		false
	);
	ResetSplineRotation(); 
}

void AElectricEffect::DeleteSplinePoint()
{
	int32 NumPoints = SplineComponent->GetNumberOfSplinePoints();
	if (NumPoints <= 1)
	{
		Deactivate();
	}
	SplineComponent->RemoveSplinePoint(0, true);
	SplineComponent->UpdateSpline();
}

void AElectricEffect::ResetSplineRotation()
{
	if (!SplineComponent) return;

	int32 NumPoints = SplineComponent->GetNumberOfSplinePoints();

	for (int32 i = 0; i < NumPoints; i++)
	{
		SplineComponent->SetSplinePointType(i, ESplinePointType::Linear, true); 
		SplineComponent->SetTangentAtSplinePoint(i, FVector::ZeroVector, ESplineCoordinateSpace::Local); 
	}

	SplineComponent->UpdateSpline();
}
