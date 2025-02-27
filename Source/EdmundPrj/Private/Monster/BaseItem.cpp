// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/BaseItem.h"
#include "Player/PlayerCharacter.h"
#include "Components/SphereComponent.h"

// Sets default values
ABaseItem::ABaseItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SetRootComponent(StaticMeshComp);

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	CollisionComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	CollisionComp->SetupAttachment(StaticMeshComp);

	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ABaseItem::OnItemOverlap);

	PickupSoundComp = CreateDefaultSubobject<UAudioComponent>(TEXT("PickupSound"));
	PickupSoundComp->SetupAttachment(StaticMeshComp);
}

void ABaseItem::OnItemOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
		if (Player)
		{
			ActivateItem(OtherActor);
			Destroy();
		}
	}
}

void ABaseItem::ActivateItem(AActor* Actor)
{
}
