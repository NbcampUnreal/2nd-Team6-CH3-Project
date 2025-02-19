// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "System/EnumSet.h"
#include "UIHandle.generated.h"

class UEdmundGameInstance;
class UUIHandleSettings;
class UBaseWidget;
class IGameStateObserver;

UCLASS()
class EDMUNDPRJ_API UUIHandle : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	//virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	void InitUIHandle(UEdmundGameInstance* NewGameInstance);

	//BaseWidget Controll. Add or Remove to Viewport
	void AddToViewportBySceneType(const ESceneType SceneType);
	void AddToViewportByCoverType(const EWidgetType WidgetType);
	void RequestRemoveCoverFromViewport(const EWidgetType WidgetType);
	void RemoveCoverFromViewport(const EWidgetType WidgetType);

	//Fade In / Out : Play Animation from FadeWidget
	void FadeIn();
	void FadeOut(const bool bIsNext, const ESceneType SceneType);
	void RequestMoveSceneByFade(const bool bIsNext, const ESceneType SceneType);

	// Apply Sound Volume To Option Widget
	float GetBGMVolumeByGameInstance() const;
	float GetEffectVolumeByGameInstance() const;

	// UI Button Click Event
	void ClickedCloseCoverWidget() const;
	void ClickedMoveToTitle() const;
	void ClickedMoveToMain() const;
	void ClickedMoveToNext() const;
	void ClickedMoveToMission(const ESceneType SceneType) const;
	void ClickedQuitGame() const;
	void ClickedBGMVolume(const float Volume) const;
	void ClickedEffectVolume(const float Volume) const;

private:
	//Initialize Widgets : Load WidgetClass from UIHandleSettings(DeveloperSettings), and Create UserWidget from WidgetClass
	void LoadBaseWidgets(const UUIHandleSettings* UISettings);
	void LoadCoverWidgets(const UUIHandleSettings* UISettings);
	void CreateBaseWidgets();
	void CreateCoverWidgets();

	// Cursor Visible and Input Mode Change. Request to GameInstance
	void RequestChangeCursorMode(const bool bIsVisible, const FInputModeDataBase& InputMode);

	// Add or Remove
	void AddWidgetToViewport(UBaseWidget* Widget);
	void RemoveWidgetFromViewport(UBaseWidget* Widget);

	// OptionWidget Close or Open
	void OpenOption();
	void CloseOption();

	// TextWidget Close or Open
	void OpenText();
	void CloseText();

	// ShopWidget Close or Open
	void OpenShop();
	void CloseShop();

	// ResultWidget Close or Open
	void OpenResult();
	void CloseResult();

	// CharacterListWidget Close or Open
	void OpenCharacterList();
	void CloseCharacterList();

	// SkillListWidget Close or Open
	void OpenSkillList();
	void CloseSkillList();

	// MissionListWidget Close or Open
	void OpenMissionList();
	void CloseMissionList();

private:
	TSubclassOf<UBaseWidget> TitleWidgetClass = nullptr;
	TSubclassOf<UBaseWidget> MainWidgetClass = nullptr;
	TSubclassOf<UBaseWidget> InGameWidgetClass = nullptr;
	TSubclassOf<UBaseWidget> EndingWidgetClass = nullptr;
	TSubclassOf<UBaseWidget> FadeWidgetClass = nullptr;
	TSubclassOf<UBaseWidget> OptionWidgetClass = nullptr;
	TSubclassOf<UBaseWidget> ShopWidgetClass = nullptr;
	TSubclassOf<UBaseWidget> TextWidgetClass = nullptr;
	TSubclassOf<UBaseWidget> ResultWidgetClass = nullptr;
	TSubclassOf<UBaseWidget> CharacterListWidgetClass = nullptr;
	TSubclassOf<UBaseWidget> SkillListWidgetClass = nullptr;
	TSubclassOf<UBaseWidget> MissionListWidgetClass = nullptr;

	UPROPERTY()
	TObjectPtr<UBaseWidget> TitleWidget = nullptr;

	UPROPERTY()
	TObjectPtr<UBaseWidget> MainWidget = nullptr;

	UPROPERTY()
	TObjectPtr<UBaseWidget> InGameWidget = nullptr;

	UPROPERTY()
	TObjectPtr<UBaseWidget> EndingWidget = nullptr;

	UPROPERTY()
	TObjectPtr<UBaseWidget> FadeWidget = nullptr;

	UPROPERTY()
	TObjectPtr<UBaseWidget> OptionWidget = nullptr;

	UPROPERTY()
	TObjectPtr<UBaseWidget> ShopWidget = nullptr;

	UPROPERTY()
	TObjectPtr<UBaseWidget> TextWidget = nullptr;

	UPROPERTY()
	TObjectPtr<UBaseWidget> ResultWidget = nullptr;

	UPROPERTY()
	TObjectPtr<UBaseWidget> CharacterListWidget = nullptr;

	UPROPERTY()
	TObjectPtr<UBaseWidget> SkillListWidget = nullptr;

	UPROPERTY()
	TObjectPtr<UBaseWidget> MissionListWidget = nullptr;


	UPROPERTY()
	TObjectPtr<UBaseWidget> CurrentBaseWidget = nullptr;

	UPROPERTY()
	TObjectPtr<UBaseWidget> CurrentCoverWidget = nullptr;

	TObjectPtr<UEdmundGameInstance> EdmundGameInstance = nullptr;
	TArray<TScriptInterface<IGameStateObserver>> UIObservers;
	FTimerHandle TimerHandle;
	int32 ViewCount = 0;
	bool bBaseCursorMode = false;
};
