// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/BaseItem.h"
#include "Player/BaseCharacter.h"
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
		ABaseCharacter* Player = Cast<ABaseCharacter>(OtherActor);
		if (Player)
		{
			SetActorHiddenInGame(true);
			CollisionComp->Deactivate();
			ActivateItem(OtherActor);
		}
	}
}

void ABaseItem::ActivateItem(AActor* Actor)
{
}

void ABaseItem::PlaySound()
{
	UE_LOG(LogTemp, Warning, TEXT("사운드재생"));
	PickupSoundComp->SetSound(PickupSound);

	float SoundDuration = PickupSoundComp->Sound->GetDuration();

	PickupSoundComp->Play();

	GetWorld()->GetTimerManager().SetTimer(SoundDurationTimerHandle, this, &ABaseItem::ItemDestroy, SoundDuration, false);
}

void ABaseItem::ItemDestroy()
{
	Destroy();
}
