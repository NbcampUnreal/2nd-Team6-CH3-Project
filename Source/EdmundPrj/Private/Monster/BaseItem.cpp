// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/BaseItem.h"
#include "Player/BaseCharacter.h"
#include "Components/PrimitiveComponent.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ABaseItem::ABaseItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SetRootComponent(StaticMeshComp);

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	CollisionComp->SetupAttachment(StaticMeshComp);

	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ABaseItem::OnItemOverlap);

	PickupSoundComp = CreateDefaultSubobject<UAudioComponent>(TEXT("PickupSound"));
	PickupSoundComp->SetupAttachment(StaticMeshComp);


}

void ABaseItem::BeginPlay()
{
	Super::BeginPlay();

	float XPulse = FMath::RandRange(-300.0f, 300.0f);
	float YPulse = FMath::RandRange(-300.0f, 300.0f);
	float ZPulse = FMath::RandRange(300.0f, 700.0f);

	StaticMeshComp->SetSimulatePhysics(true);  // 물리 활성화
	StaticMeshComp->AddImpulse(FVector(XPulse, YPulse, ZPulse), NAME_None, true);
}

void ABaseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator NewRotator = GetActorRotation();
	NewRotator.Yaw += 100.0f * DeltaTime;
	SetActorRotation(NewRotator);
}



void ABaseItem::OnItemOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsItemActived)
	{
		if (OtherActor && OtherActor->ActorHasTag("Player"))
		{
			ABaseCharacter* Player = Cast<ABaseCharacter>(OtherActor);
			if (Player)
			{
				SetActorHiddenInGame(true);
				ActivateItem(OtherActor);
				bIsItemActived = true;
			}
		}
	}
}

void ABaseItem::ActivateItem(AActor* Actor)
{
}

void ABaseItem::PlaySound()
{
	PickupSoundComp->SetSound(PickupSound);

	if (PickupSound)
	{
		float SoundDuration = PickupSoundComp->Sound->GetDuration();

		PickupSoundComp->Play();

		GetWorld()->GetTimerManager().SetTimer(SoundDurationTimerHandle, this, &ABaseItem::ItemDestroy, SoundDuration, false);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PickupSound가 없습니다."));
	}
}

void ABaseItem::ItemDestroy()
{
	Destroy();
}
