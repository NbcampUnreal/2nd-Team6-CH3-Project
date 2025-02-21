#include "Boss/Boss.h"
#include "Boss/State/Boss_Attack1.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"


//void ABoss_Attack1::ABoss_Attack1()
//{
//	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
//	RootComponent = MuzzleLocation;
//}
//
//void ABoss_Attack1::FireAttack()
//{
//	if (!BullectClass) return;
//
//	AActor* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
//	if (!Player) return;
//
//	FVector SpawnLocation = MuzzleLocation->GetComponentLocation();
//	FRotator SpawnRotation = (Player->GetActorLocation() - SpawnLocation).Rotation();
//
//	//ABoss_Attack1_Bullet* Bullet = GetWorld()->
//}




//void UBoss_Attack1::EnterState(ABoss* Boss) {
//    Super::EnterState(Boss);
//    UE_LOG(LogTemp, Log, TEXT("Boss is now ATTACKING"));
//
//    if (BossCharacter && BossCharacter->GetWorld()) {
//        BossCharacter->GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, [this]() {
//            BossCharacter->ChangeState(EBossState::Idle);
//            }, 3.0f, false);
//    }
//}
//
//void UBoss_Attack1::UpdateState(float DeltaTime) {
//    if (!BossCharacter) return;
//}
//
//void UBoss_Attack1::ExitState() {
//    UE_LOG(LogTemp, Log, TEXT("Boss is exiting ATTACK1"));
//    if (BossCharacter && BossCharacter->GetWorld()) {
//        BossCharacter->GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
//    }
//}
