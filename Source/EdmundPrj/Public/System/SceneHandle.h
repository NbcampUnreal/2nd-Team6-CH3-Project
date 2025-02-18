// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "System/EnumSet.h"
#include "SceneHandle.generated.h"


UCLASS()
class EDMUNDPRJ_API USceneHandle : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	//Initialize
	//virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	void InitSceneHandle();

	//OpenLevel
	void MoveNextScene();
	void OpenScene(ESceneType SceneName);

	//Controll CurrentScene
	ESceneType GetCurrentScene() const;
	void CheckCurrentScene();

protected:
	

protected:
	const FName TitleSceneName = "TitleLevel";
	const FName MainSceneName = "MainLevel";
	const FName Mission1SceneName = "Mission1Level";
	const FName Mission2SceneName = "Mission2Level";
	const FName Mission3SceneName = "Mission3Level";
	const FName InfinitySceneName = "InfinityLevel";
	const FName EndingSceneName = "EndingLevel";

	const float FadeTime = 0.5f;
	ESceneType CurrentScene = ESceneType::Title;
	FTimerHandle TimerHandle;
};
