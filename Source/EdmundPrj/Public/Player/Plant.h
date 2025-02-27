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

	// ���ö��� ���� �Լ�
	void MoveSplinePoint(int PointIndex, FVector NewLocation);
	void UpdateSplineMesh();
	void GenerateSplineMeshes();

	// �̵� ���� �Լ�
	void StartSplineMove(FVector TargetLocation, float Duration);
	void UpdateSplineMove();

public:
	// ���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Monster")
	TSet<TObjectPtr<ABaseMonster>> Monsters;

	// ���ö��� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Spline")
	TObjectPtr<USplineComponent> Spline_1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Spline")
	TObjectPtr<USplineComponent> Spline_2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Spline")
	TObjectPtr<USplineComponent> Spline_3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	TObjectPtr<UStaticMesh> SplineStaticMesh = nullptr;
	TArray<TArray<TObjectPtr<USplineMeshComponent>>> SplineMeshs;

	// �̵� ����
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