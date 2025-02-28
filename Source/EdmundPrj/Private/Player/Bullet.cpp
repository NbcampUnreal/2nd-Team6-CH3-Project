#include "Player/Bullet.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Player/BaseCharacter.h"

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

	BulletLandParticle = nullptr;

	bIsHidden = true;
}

void ABullet::BeginPlay()
{
	Super::BeginPlay();

	SetBulletHidden(true);
}

void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABullet::SetBulletHidden(bool IsHidden)
{
	bIsHidden = IsHidden;

	// 3�� �ڱ��� ������ �ȵ� ��� Ǯ��
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
	SetActorHiddenInGame(bIsHidden);  // �Ѿ� ���� ó��
}

void ABullet::EndBulletLife()
{
	SetBulletHidden(true);
}

void ABullet::OnBulletOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor->ActorHasTag("Player")))
	{
		return;
	}

	if (OtherActor && (OtherActor->ActorHasTag("Monster") || OtherActor->ActorHasTag("MissionItem")) && !bIsHidden)
	{
		float Damage = 30.0f;

		// ���� ���� ��忡�� ���� ���� �÷��̾��� Pawn�� ���
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);  // 0�� ù ��° �÷��̾�
		if (PlayerController)
		{
			APawn* ControlledPawn = PlayerController->GetPawn();
			if (ControlledPawn)
			{
				ABaseCharacter* Player = Cast<ABaseCharacter>(ControlledPawn);

				if (IsValid(Player))
				{
					Damage = Player->GetAttackDamage();
				}
			}
		}

		UGameplayStatics::ApplyDamage(
			OtherActor,
			Damage,
			nullptr,
			this,
			UDamageType::StaticClass()
		);
	}

	GetWorld()->GetTimerManager().ClearTimer(BulletLifeTimerHandle);
	
	// ź�� ����Ʈ ���
	if (BulletLandParticle && !bIsHidden)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletLandParticle, GetActorLocation(), GetActorRotation());
	}

	SetBulletHidden(true);
}
