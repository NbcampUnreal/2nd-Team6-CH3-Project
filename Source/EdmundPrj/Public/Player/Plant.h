#pragma once

#include "CoreMinimal.h"
#include "Player/TimerSkill.h"
#include "Plant.generated.h"

class ABaseMonster;
class USplineComponent;
class USplineMeshComponent;

UCLASS()
class EDMUNDPRJ_API APlant : public ATimerSkill
{
	GENERATED_BODY()

public:
	APlant();
	virtual void BeginPlay() override;

	// 스플라인 관련 함수
	void MoveSplinePoint(int PointIndex, FVector NewLocation);
	void UpdateSplineMesh();
	void GenerateSplineMeshes();

	// 이동 관련 함수
	void StartSplineMove(FVector TargetLocation, float Duration);
	void UpdateSplineMove();

public:
	// 몬스터 저장
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Monster")
	TSet<TObjectPtr<ABaseMonster>> Monsters;

	// 스플라인 관련
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Spline")
	TObjectPtr<USplineComponent> Spline_1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Spline")
	TObjectPtr<USplineComponent> Spline_2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Spline")
	TObjectPtr<USplineComponent> Spline_3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	TObjectPtr<UStaticMesh> SplineStaticMesh = nullptr;
	TArray<TArray<TObjectPtr<USplineMeshComponent>>> SplineMeshs;

	// 이동 관련
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector AttackPoint = FVector();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector DefaultPoint = FVector();
	FTimerHandle MovementHandle;
	FVector StartLocation;
	FVector EndLocation;
	float MoveDuration = 0.5f;
	float ElapsedTime = 0.0f;
	bool bMovingForward = true;

};