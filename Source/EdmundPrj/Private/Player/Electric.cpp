// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Electric.h"
#include "Components\SphereComponent.h"
#include "Components\SplineComponent.h"
#include "Monster\BaseMonster.h"
#include "Kismet\GameplayStatics.h"
#include "Kismet\KismetMathLibrary.h"
#include "System\EdmundGameState.h"
#include "Components\AudioComponent.h"

// Sets default values
AElectric::AElectric()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	AudioComponent->SetupAttachment(RootComponent);

	EnemySearchCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	EnemySearchCollision->SetCollisionProfileName(TEXT("OverlapAll"));
	EnemySearchCollision->SetupAttachment(RootComponent);
	EnemySearchCollision->OnComponentBeginOverlap.AddDynamic(this, &AElectric::FindMonster);

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	SplineComponent->SetupAttachment(RootComponent);
	SplineComponent->SetClosedLoop(false);
}

// Called when the game starts or when spawned
void AElectric::BeginPlay()
{
	Super::BeginPlay();
	
}

void AElectric::FindMonster(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (otherActor && otherActor->ActorHasTag("Monster"))
	{
		if (TObjectPtr<ABaseMonster> Monster = Cast<ABaseMonster>(otherActor))
		{
			if (TargetMonster != nullptr && TargetMonster == Monster) return;

			if (CurrentElectricCount >= ElectricCount) return;

			TargetMonster = Monster;

			if (!isFirst)
			{
				isFirst = true;
				FirstEnemyPos = TargetMonster->GetActorLocation();
				SplineComponent->SetWorldLocation(FirstEnemyPos);
				SplineComponent->ClearSplinePoints();
				SplineComponent->UpdateSpline();

			}
			
			EnemyPosArray.Add(TargetMonster->GetActorLocation());
			MoveToMonster(TargetMonster);
		}
	}
}

void AElectric::MoveToMonster(ABaseMonster* monster)
{
	AddSplinePoint(monster->GetActorLocation());
	CurrentElectricCount++;
	FRotator Direction = UKismetMathLibrary::FindLookAtRotation(EnemySearchCollision->GetComponentLocation(), monster->GetActorLocation());
	EnemySearchCollision->SetWorldRotation(Direction);

	GetWorldTimerManager().SetTimer(
		MoveTimer,
		this,
		&AElectric::Move,
		0.1f,
		true
	);
}

void AElectric::Move()
{
	if (!IsValid(TargetMonster) || !IsValid(EnemySearchCollision))
	{
		GetWorldTimerManager().ClearTimer(MoveTimer);
		Deactivate();
		return;
	}
	FVector NewLocation = EnemySearchCollision->GetComponentLocation() + EnemySearchCollision->GetForwardVector() * Speed;
	EnemySearchCollision->SetWorldLocation(NewLocation);

	float Distance = FVector::Distance(EnemySearchCollision->GetComponentLocation(), TargetMonster->GetActorLocation());
}

void AElectric::Attack(ABaseMonster* monster)
{
	UGameplayStatics::ApplyDamage(
		monster,
		30.0f,
		nullptr,
		this,
		UDamageType::StaticClass()
	);
	AEdmundGameState* GameState = GetWorld() ? Cast<AEdmundGameState>(GetWorld()->GetGameState()) : nullptr;
	if (GameState != nullptr)
	{
		//GameState->PlayItemSound(AudioComponent, EItemType::);
	}
	if (CurrentElectricCount >= ElectricCount)
	{
		GetWorldTimerManager().ClearTimer(MoveTimer);
	}
}

void AElectric::Activate()
{
	UE_LOG(LogTemp, Warning, TEXT("%s is Activate"), *GetActorLabel());
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);

	FVector OwnerLocation = GetActorLocation();
	EnemySearchCollision->SetWorldLocation(OwnerLocation);
	UE_LOG(LogTemp, Warning, TEXT("Location Init Success"));
	FTimerHandle AutoDeactivateHandle;
	GetWorldTimerManager().SetTimer(
		AutoDeactivateHandle,
		[this] {
			if (IsValid(this) && !isFirst)
			{
				Deactivate();
			}
		},
		0.5f,
		false
	);
}

void AElectric::Deactivate()
{
	GetWorldTimerManager().ClearTimer(MoveTimer);
	UE_LOG(LogTemp, Warning, TEXT("%s is Deactivate"), *GetActorLabel());
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);

	isFirst = false;
	SplineComponent->ClearSplinePoints();
	SplineComponent->UpdateSpline();
	FirstEnemyPos = FVector();
	TargetMonster = nullptr;
	EnemyPosArray.Empty();
	CurrentElectricCount = 0;
	if (!IsValid(GetOwner())) return;
	EnemySearchCollision->SetWorldLocation(GetOwner()->GetActorLocation());
	SplineComponent->SetWorldLocation(GetOwner()->GetActorLocation());
}

void AElectric::ResetSplineRotation()
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

void AElectric::AddSplinePoint(FVector NewPoint)
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
		&AElectric::DeleteSplinePoint,
		PointDestroyTime,
		false
	);
	ResetSplineRotation();
}

void AElectric::DeleteSplinePoint()
{
	int32 NumPoints = SplineComponent->GetNumberOfSplinePoints();
	if (NumPoints <= 1)
	{
		Deactivate();
	}
	SplineComponent->RemoveSplinePoint(0, true);
	SplineComponent->UpdateSpline();
}
