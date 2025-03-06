// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AttackPlant.h"
#include "Monster\BaseMonster.h"
#include "Kismet\KismetMathLibrary.h"
#include "Components\SplineComponent.h"
#include "Kismet\GameplayStatics.h"
#include "Components\SphereComponent.h"
#include "System\EdmundGameState.h"
void AAttackPlant::BeginPlay()
{
	Super::BeginPlay();

}

void AAttackPlant::Attack()
{
	if (Monsters.IsEmpty())
	{
		StartSplineMove(GetActorTransform().TransformPosition(DefaultPoint), 1.f);
		TargetMonster = nullptr;
	}

	TargetMonster = FindCloseMonster();
	if (IsValid(TargetMonster))
	{
		StartSplineMove(TargetMonster->GetActorLocation(), 0.7f);
		GetWorldTimerManager().SetTimer(
			RotationHandle,
			this,
			&AAttackPlant::RotationToTarget,
			0.03f,
			true
		);
		GetWorldTimerManager().SetTimer(
			ApplyDamageHandle,
			this,
			&AAttackPlant::ApplyDamage,
			0.7f,
			false
		);
	}
}

void AAttackPlant::ApplyDamage()
{
	GetWorldTimerManager().ClearTimer(ApplyDamageHandle);
	if (!IsValid(TargetMonster)) return;
	UGameplayStatics::ApplyDamage(
		TargetMonster,
		30.0f,
		nullptr,
		this,
		UDamageType::StaticClass());
	AEdmundGameState* GameState = GetWorld() ? Cast<AEdmundGameState>(GetWorld()->GetGameState()) : nullptr;
	if (GameState != nullptr)
	{
		//GameState->PlayItemSound(AudioComponent)
	}
}

void AAttackPlant::HitToMonster(TObjectPtr<ABaseMonster> Monster)
{
	Monsters.Add(Monster);
}

TObjectPtr<ABaseMonster> AAttackPlant::FindCloseMonster()
{
	if (Monsters.IsEmpty()) return nullptr;
	TObjectPtr<ABaseMonster> CloseMonster = nullptr;
	float MinDistance = 500;
	TArray<AActor*> activators;
	EnemySearchCollision->GetOverlappingActors(activators);
	FVector MyLocation = GetActorLocation();
	for (AActor* monster : activators)
	{
		if (!IsValid(monster)) continue;
		if (monster->ActorHasTag("Monster"))
		{
			if (TObjectPtr<ABaseMonster> Monster = Cast<ABaseMonster>(monster))
			{
				float Distance = FVector::Dist(MyLocation, Monster->GetActorLocation());

				if (Distance < MinDistance)
				{
					MinDistance = Distance;
					CloseMonster = Monster;
				}
			}
		}
	}
	return CloseMonster;
}

void AAttackPlant::RotationToTarget()
{
	if (!IsValid(TargetMonster)) return;
	FRotator CurrentRotation = GetActorRotation();
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetMonster->GetActorLocation());
	FRotator SmoothRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), 5.0f);
	SetActorRotation(SmoothRotation);
	
	Spline_1->UpdateSpline();
	Spline_2->UpdateSpline();
	Spline_3->UpdateSpline();

	if (FMath::Abs(CurrentRotation.Yaw - TargetRotation.Yaw) < 1.0f)
	{
		GetWorldTimerManager().ClearTimer(RotationHandle);
	}
}

void AAttackPlant::UpgradeSkill()
{
	DamageMultiplier += DamageMultiplierAmount;
}

void AAttackPlant::SpawnTimerSkill()
{
	Super::SpawnTimerSkill();
	GetWorldTimerManager().SetTimer(
		AttackCycleHandle,
		this,
		&AAttackPlant::Attack,
		AttackSpeed,
		true
	);
}

void AAttackPlant::Deactivate()
{
	StartSplineMove(GetActorTransform().TransformPosition(DefaultPoint), 0);
	GetWorldTimerManager().ClearTimer(AttackCycleHandle);
	Super::Deactivate();
}
