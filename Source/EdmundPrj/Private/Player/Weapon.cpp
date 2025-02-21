#include "Player/Weapon.h"
#include "Player/Bullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Player/BaseCharacter.h"
#include "Kismet/KismetMathLibrary.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	MuzzleOffset = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleOffset"));
	MuzzleOffset->SetupAttachment(Mesh);
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

void AWeapon::Fire()
{
	UAnimInstance* AnimInstance = Mesh->GetAnimInstance();

	if (IsValid(AnimInstance) && IsValid(FireMontage))
	{
		AnimInstance->Montage_Play(FireMontage);
	}

	// BulletPool���� �Ѿ��� �����ɴϴ�.
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
		FVector TargetLocation = WorldCenter + WorldDirection * 10000; // ����� �� �Ÿ��� ����
		SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, TargetLocation);

		BulletToFire->SetActorLocation(SpawnLocation + WorldDirection * 100);
		BulletToFire->SetActorRotation(SpawnRotation);

		// �Ѿ��� �̵� ���� ����
		FVector ForwardDirection = SpawnRotation.Vector();
		if (BulletToFire->ProjectileMovementComponent)
		{
			BulletToFire->ProjectileMovementComponent->Velocity = ForwardDirection * BulletToFire->ProjectileMovementComponent->InitialSpeed;
		}
	}
}


//void AWeapon::Fire()
//{
//	UAnimInstance* AnimInstance = Mesh->GetAnimInstance();
//
//	if (IsValid(AnimInstance) && IsValid(FireMontage))
//	{
//		AnimInstance->Montage_Play(FireMontage);
//	}
//	
//	// BulletPool���� �Ѿ��� �����ɴϴ�.
//	ABullet* BulletToFire = GetBulletFromPool();
//	if (BulletToFire)
//	{
//		FRotator SpawnRotation = MuzzleOffset->GetComponentRotation();
//		FVector SpawnLocation = MuzzleOffset->GetComponentLocation();
//
//		//// �Ѿ� ��ġ �� ȸ�� ����
//		//BulletToFire->SetActorLocation(SpawnLocation);
//		//BulletToFire->SetActorRotation(SpawnRotation);
//
//		// �Ѿ� �߻� ó���� �߰��Ϸ��� ���⿡ ������ �߰��ϼ���.
//		int32 x, y;
//
//		APlayerController* PC = Cast<APlayerController>(Cast<ABaseCharacter>(GetOwner())->GetController());
//
//		PC->GetViewportSize(x, y);
//
//		FVector WorldCenter;
//		FVector WorldFront;
//
//		PC->DeprojectScreenPositionToWorld(x / 2.0f, y / 2.0f, WorldCenter, WorldFront);
//
//		WorldCenter += WorldFront * 10000;
//		SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, WorldCenter);
//		BulletToFire->SetActorLocation(SpawnLocation);
//		BulletToFire->SetActorRotation(SpawnRotation);
//	}
//
//	
//}
	/*void AWeapon::Fire()
	{

		if (IsValid(Bullet))
		{
			FRotator SpawnRotation = MuzzleOffset->GetComponentRotation();
			FVector SpawnLocation = MuzzleOffset->GetComponentLocation();

			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;

			ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(GetOwner());

			if (!IsValid(BaseCharacter))
			{
				GetWorld()->SpawnActor<ABullet>(Bullet, SpawnLocation, SpawnRotation, SpawnParams);
				return;
			}

			APlayerController* PC = Cast<APlayerController>(BaseCharacter->GetController());
			int32 x, y;

			if (!IsValid(PC))
			{
				GetWorld()->SpawnActor<ABullet>(Bullet, SpawnLocation, SpawnRotation, SpawnParams);
				return;
			}

			PC->GetViewportSize(x, y);

			FVector WorldCenter;
			FVector WorldFront;

			PC->DeprojectScreenPositionToWorld(x / 2.0f, y / 2.0f, WorldCenter, WorldFront);

			WorldCenter += WorldFront * 10000;
			SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, WorldCenter);
			GetWorld()->SpawnActor<ABullet>(Bullet, SpawnLocation, SpawnRotation, SpawnParams);
		}
	}*/
