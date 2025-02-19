// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ElectricEffect.h"
#include "Components\StaticMeshComponent.h"
#include "Components\SphereComponent.h"
#include "Monster\BaseMonster.h"
#include "Kismet\KismetMathLibrary.h"


AElectricEffect::AElectricEffect()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;

	EnemySearchCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));

	EnemySearchCollision->SetCollisionProfileName(TEXT("OverlapAll"));
	EnemySearchCollision->SetupAttachment(RootComponent);
	EnemySearchCollision->OnComponentBeginOverlap.AddDynamic(this, &AElectricEffect::FindMonster);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(EnemySearchCollision);

}

// Called when the game starts or when spawned
void AElectricEffect::BeginPlay()
{
	Super::BeginPlay();
	EnemySearchCollision->SetSphereRadius(AttackRadius);
	for (int i = 0; i < ElectricCount; i++)
	{
		TObjectPtr<AActor> electric = GetWorld()->SpawnActor<AActor>(ElectricClass, FVector::ZeroVector, FRotator::ZeroRotator);
		electric->SetActorTickEnabled(false);
		electric->SetActorHiddenInGame(true);
		electric->SetActorEnableCollision(false);
		Electrics.Add(electric);
	}
	GetWorldTimerManager().SetTimer(MoveTimer,
		[this] {
			SetActorLocation(GetActorLocation() + FVector(1, 0, 0));
		},
		0.1f,
		true);
}

void AElectricEffect::FindMonster(
	UPrimitiveComponent* overlappedComp,
	AActor* otherActor,
	UPrimitiveComponent* otherComp,
	int32 otherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	GetWorldTimerManager().ClearTimer(MoveTimer);
	if (TargetMonster != nullptr)
	{
		//SetActorLocation(TargetMonster->GetActorLocation());
	}
	if (CurrentElectricCount >= ElectricCount) return;
	if (otherActor && otherActor->ActorHasTag("Monster"))
	{
		TObjectPtr<ABaseMonster> monster = Cast<ABaseMonster>(otherActor);
		if (TargetMonster != nullptr)
		{
			if (monster == TargetMonster) return;
		}
		TargetMonster = monster;
		if (TargetMonster != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("IsMonster!!!!: %s"), *TargetMonster->GetName());
			MoveToMonster(TargetMonster);
		}
	}
}

void AElectricEffect::MoveToMonster(ABaseMonster* monster)
{
	FRotator direction = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), monster->GetActorLocation());
	ActivateElectric(monster->GetActorLocation(), direction);
	CurrentElectricCount++;
	SetActorRotation(direction);
	
	GetWorldTimerManager().SetTimer(MoveTimer,
		this,
		&AElectricEffect::Move,
		0.003f,
		true);
	Attack(monster);
}

void AElectricEffect::Move()
{
	SetActorLocation(GetActorLocation() + GetActorForwardVector() * 2.3f);
	float distance = FVector::Distance(GetActorLocation(), TargetMonster->GetActorLocation());
	if (distance < 5)
	{
		GetWorldTimerManager().ClearTimer(MoveTimer);
	}
}

void AElectricEffect::Attack(ABaseMonster* monster)
{
	//몬스터에게 데미지 입히기
	if (CurrentElectricCount >= ElectricCount)
	{
		GetWorldTimerManager().ClearTimer(MoveTimer);
		UE_LOG(LogTemp, Warning, TEXT("IsEnd!!!!"));
	}
}

TObjectPtr<AActor> AElectricEffect::ActivateElectric(FVector monsterPos, FRotator direction)
{
	if (CurrentElectricCount >= Electrics.Num()) return nullptr; // 배열 크기 확인

	TObjectPtr<AActor> electric = Electrics[CurrentElectricCount];
	if (!electric->IsHidden()) return nullptr;
	if (!electric) return nullptr; // nullptr 체크
	FString NewName = FString::Printf(TEXT("Electric %d"), CurrentElectricCount);
	electric->Rename(*NewName);
	electric->SetActorLabel(*NewName); 
	electric->SetActorLocation(monsterPos);
	electric->SetActorRotation(direction);
	electric->SetActorHiddenInGame(false);
	electric->SetActorEnableCollision(true);
	electric->SetActorTickEnabled(true);
	FTimerHandle DeactivateTimer;
	GetWorldTimerManager().SetTimer(DeactivateTimer,
		[electric, this] {
			DeactivateElectric(electric);
		},
		1.f,
		false);
	return electric;
}
void AElectricEffect::DeactivateElectric(TObjectPtr<AActor> electric)
{
	if (!electric) return; // nullptr 체크

	electric->SetActorHiddenInGame(true);
	electric->SetActorEnableCollision(false);
	electric->SetActorTickEnabled(false);
}

