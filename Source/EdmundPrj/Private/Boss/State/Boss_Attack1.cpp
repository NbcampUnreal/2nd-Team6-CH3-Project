#include "Boss/State/Boss_Attack1.h"
#include "Boss/Boss.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

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
