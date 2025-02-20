// Fill out your copyright notice in the Description page of Project Settings.


#include "System/SceneHandle.h"
#include "Kismet/GameplayStatics.h"

//void USceneHandle::Initialize(FSubsystemCollectionBase& Collection)
//{
//	Super::Initialize(Collection);
//}

void USceneHandle::InitSceneHandle()
{
	CurrentScene = ESceneType::Title;
}

void USceneHandle::MoveNextScene()
{
	switch (CurrentScene)
	{
	case ESceneType::Title:
		OpenScene(ESceneType::Main);
		break;

	case ESceneType::Mission1:
		OpenScene(ESceneType::Mission2);
		break;

	case ESceneType::Mission2:
		OpenScene(ESceneType::Mission3);
		break;

	case ESceneType::Mission3:
		OpenScene(ESceneType::Ending);
		break;

	case ESceneType::Infinity:
		OpenScene(ESceneType::Main);
		break;

	case ESceneType::Ending:
		OpenScene(ESceneType::Title);
		break;

	default:
		checkNoEntry();
		break;
	}
}

void USceneHandle::OpenScene(ESceneType SceneType)
{
	CurrentScene = SceneType;

	switch (CurrentScene)
	{
	case ESceneType::Title:
		UGameplayStatics::OpenLevel(GetWorld(), TitleSceneName);
		break;

	case ESceneType::Main:
		UGameplayStatics::OpenLevel(GetWorld(), MainSceneName);
		break;

	case ESceneType::Mission1:
		UGameplayStatics::OpenLevel(GetWorld(), Mission1SceneName);
		break;

	case ESceneType::Mission2:
		UGameplayStatics::OpenLevel(GetWorld(), Mission2SceneName);
		break;

	case ESceneType::Mission3:
		UGameplayStatics::OpenLevel(GetWorld(), Mission3SceneName);
		break;

	case ESceneType::Infinity:
		UGameplayStatics::OpenLevel(GetWorld(), InfinitySceneName);
		break;

	case ESceneType::Ending:
		UGameplayStatics::OpenLevel(GetWorld(), EndingSceneName);
		break;

	default:
		checkNoEntry();
		break;
	}
}

ESceneType USceneHandle::GetCurrentScene() const
{
	return CurrentScene;
}

void USceneHandle::CheckCurrentScene()
{
	FName LevelName(GetWorld()->GetMapName());

	if (LevelName == TitleSceneName)
	{
		CurrentScene = ESceneType::Title;
	}
	else if (LevelName == MainSceneName)
	{
		CurrentScene = ESceneType::Main;
	}
	else if (LevelName == Mission1SceneName)
	{
		CurrentScene = ESceneType::Mission1;
	}
	else if (LevelName == Mission2SceneName)
	{
		CurrentScene = ESceneType::Mission2;
	}
	else if (LevelName == Mission3SceneName)
	{
		CurrentScene = ESceneType::Mission3;
	}
	else if (LevelName == InfinitySceneName)
	{
		CurrentScene = ESceneType::Infinity;
	}
	else if (LevelName == EndingSceneName)
	{
		CurrentScene = ESceneType::Ending;
	}
}
