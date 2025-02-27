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
	ProjectileMovementComponent->InitialSpeed = 15000.0f;
	ProjectileMovementComponent->MaxSpeed = 15000.0f;

	Collision->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnBulletOverlap);

	bIsHidden = false;
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
	bIsHidden = IsHidden;

	// 3초 뒤까지 오버랩 안될 경우 풀링
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
	SetActorHiddenInGame(bIsHidden);  // 총알 숨김 처리
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

	if (OtherActor && (OtherActor->ActorHasTag("Monster") || OtherActor->ActorHasTag("MissionItem")) && !bIsHidden)
	{
		UGameplayStatics::ApplyDamage(
			OtherActor,
			30.0f,	// 수정필요
			nullptr,
			this,
			UDamageType::StaticClass()
		);
	}

	GetWorld()->GetTimerManager().ClearTimer(BulletLifeTimerHandle);

	SetBulletHidden(true);
}
