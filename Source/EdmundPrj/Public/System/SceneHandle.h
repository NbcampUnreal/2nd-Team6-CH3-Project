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

	static const FName TitleSceneName;
	static const FName MainSceneName;
	static const FName Mission1SceneName;
	static const FName Mission2SceneName;
	static const FName Mission3SceneName;
	static const FName InfinitySceneName;
	static const FName EndingSceneName;

	const float FadeTime = 0.5f;
	FTimerHandle TimerHandle;

	UPROPERTY()
	TObjectPtr<UEdmundGameInstance> EdmundGameInstance = nullptr;
};
