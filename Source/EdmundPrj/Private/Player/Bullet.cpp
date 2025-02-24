#include "Player/Bullet.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SetRootComponent(StaticMeshComponent);

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Collision->SetupAttachment(StaticMeshComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->InitialSpeed = 10000.0f;
	ProjectileMovementComponent->MaxSpeed = 10000.0f;

	Collision->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnBulletOverlap);
}

void ABullet::BeginPlay()
{
	Super::BeginPlay();
}

void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABullet::SetBulletHidden(bool IsHidden)
{
	// 3ÃÊ µÚ±îÁö ¿À¹ö·¦ ¾ÈµÉ °æ¿ì Ç®¸µ
	if (!IsHidden)
	{
		GetWorld()->GetTimerManager().SetTimer(
			BulletLifeTimerHandle,
			this,
			&ABullet::EndBulletLife,
			3.0f,
			false
		);
	}
	SetActorHiddenInGame(IsHidden);  // ÃÑ¾Ë ¼û±è Ã³¸®
}

void ABullet::EndBulletLife()
{
	SetBulletHidden(true);
}

void ABullet::OnBulletOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		return;
	}

	GetWorld()->GetTimerManager().ClearTimer(BulletLifeTimerHandle);

	SetBulletHidden(true);
}
