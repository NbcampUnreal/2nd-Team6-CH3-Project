#include "Player/Weapon.h"
#include "Player/Bullet.h"
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

	CurrentAudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	CurrentAudioComp->SetupAttachment(RootComponent);

	MuzzleOffset = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleOffset"));
	MuzzleOffset->SetupAttachment(Mesh);

	AttackDelay = 0.2f;
	IsAttack = false;
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	// BulletPool�� �ʱ�ȭ�մϴ�.
	InitializeBulletPool(20);  // �Ѿ� Ǯ�� ũ�⸦ 20���� ����
}

void AWeapon::InitializeBulletPool(int32 PoolSize)
{
	FRotator SpawnRotation(0, 0, 0);
	FVector SpawnLocation(0, 0, 0);
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	// Ǯ �ʱ�ȭ (�ʱ� Ǯ ũ�⸸ŭ �Ѿ��� ����)
	for (int32 i = 0; i < PoolSize; i++)
	{
		SpawnLocation.X += 2000;

		ABullet* NewBullet = GetWorld()->SpawnActor<ABullet>(BulletClass, SpawnLocation, SpawnRotation, SpawnParams);
		if (NewBullet)
		{
			NewBullet->SetActorHiddenInGame(true);  // �ʱ⿡�� �Ѿ��� ����
			BulletPool.Add(NewBullet);
		}
	}
}

ABullet* AWeapon::GetBulletFromPool()
{
	// ��� ������ �Ѿ��� Ǯ���� ������
	for (ABullet* Bullet : BulletPool)
	{
		if (Bullet && Bullet->IsHidden())
		{
			Bullet->SetBulletHidden(false);  // ���ܵ� �Ѿ��� ���̰� ����
			return Bullet;
		}
	}

	return nullptr;
}

void AWeapon::ReturnBulletToPool(ABullet* Bullet)
{
	// �Ѿ��� Ǯ�� ��ȯ�ϰ� ����
	if (Bullet)
	{
		Bullet->SetBulletHidden(true);
	}
}

bool AWeapon::Fire()
{
	if (IsAttack)
	{
		return false;
	}

	UAnimInstance* AnimInstance = Mesh->GetAnimInstance();

	if (IsValid(AnimInstance) && IsValid(FireMontage))
	{
		AnimInstance->Montage_Play(FireMontage);
	}

	// BulletPool���� �Ѿ��� ������
	ABullet* BulletToFire = GetBulletFromPool();
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
		FVector TargetLocation = WorldCenter + WorldDirection * 10000;
		SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, TargetLocation);

		BulletToFire->SetActorLocation(SpawnLocation + WorldDirection * 100);
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

		// �ѼҸ� ���
		if (FireSound)
		{
			//UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
			CurrentAudioComp->SetSound(FireSound);
			CurrentAudioComp->Play();
		}

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
