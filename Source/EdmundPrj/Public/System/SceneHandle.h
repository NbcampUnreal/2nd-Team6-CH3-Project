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
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	void MoveNextScene();
	void OpenScene(ESceneName SceneName);
	ESceneName GetCurrentScene() const;
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

	ESceneName CurrentScene;
};
