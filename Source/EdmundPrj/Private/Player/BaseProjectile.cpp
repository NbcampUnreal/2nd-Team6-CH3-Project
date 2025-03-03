#include "Player/BaseProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/BaseCharacter.h"

ABaseProjectile::ABaseProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SetRootComponent(StaticMeshComponent);

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Collision->SetupAttachment(StaticMeshComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));

	BulletSpeed = 10000.0f;
	ProjectileMovementComponent->InitialSpeed = BulletSpeed;
	ProjectileMovementComponent->MaxSpeed = BulletSpeed;

	BulletDuraion = 3.0f;

	Collision->OnComponentBeginOverlap.AddDynamic(this, &ABaseProjectile::OnProjectileOverlap);

	BulletLandParticle = nullptr;

	bIsHidden = true;
}

void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMovementComponent->InitialSpeed = BulletSpeed;
	ProjectileMovementComponent->MaxSpeed = BulletSpeed;

	SetBulletHidden(true);
}

void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector CurrentProjectileLocation = GetActorLocation();

	// Sweep �浹 �˻�
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this); // �Ѿ� ��ü�� ����

	// SweepMultiByChannel�� ����Ͽ� �浹�� �˻�
	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,    // �浹 ���
		PrevProjectileLocation,        // ���� ��ġ
		CurrentProjectileLocation,          // �� ��ġ (�̵� ��)
		FQuat::Identity,  // ȸ���� (ȸ�� ���� �̵�)
		ECC_Visibility,   // �浹 ä��
		FCollisionShape::MakeSphere(32.0f)  // ���� ���� (��ü ����)
	);

	if (bHit)
	{
		// �浹�� ��ü�� �ִٸ�
		OnProjectileOverlap(nullptr, HitResult.GetActor(), nullptr, 0, true, HitResult);
	}
}

void ABaseProjectile::SetBulletHidden(bool IsHidden)
{
	bIsHidden = IsHidden;
	// 3�� �ڱ��� ������ �ȵ� ��� Ǯ��
	if (!IsHidden)
	{
		PrevProjectileLocation = GetActorLocation();
		PrimaryActorTick.bCanEverTick = true;

		GetWorld()->GetTimerManager().SetTimer(
			BulletLifeTimerHandle,
			this,
			&ABaseProjectile::EndBulletLife,
			BulletDuraion,
			false
		);
	}
	else
	{
		PrimaryActorTick.bCanEverTick = false;
	}
	SetActorHiddenInGame(bIsHidden);  // �Ѿ� ���� ó��
}

void ABaseProjectile::EndBulletLife()
{
	SetBulletHidden(true);
}

void ABaseProjectile::OnProjectileOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsHidden || (OtherActor && (OtherActor->ActorHasTag("Player") || OtherActor->ActorHasTag("Skill") || OtherActor->ActorHasTag("Bullet"))))
	{
		return;
	}

	// �浹 �߻� �� ó��
	if (BulletLandParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletLandParticle, GetActorLocation(), GetActorRotation());
	}

	SetBulletHidden(true);

	if (OtherActor && (OtherActor->ActorHasTag("Monster") || OtherActor->ActorHasTag("MissionItem")))
	{
		float Damage = 30.0f;

		// ���� �÷��̾� ĳ���ͷκ��� ������ ���� ��� �ڵ�
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

		// ������ ����
		UGameplayStatics::ApplyDamage(OtherActor, Damage, nullptr, this, UDamageType::StaticClass());
	}

	GetWorld()->GetTimerManager().ClearTimer(BulletLifeTimerHandle);
}