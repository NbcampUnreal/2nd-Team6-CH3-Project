#include "Player/Plant.h"
#include "Monster\BaseMonster.h"
#include "Components\SplineComponent.h"
#include "Components\SplineMeshComponent.h"

APlant::APlant()
{
	SplineMeshs.Empty();
	Spline_1 = CreateDefaultSubobject<USplineComponent>(TEXT("Spline_1"));
	Spline_1->SetupAttachment(RootComponent);
	Spline_2 = CreateDefaultSubobject<USplineComponent>(TEXT("Spline_2"));
	Spline_2->SetupAttachment(RootComponent);
	Spline_3 = CreateDefaultSubobject<USplineComponent>(TEXT("Spline_3"));
	Spline_3->SetupAttachment(RootComponent);
}

void APlant::BeginPlay()
{
	GenerateSplineMeshes();
}

void APlant::MoveSplinePoint(int PointIndex, FVector NewLocation)
{
	TArray<TObjectPtr<USplineComponent>> Splines = { Spline_1, Spline_2, Spline_3 };
	for (int i = 0; i < Splines.Num(); i++)
	{
		if (Splines.IsValidIndex(i))
		{
			USplineComponent* Spline = Splines[i];
			if (Spline && Spline->IsValidLowLevel())
			{
				Spline->SetLocationAtSplinePoint(PointIndex, NewLocation, ESplineCoordinateSpace::World);
				Spline->UpdateSpline();
				UpdateSplineMesh();
			}
		}
	}
}

void APlant::StartSplineMove(FVector TargetLocation, float Duration)
{
	StartLocation = GetActorTransform().TransformPosition(Spline_2->GetLocationAtSplinePoint(2, ESplineCoordinateSpace::Local));
	EndLocation = TargetLocation;
	MoveDuration = Duration;
	ElapsedTime = 0.0f;
	bMovingForward = true;

	GetWorldTimerManager().SetTimer(
		MovementHandle,
		this,
		&APlant::UpdateSplineMove,
		0.016f,  // 약 60fps로 실행
		true
	);
}

void APlant::UpdateSplineMove()
{
	if (ElapsedTime >= MoveDuration)
	{
		GetWorldTimerManager().ClearTimer(MovementHandle);

		// 공격 후 다시 원래 위치로 돌아가기
		if (bMovingForward)
		{
			bMovingForward = false;
			StartSplineMove(GetActorTransform().TransformPosition(DefaultPoint), MoveDuration);
		}
		return;
	}

	float Alpha = ElapsedTime / MoveDuration;  // 0 ~ 1 사이 값
	float SpeedMultiplier = FMath::Clamp(1.0f + FMath::Pow(Alpha, 3.0f) * 10.0f, 1.0f, 10.0f);


	ElapsedTime += GetWorld()->GetDeltaSeconds() * SpeedMultiplier;


	FVector NewLocation = FMath::Lerp(StartLocation, EndLocation, Alpha);
	MoveSplinePoint(2, NewLocation);
}

void APlant::UpdateSplineMesh()
{
	TArray<TObjectPtr<USplineComponent>> Splines = { Spline_1, Spline_2, Spline_3 };
	for (int i = 0; i < Splines.Num(); i++)
	{
		USplineComponent* Spline = Splines[i];
		if (!Spline) return;

		for (int j = 0; j < SplineMeshs[i].Num(); j++)
		{
			if (USplineMeshComponent* SplineMesh = SplineMeshs[i][j])
			{
				FVector StartPos, StartTangent, EndPos, EndTangent;
				Spline->GetLocationAndTangentAtSplinePoint(j, StartPos, StartTangent, ESplineCoordinateSpace::Local);
				Spline->GetLocationAndTangentAtSplinePoint(j + 1, EndPos, EndTangent, ESplineCoordinateSpace::Local);

				SplineMesh->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent);
			}
		}
	}


}

void APlant::GenerateSplineMeshes()
{
	SplineMeshs.Empty();
	TArray<TObjectPtr<USplineComponent>> Splines = { Spline_1, Spline_2, Spline_3 };
	for (int j = 0; j < Splines.Num(); j++)
	{
		TArray<TObjectPtr<USplineMeshComponent>> splineMeshs;
		for (int i = 0; i < Splines[j]->GetNumberOfSplinePoints() - 1; i++)
		{
			USplineMeshComponent* SplineMesh = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
			if (SplineMesh)
			{
				SplineMesh->RegisterComponent();

				FVector StartPos, StartTangent, EndPos, EndTangent;

				SplineMesh->SetMobility(EComponentMobility::Movable);
				Splines[j]->GetLocationAndTangentAtSplinePoint(i, StartPos, StartTangent, ESplineCoordinateSpace::World);
				Splines[j]->GetLocationAndTangentAtSplinePoint(i + 1, EndPos, EndTangent, ESplineCoordinateSpace::World);

				FVector RelativeStartPos = StartPos - GetActorLocation();
				FVector RelativeEndPos = EndPos - GetActorLocation();

				SplineMesh->AttachToComponent(Splines[j], FAttachmentTransformRules::KeepRelativeTransform);

				SplineMesh->SetWorldLocation(GetActorLocation());

				StartTangent = StartTangent.GetClampedToSize(0, FVector::Dist(StartPos, EndPos) * 0.5f);
				EndTangent = EndTangent.GetClampedToSize(0, FVector::Dist(StartPos, EndPos) * 0.5f);

				SplineMesh->SetStartAndEnd(RelativeStartPos, StartTangent, RelativeEndPos, EndTangent);

				SplineMesh->SetStaticMesh(SplineStaticMesh);
				SplineMesh->SetForwardAxis(ESplineMeshAxis::Z, true);
				splineMeshs.Add(SplineMesh);
			}
		}
		SplineMeshs.Add(splineMeshs);
	}
}