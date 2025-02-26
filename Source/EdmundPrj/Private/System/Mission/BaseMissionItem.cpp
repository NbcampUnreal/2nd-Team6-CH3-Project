// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Mission/BaseMissionItem.h"
#include "System/MissionHandle.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/3DWidget/InteractionWidget.h"

ABaseMissionItem::ABaseMissionItem()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	SetRootComponent(CollisionComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	MeshComp->SetupAttachment(RootComponent);

	WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	WidgetComp->SetupAttachment(MeshComp);
	WidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	
	Tags.Add("MissionItem");
}

void ABaseMissionItem::InitMissionItem(AMissionHandle* NewMissionHandle, const FName& Type, const FString& MissionInfo)
{
	MissionHandle = NewMissionHandle;
	MissionType = Type;
	MissionText = MissionInfo;
	
	if (!IsValid(InteractionWidgetClass))
	{
		return;
	}

	WidgetComp->SetWidgetClass(InteractionWidgetClass);
	InteractionWidget = Cast<UInteractionWidget>(WidgetComp->GetUserWidgetObject());
	
	InteractionWidget->VisibleNotify(false);
	InteractionWidget->VisibleProgressBar(false);
}

void ABaseMissionItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseMissionItem::ActionBeginOverlap()
{
	ProgressValue = 1.0f;
	InteractionWidget->UpdateProgressBar(ProgressValue);
	MissionHandle->OnBeginOverlapedItem(this);
}

void ABaseMissionItem::ActionEndOverlap()
{
	bIsPlayingInteraction = false;
	MissionHandle->OnEndOverlapedItem();
}

void ABaseMissionItem::ActionOnHit()
{

}

void ABaseMissionItem::SetVisible(bool bIsVisible)
{
	if (bIsVisible)
	{
		SetActorHiddenInGame(false);
		SetActorTickEnabled(true);
		SetActorEnableCollision(true);
	}
	else
	{
		SetActorHiddenInGame(true);
		SetActorTickEnabled(false);
		SetActorEnableCollision(false);
	}
}

void ABaseMissionItem::ActionEventByPressedKey()
{
	bIsPlayingInteraction = true;
	InteractionWidget->VisibleNotify(false);
}

void ABaseMissionItem::UpdateMissionTextToUI()
{
	MissionHandle->RequestUpdateMissionText(MissionText);
}

void ABaseMissionItem::UpdateNotifyTextToUI()
{
	MissionHandle->RequestUpdateNotifyText(MissionText);
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
	if (!Hit.bBlockingHit)
	{
		return;
	}

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

void ABaseMissionItem::SetIsActive(bool Value)
{
	bIsActive = Value;
}

void ABaseMissionItem::PrintMissionText()
{
	if (MissionType == "Main")
	{
		UpdateMissionTextToUI();
	}
	else
	{
		UpdateNotifyTextToUI();
	}
}


