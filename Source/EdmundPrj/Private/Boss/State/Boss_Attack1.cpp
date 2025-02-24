#include "Boss/State/Boss_Attack1.h"
#include "Boss/Boss.h"
#include "Kismet/GameplayStatics.h"
#include "Boss/Attack/Boss_Attack1_Bullet.h"

UBoss_Attack1::UBoss_Attack1()
{
}

void UBoss_Attack1::EnterState(ABoss* Boss)
{
	Super::EnterState(Boss);
	UE_LOG(LogTemp, Log, TEXT("Boss is ATTACK1"));

	if (!Boss) return;
	BossRef = Boss;

	// 진입 시 2초 동안 정지(대기)
	BossRef->GetWorld()->GetTimerManager().SetTimer(RotationTimerHandle, this, &UBoss_Attack1::DelayedFire, 2.0f, false);
}

void UBoss_Attack1::DelayedFire()
{
	if (!BossRef) return;

	// 보스의 애님 인스턴스 캐스팅
	UBoss_AnimInstance* AnimInst = Cast<UBoss_AnimInstance>(BossRef->GetMesh()->GetAnimInstance());
	if (AnimInst && AnimInst->Attack1Montage)
	{
		// 몽타주 재생 시작: 몽타주 애셋은 애님 인스턴스의 Attack1Montage에 할당되어 있어야 함.
		float Duration = BossRef->GetMesh()->GetAnimInstance()->Montage_Play(AnimInst->Attack1Montage);
		UE_LOG(LogTemp, Log, TEXT("Attack1 montage playing for duration: %f"), Duration);

		// 몽타주 재생이 끝나면 상태 전환하기 위한 타이머 설정
		BossRef->GetWorld()->GetTimerManager().SetTimer(TransitionTimerHandle, this, &UBoss_Attack1::DelayedTransition, Duration, false);
	}
	else
	{
		// 몽타주 관련 코드가 없다면, 그냥 일정 시간 후 상태 전환 (옵션)
		BossRef->GetWorld()->GetTimerManager().SetTimer(TransitionTimerHandle, this, &UBoss_Attack1::DelayedTransition, 2.0f, false);
	}

	AActor* Player = UGameplayStatics::GetPlayerPawn(BossRef->GetWorld(), 0);
	if (!Player)
	{
		//UE_LOG(LogTemp, Error, TEXT("DelayedFire: Player is NULL"));
		return;
	}

	FVector PlayerLocation = Player->GetActorLocation();
	FVector BossLocation = BossRef->GetActorLocation();
	FVector Direction = (PlayerLocation - BossLocation).GetSafeNormal();
	FRotator TargetRotation = Direction.Rotation();

	BossRef->SetActorRotation(TargetRotation);
	//UE_LOG(LogTemp, Log, TEXT("Boss fully rotated towards %s"), *TargetRotation.ToString());

	// 탄환 발사
	FireBullet();

	// 탄환 발사 후 2초 대기한 후 다음 상태(Chase)로 전환
	BossRef->GetWorld()->GetTimerManager().SetTimer(TransitionTimerHandle, this, &UBoss_Attack1::DelayedTransition, 2.0f, false);
}

void UBoss_Attack1::DelayedTransition()
{
	if (!BossRef) return;
	BossRef->SetState(EBossState::Idle);
}

void UBoss_Attack1::ExitState()
{
	if (BossRef)
	{
		BossRef->GetWorld()->GetTimerManager().ClearTimer(RotationTimerHandle);
		BossRef->GetWorld()->GetTimerManager().ClearTimer(TransitionTimerHandle);
	}
	BossRef = nullptr;
	UE_LOG(LogTemp, Log, TEXT("Boss is exiting ATTACK1"));
}

void UBoss_Attack1::FireBullet()
{
	if (!BossRef || !BossRef->GetWorld() || !BossRef->MuzzleLocation)
	{
		UE_LOG(LogTemp, Error, TEXT("FireBullet: BossRef, World, or MuzzleLocation is NULL"));
		return;
	}

	if (!BossRef->Attack1BulletClass)
	{
		//UE_LOG(LogTemp, Error, TEXT("FireBullet: Attack1BulletClass is NULL"));
		return;
	}

	FVector SpawnLocation = BossRef->MuzzleLocation->GetComponentLocation();

	AActor* Player = UGameplayStatics::GetPlayerPawn(BossRef->GetWorld(), 0);
	if (!Player)
	{
		//UE_LOG(LogTemp, Error, TEXT("FireBullet: Player is NULL"));
		return;
	}

	FVector PlayerLocation = Player->GetActorLocation();
	FVector Direction = (PlayerLocation - SpawnLocation).GetSafeNormal();
	FRotator TargetRotation = Direction.Rotation();

	// 부드러운 회전 보간 (선택사항)
	FRotator NewRotation = FMath::RInterpTo(BossRef->GetActorRotation(), TargetRotation, BossRef->GetWorld()->GetDeltaSeconds(), 5.0f);
	BossRef->SetActorRotation(NewRotation);

	//UE_LOG(LogTemp, Log, TEXT("FireBullet: Boss rotating smoothly towards %s"), *NewRotation.ToString());

	// 탄환 풀에서 탄환을 가져오거나 새로 생성 후 FireProjectile() 호출
	ABoss_Attack1_Bullet* Bullet = ABoss_Attack1_Bullet::GetBulletFromPool(BossRef->GetWorld(), BossRef->Attack1BulletClass);

	if (Bullet)
	{
		Bullet->FireProjectile(SpawnLocation, TargetRotation, Direction);
		//UE_LOG(LogTemp, Log, TEXT("Bullet fired successfully at %s"), *Bullet->GetActorLocation().ToString());
	}
	else
	{
		//UE_LOG(LogTemp, Error, TEXT("Bullet fire failed"));
	}
}
