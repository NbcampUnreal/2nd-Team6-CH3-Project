// Fill out your copyright notice in the Description page of Project Settings.


#include "System/SceneHandle.h"
#include "Kismet/GameplayStatics.h"

void USceneHandle::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UE_LOG(LogTemp, Warning, TEXT("Init SceneHandle"));

	CurrentScene = ESceneName::Title;
}

void USceneHandle::MoveNextScene()
{
	switch (CurrentScene)
	{
	case ESceneName::Title:
		OpenScene(ESceneName::Main);
		break;

	case ESceneName::Mission1:
		OpenScene(ESceneName::Mission2);
		break;

	case ESceneName::Mission2:
		OpenScene(ESceneName::Mission3);
		break;

	case ESceneName::Mission3:
		OpenScene(ESceneName::Ending);
		break;

	case ESceneName::Infinity:
		OpenScene(ESceneName::Main);
		break;

	case ESceneName::Ending:
		OpenScene(ESceneName::Title);
		break;
	}
}

void USceneHandle::OpenScene(ESceneName SceneName)
{
	switch (CurrentScene)
	{
	case ESceneName::Title:
		UGameplayStatics::OpenLevel(GetWorld(), TitleSceneName);
		break;

	case ESceneName::Main:
		UGameplayStatics::OpenLevel(GetWorld(), MainSceneName);
		break;

	case ESceneName::Mission1:
		UGameplayStatics::OpenLevel(GetWorld(), Mission1SceneName);
		break;

	case ESceneName::Mission2:
		UGameplayStatics::OpenLevel(GetWorld(), Mission2SceneName);
		break;

	case ESceneName::Mission3:
		UGameplayStatics::OpenLevel(GetWorld(), Mission3SceneName);
		break;

	case ESceneName::Infinity:
		UGameplayStatics::OpenLevel(GetWorld(), InfinitySceneName);
		break;

	case ESceneName::Ending:
		UGameplayStatics::OpenLevel(GetWorld(), EndingSceneName);
		break;
	}
}

ESceneName USceneHandle::GetCurrentScene() const
{
	return CurrentScene;
}

void USceneHandle::CheckCurrentScene()
{
	FName LevelName(GetWorld()->GetMapName());

	if (LevelName == TitleSceneName)
	{
		CurrentScene = ESceneName::Title;
	}
	else if (LevelName == MainSceneName)
	{
		CurrentScene = ESceneName::Main;
	}
	else if (LevelName == Mission1SceneName)
	{
		CurrentScene = ESceneName::Mission1;
	}
	else if (LevelName == Mission2SceneName)
	{
		CurrentScene = ESceneName::Mission2;
	}
	else if (LevelName == Mission3SceneName)
	{
		CurrentScene = ESceneName::Mission3;
	}
	else if (LevelName == InfinitySceneName)
	{
		CurrentScene = ESceneName::Infinity;
	}
	else if (LevelName == EndingSceneName)
	{
		CurrentScene = ESceneName::Ending;
	}
}
