// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Mission/BaseMissionItem.h"
#include "System/MissionHandle.h"
#include "Components/BoxComponent.h"

ABaseMissionItem::ABaseMissionItem()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	SetRootComponent(CollisionComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	MeshComp->SetupAttachment(RootComponent);
}

void ABaseMissionItem::InitMissionItem(AMissionHandle* NewMissionHandle)
{
	MissionHandle = NewMissionHandle;
}

void ABaseMissionItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseMissionItem::ActionBeginOverlap()
{
}

void ABaseMissionItem::ActionEndOverlap()
{
}

void ABaseMissionItem::ActionOnHit()
{
}

void ABaseMissionItem::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor))
	{
		return;
	}

	if (!OtherActor->ActorHasTag("Player"))
	{
		return;
	}

	ActionBeginOverlap();
}

void ABaseMissionItem::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!IsValid(OtherActor))
	{
		return;
	}

	if (!OtherActor->ActorHasTag("Player"))
	{
		return;
	}

	ActionEndOverlap();
}

void ABaseMissionItem::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{


	ActionOnHit();
}

void ABaseMissionItem::ApplyOverlapCollision(bool bIsBlockedMesh)
{
	if (bIsBlockedMesh)
	{
		MeshComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	}
	else
	{
		MeshComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	}

	CollisionComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	CollisionComp->SetGenerateOverlapEvents(true);

	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);
	CollisionComp->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnEndOverlap);
}

void ABaseMissionItem::ApplyBlockCollision()
{
	MeshComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	CollisionComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	CollisionComp->SetNotifyRigidBodyCollision(true);

	CollisionComp->OnComponentHit.AddDynamic(this, &ThisClass::OnHit);
}


