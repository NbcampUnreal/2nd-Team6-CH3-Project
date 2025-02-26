#include "NPC/NPCAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void ANPCAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ANPCAIController::BeginPlay()
{
	Super::BeginPlay();
	if (BehaviorTreeAsset)
	{
		UE_LOG(LogTemp, Error, TEXT("asdsadasdasdadasda"));
		return;
	}
}

//void ANPCAIController::Chase()
//{
//	ANPC* NPCChar = Cast<ANPC>(GetPawn());
//	if (!NPCChar)
//	{
//		return;
//	}
//
//	MoveToActor(
//		PlayerPosition, //캐릭터 위치
//		5.0f, // 캐릭터와의 거리
//		false, // 도착 후 종료
//		true, // 장애물 회피
//		false, // 좌우 이동?
//		nullptr, // 이동 경로 필터
//		false // 부분경로
//	);
//}

void ANPCAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);
}