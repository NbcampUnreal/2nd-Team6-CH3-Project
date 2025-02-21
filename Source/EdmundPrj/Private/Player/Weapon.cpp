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

	// BulletPool을 초기화합니다.
	InitializeBulletPool(20);  // 총알 풀의 크기를 20으로 설정
}

void AWeapon::InitializeBulletPool(int32 PoolSize)
{
	FRotator SpawnRotation(0, 0, 0);
	FVector SpawnLocation(0, 0, 0);
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	// 풀 초기화 (초기 풀 크기만큼 총알을 생성)
	for (int32 i = 0; i < PoolSize; i++)
	{
		SpawnLocation.X += 2000;

		ABullet* NewBullet = GetWorld()->SpawnActor<ABullet>(BulletClass, SpawnLocation, SpawnRotation, SpawnParams);
		if (NewBullet)
		{
			NewBullet->SetActorHiddenInGame(true);  // 초기에는 총알을 숨김
			BulletPool.Add(NewBullet);
		}
	}
}

ABullet* AWeapon::GetBulletFromPool()
{
	// 사용 가능한 총알을 풀에서 가져옴
	for (ABullet* Bullet : BulletPool)
	{
		if (Bullet && Bullet->IsHidden())
		{
			Bullet->SetBulletHidden(false);  // 숨겨둔 총알을 보이게 설정
			return Bullet;
		}
	}

	return nullptr;
}

void AWeapon::ReturnBulletToPool(ABullet* Bullet)
{
	// 총알을 풀로 반환하고 숨김
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

	// BulletPool에서 총알을 가져옵니다.
	ABullet* BulletToFire = GetBulletFromPool();
	if (BulletToFire)
	{
		FRotator SpawnRotation = MuzzleOffset->GetComponentRotation();
		FVector SpawnLocation = MuzzleOffset->GetComponentLocation();

		// 화면 중앙의 좌표 계산
		int32 x, y;
		APlayerController* PC = Cast<APlayerController>(Cast<ABaseCharacter>(GetOwner())->GetController());

		PC->GetViewportSize(x, y);

		// 화면 중앙 좌표를 얻어 해당 방향으로 총알을 발사
		FVector WorldCenter;
		FVector WorldDirection;
		PC->DeprojectScreenPositionToWorld(x / 2.0f, y / 2.0f, WorldCenter, WorldDirection);

		// WorldCenter에서 WorldDirection 방향으로 발사
		FVector TargetLocation = WorldCenter + WorldDirection * 10000; // 충분히 먼 거리로 설정
		SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, TargetLocation);

		BulletToFire->SetActorLocation(SpawnLocation + WorldDirection * 100);
		BulletToFire->SetActorRotation(SpawnRotation);

		// 총알의 이동 방향 설정
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
//	// BulletPool에서 총알을 가져옵니다.
//	ABullet* BulletToFire = GetBulletFromPool();
//	if (BulletToFire)
//	{
//		FRotator SpawnRotation = MuzzleOffset->GetComponentRotation();
//		FVector SpawnLocation = MuzzleOffset->GetComponentLocation();
//
//		//// 총알 위치 및 회전 설정
//		//BulletToFire->SetActorLocation(SpawnLocation);
//		//BulletToFire->SetActorRotation(SpawnRotation);
//
//		// 총알 발사 처리를 추가하려면 여기에 로직을 추가하세요.
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
