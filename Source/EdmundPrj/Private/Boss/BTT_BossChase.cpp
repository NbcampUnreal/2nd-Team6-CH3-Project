#include "Boss/BTT_BossChase.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "NavigationSystem.h"
#include "AI/NavigationSystemBase.h"
#include "AI/Navigation/NavigationTypes.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"

UBTT_BossChase::UBTT_BossChase()
{
    NodeName = "Boss Chase Player";
}

EBTNodeResult::Type UBTT_BossChase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

    AAIController* AICon = OwnerComp.GetAIOwner();
    if (!AICon)
    {
        return EBTNodeResult::Failed;
    }

    AActor* Player = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("Player"));
    if (!Player)
    {
        return EBTNodeResult::Failed;
    }


    EPathFollowingRequestResult::Type MoveResult = AICon->MoveToActor(Player, 50.0f);

    if (MoveResult == EPathFollowingRequestResult::RequestSuccessful)
    {
        return EBTNodeResult::Succeeded;
    }
    else
    {
        return EBTNodeResult::Failed;
    }
}


