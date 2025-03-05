// Fill out your copyright notice in the Description page of Project Settings.


#include "System/MainLevelPawn.h"
#include "Camera/CameraComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/SpotLightComponent.h"
#include "Particles/ParticleSystem.h"
#include "System/MainLevelPlayerController.h"

// Sets default values
AMainLevelPawn::AMainLevelPawn()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Comp"));
	SetRootComponent(SceneComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	MeshComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Main Camera"));
	CameraComp->SetupAttachment(RootComponent);

	LightComp = CreateDefaultSubobject<UPointLightComponent>(TEXT("Point Light"));
	LightComp->SetupAttachment(RootComponent);

	SpotLightComp = CreateDefaultSubobject<USpotLightComponent>(TEXT("Spot Light"));
	SpotLightComp->SetupAttachment(RootComponent);
}

void AMainLevelPawn::BeginPlay()
{
	Super::BeginPlay();
}


