#include "NPC/NPC.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NPC/NPCAIController.h"

ANPC::ANPC()
{
	PrimaryActorTick.bCanEverTick = false;
	AIControllerClass = ANPCAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	BaseEyeHeight = 64.0f;
}

void ANPC::BeginPlay()
{
	Super::BeginPlay();


}


