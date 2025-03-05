#include "Player/Weapon.h"
#include "Player/BaseProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Player/BaseCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Particles/ParticleSystem.h"
#include "System/EdmundGameState.h"
#include "Player/ChargingBullet.h"
#include "Player/PlayerCharacterWraith.h"

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

	// 현재 게임 모드에서 조종 중인 플레이어의 Pawn을 얻기
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);  // 0은 첫 번째 플레이어
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

	// BulletPool을 초기화합니다.
	InitializeBulletPool(20);  // 총알 풀의 크기를 20으로 설정
}

void AWeapon::InitializeBulletPool(int32 PoolSize)
{
	FRotator SpawnRotation(0, 0, 0);
	FVector SpawnLocation(0, 0, -1000);
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	// 풀 초기화 (초기 풀 크기만큼 총알을 생성)
	for (int32 i = 0; i < PoolSize; i++)
	{
		SpawnLocation.X += 2000;

		ABaseProjectile* NewBullet = GetWorld()->SpawnActor<ABaseProjectile>(BulletClass, SpawnLocation, SpawnRotation, SpawnParams);
		if (NewBullet)
		{
			NewBullet->SetActorHiddenInGame(true);  // 초기에는 총알을 숨김
			BulletPool.Add(NewBullet);
		}
	}
}

ABaseProjectile* AWeapon::GetBulletFromPool()
{
	// 사용 가능한 총알을 풀에서 가져옴
	for (ABaseProjectile* ABaseProjectile : BulletPool)
	{
		if (ABaseProjectile && ABaseProjectile->IsHidden())
		{
			ABaseProjectile->SetBulletHidden(false);  // 숨겨둔 총알을 보이게 설정

			APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);  // 0은 첫 번째 플레이어

			if (PlayerController)
			{
				APawn* ControlledPawn = PlayerController->GetPawn();
				if (ControlledPawn)
				{
					ABaseCharacter* Player = Cast<ABaseCharacter>(ControlledPawn);
					if (IsValid(Player) && Player->GetCharacterType() == ECharacterType::Sparrow)
					{
						AChargingBullet* CharingBullet = Cast<AChargingBullet>(ABaseProjectile);
						APlayerCharacterWraith* Wraith = Cast<APlayerCharacterWraith>(Player);

						if (IsValid(CharingBullet))
						{
							CharingBullet->SetBulletScale();
						}
					}
				}
			}

			return ABaseProjectile;
		}
	}

	return nullptr;
}

void AWeapon::ReturnBulletToPool(ABaseProjectile* Bullet)
{
	// 총알을 풀로 반환하고 숨김
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

	// BulletPool에서 총알을 가져옴
	ABaseProjectile* BulletToFire = GetBulletFromPool();
	if (BulletToFire)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);  // 0은 첫 번째 플레이어

		if (PlayerController)
		{
			APawn* ControlledPawn = PlayerController->GetPawn();
			if (ControlledPawn)
			{
				ABaseCharacter* Player = Cast<ABaseCharacter>(ControlledPawn);
				if (IsValid(Player) && Player->GetCharacterType() == ECharacterType::Sparrow)
				{
					AChargingBullet* CharingBullet = Cast<AChargingBullet>(BulletToFire);
					APlayerCharacterWraith* Wraith = Cast<APlayerCharacterWraith>(Player);

					if (IsValid(CharingBullet))
					{
						CharingBullet->SetBulletDamage(Wraith->GetAttackDamage());
					}
				}
			}
		}

		FRotator SpawnRotation = MuzzleOffset->GetComponentRotation();
		FVector SpawnLocation = MuzzleOffset->GetComponentLocation();

		APlayerController* PC = Cast<APlayerController>(Cast<ABaseCharacter>(GetOwner())->GetController());

		FVector CamPos;
		FRotator CamRotator;
		PC->GetPlayerViewPoint(CamPos, CamRotator);
		FHitResult HitResult;
		FVector Start = CamPos; // 시작 위치
		FVector ForwardVector = CamRotator.Vector(); // 배우의 전방 벡터
		FVector End = Start + (ForwardVector * 20000.0f); // 1000 단위 앞까지 탐색

		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this); // 자기 자신은 감지에서 제외

		bool bHit = GetWorld()->LineTraceSingleByChannel(
			HitResult, // 충돌 결과 저장
			Start,     // 시작 위치
			End,       // 끝 위치
			ECC_Visibility, // 충돌 채널
			CollisionParams // 충돌 파라미터
		);

		if (bHit && HitResult.Distance > 350 && !HitResult.GetActor()->ActorHasTag("Water"))
		{
			FVector HitPos = HitResult.Location;
			FVector Direction = HitPos - SpawnLocation;
			if (!Direction.IsNearlyZero())
			{
				FRotator NewRotation = Direction.Rotation();
				SpawnRotation = NewRotation;
			}
		}
		else
		{
			// 화면 중앙의 좌표 계산
			int32 x, y;

			PC->GetViewportSize(x, y);

			// 화면 중앙 좌표를 얻어 해당 방향으로 총알을 발사
			FVector WorldCenter;
			FVector WorldDirection;
			PC->DeprojectScreenPositionToWorld(x / 2.0f, y / 2.0f, WorldCenter, WorldDirection);

			// WorldCenter에서 WorldDirection 방향으로 발사
			FVector TargetLocation = WorldCenter + WorldDirection * 3500;
			SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, TargetLocation);
		}
		

		BulletToFire->SetActorLocation(SpawnLocation);
		BulletToFire->SetActorRotation(SpawnRotation);

		// 총알의 이동 방향 설정
		FVector ForwardDirection = SpawnRotation.Vector();
		if (BulletToFire->ProjectileMovementComponent)
		{
			BulletToFire->ProjectileMovementComponent->Velocity = ForwardDirection * BulletToFire->ProjectileMovementComponent->InitialSpeed;
		}

		// 총알에 딜레이 설정
		IsAttack = true;

		GetWorld()->GetTimerManager().SetTimer(
			AttackDelayHandle,
			this,
			&AWeapon::ActivateAttack,
			AttackDelay,
			false
		);

		// 총 이펙트 재생
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
