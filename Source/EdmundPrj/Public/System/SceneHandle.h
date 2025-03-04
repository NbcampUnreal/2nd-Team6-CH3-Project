// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "System/EnumSet.h"
#include "SceneHandle.generated.h"

class UEdmundGameInstance;

UCLASS()
class EDMUNDPRJ_API USceneHandle : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	void InitSceneHandle(UEdmundGameInstance* NewGameInstance);

	//OpenLevel
	void MoveNextScene();
	void OpenScene(ESceneType SceneName);

	//Controll CurrentScene
	ESceneType GetCurrentScene() const;
	void CheckCurrentScene();

protected:
	

protected:
	UPROPERTY()
	ESceneType CurrentScene = ESceneType::Title;

	const FName TitleSceneName = "TitleLevel";
	const FName MainSceneName = "MainLevel";
	const FName Mission1SceneName = "Mission1Level";
	const FName Mission2SceneName = "Mission2Level";
	const FName Mission3SceneName = "Mission3Level";
	const FName InfinitySceneName = "InfinityLevel";
	const FName EndingSceneName = "EndingLevel";

	const float FadeTime = 0.5f;
	FTimerHandle TimerHandle;

	TObjectPtr<UEdmundGameInstance> EdmundGameInstance = nullptr;
};
