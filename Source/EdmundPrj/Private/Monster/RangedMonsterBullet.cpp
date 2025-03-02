// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/RangedMonsterBullet.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARangedMonsterBullet::ARangedMonsterBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SetRootComponent(StaticMeshComp);

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	CollisionComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	CollisionComp->SetupAttachment(StaticMeshComp);

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComp->InitialSpeed = 10000.0f;
	ProjectileMovementComp->MaxSpeed = 10000.0f;

	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ARangedMonsterBullet::OnMonsterBulletOverlap);

	this->Tags.Add(FName("Bullet"));
}

void ARangedMonsterBullet::SetMonsterBulletHidden(bool isHidden)
{
	SetActorHiddenInGame(isHidden);

	if (!isHidden)
	{
		GetWorld()->GetTimerManager().SetTimer(
			MonsterBulletLifeTimerHandle,
			this,
			&ARangedMonsterBullet::EndMonsterBulletLife,
			1.5f,
			false
		);
	}
}

void ARangedMonsterBullet::EndMonsterBulletLife()
{
	AddActorLocalOffset(FVector(0.0f, 0.0f, -500.0f));
	SetMonsterBulletHidden(true);
}

void ARangedMonsterBullet::SetDamage(float Damage)
{
	DamageValue = Damage;
}

void ARangedMonsterBullet::OnMonsterBulletOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Skill")) return;

	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		UGameplayStatics::ApplyDamage(
			OtherActor,
			DamageValue,
			nullptr,
			nullptr,
			UDamageType::StaticClass()
		);

		GetWorld()->GetTimerManager().ClearTimer(MonsterBulletLifeTimerHandle);

		//UE_LOG(LogTemp, Warning, TEXT("%f"), DamageValue);

		EndMonsterBulletLife();
	}

	//if (OtherActor && !OtherActor->ActorHasTag("Monster"))
	//{
	//	EndMonsterBulletLife();
	//}
}
