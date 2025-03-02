#include "Player/Weapon.h"
#include "Player/BaseProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Player/BaseCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Particles/ParticleSystem.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	MuzzleOffset = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleOffset"));
	MuzzleOffset->SetupAttachment(Mesh);

	AttackDelay = 0.2f;
	IsAttack = false;
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

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
				AttackDelay = Player->GetAttackDelay();
			}
		}
	}

	// BulletPool�� �ʱ�ȭ�մϴ�.
	InitializeBulletPool(20);  // �Ѿ� Ǯ�� ũ�⸦ 20���� ����
}

void AWeapon::InitializeBulletPool(int32 PoolSize)
{
	FRotator SpawnRotation(0, 0, 0);
	FVector SpawnLocation(0, 0, -1000);
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	// Ǯ �ʱ�ȭ (�ʱ� Ǯ ũ�⸸ŭ �Ѿ��� ����)
	for (int32 i = 0; i < PoolSize; i++)
	{
		SpawnLocation.X += 2000;

		ABaseProjectile* NewBullet = GetWorld()->SpawnActor<ABaseProjectile>(BulletClass, SpawnLocation, SpawnRotation, SpawnParams);
		if (NewBullet)
		{
			NewBullet->SetActorHiddenInGame(true);  // �ʱ⿡�� �Ѿ��� ����
			BulletPool.Add(NewBullet);
		}
	}
}

ABaseProjectile* AWeapon::GetBulletFromPool()
{
	// ��� ������ �Ѿ��� Ǯ���� ������
	for (ABaseProjectile* ABaseProjectile : BulletPool)
	{
		if (ABaseProjectile && ABaseProjectile->IsHidden())
		{
			ABaseProjectile->SetBulletHidden(false);  // ���ܵ� �Ѿ��� ���̰� ����
			return ABaseProjectile;
		}
	}

	return nullptr;
}

void AWeapon::ReturnBulletToPool(ABaseProjectile* Bullet)
{
	// �Ѿ��� Ǯ�� ��ȯ�ϰ� ����
	if (Bullet)
	{
		Bullet->SetBulletHidden(true);
	}
}

bool AWeapon::Fire(float NewAttackDelay)
{
	if (IsAttack)
	{
		return false;
	}

	AttackDelay = NewAttackDelay;
	UAnimInstance* AnimInstance = Mesh->GetAnimInstance();

	// BulletPool���� �Ѿ��� ������
	ABaseProjectile* BulletToFire = GetBulletFromPool();
	if (BulletToFire)
	{
		FRotator SpawnRotation = MuzzleOffset->GetComponentRotation();
		FVector SpawnLocation = MuzzleOffset->GetComponentLocation();

		// ȭ�� �߾��� ��ǥ ���
		int32 x, y;
		APlayerController* PC = Cast<APlayerController>(Cast<ABaseCharacter>(GetOwner())->GetController());

		PC->GetViewportSize(x, y);

		// ȭ�� �߾� ��ǥ�� ��� �ش� �������� �Ѿ��� �߻�
		FVector WorldCenter;
		FVector WorldDirection;
		PC->DeprojectScreenPositionToWorld(x / 2.0f, y / 2.0f, WorldCenter, WorldDirection);

		// WorldCenter���� WorldDirection �������� �߻�
		FVector TargetLocation = WorldCenter + WorldDirection * 3500;
		SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, TargetLocation);

		BulletToFire->SetActorLocation(SpawnLocation);
		BulletToFire->SetActorRotation(SpawnRotation);

		// �Ѿ��� �̵� ���� ����
		FVector ForwardDirection = SpawnRotation.Vector();
		if (BulletToFire->ProjectileMovementComponent)
		{
			BulletToFire->ProjectileMovementComponent->Velocity = ForwardDirection * BulletToFire->ProjectileMovementComponent->InitialSpeed;
		}

		// �Ѿ˿� ������ ����
		IsAttack = true;

		GetWorld()->GetTimerManager().SetTimer(
			AttackDelayHandle,
			this,
			&AWeapon::ActivateAttack,
			AttackDelay,
			false
		);

		// �� ����Ʈ ���
		if (FireParticle)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FireParticle, MuzzleOffset->GetComponentLocation(), MuzzleOffset->GetComponentRotation());
		}
	}

	return true;
}

void AWeapon::ActivateAttack()
{
	IsAttack = false;
}
