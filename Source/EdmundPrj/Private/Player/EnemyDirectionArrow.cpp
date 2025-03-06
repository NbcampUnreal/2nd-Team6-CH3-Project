// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/EnemyDirectionArrow.h"
#include "Monster\BaseMonster.h"
#include "Kismet\GameplayStatics.h"
#include "Player\BaseCharacter.h"
#include "Player\SupportCharacter.h"
// Sets default values
AEnemyDirectionArrow::AEnemyDirectionArrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(SceneComponent);
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	MeshComponent->SetupAttachment(RootComponent);
	
}

// Called every frame
void AEnemyDirectionArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector MyPos = GetActorLocation();
	FVector MonsterPos = Monster->GetActorLocation();
	FVector Direction = MonsterPos - MyPos;
	Direction.Z = 0;
	if (!Direction.IsNearlyZero())
	{
		FRotator NewRotation = Direction.Rotation();
		SetActorRotation(NewRotation);
	}
	float Distance = FVector::Distance(MyPos, MonsterPos);
	if (Distance > CollisionRadius / 2)
	{
		CurrentMaterial = DefaultMaterial;
	}
	else
	{
		CurrentMaterial = DangerMaterial;
	}
	if (Distance > CollisionRadius)
	{
		SupportChar->DeactivateArrow(this, Monster);
	}
	MeshComponent->SetMaterial(0, CurrentMaterial);
}