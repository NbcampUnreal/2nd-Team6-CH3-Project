// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "System/EnumSet.h"
#include "UIHandle.generated.h"

class UEdmundGameInstance;
class UUIHandleSettings;
class UBaseWidget;
class UFadeWidget;
class IGameStateObserver;
struct FShopCatalogRow;
struct FPlayerSkillRow;
struct FCharacterDataRow;

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
	void FadeOut(const bool bIsNext = false, const ESceneType SceneType = ESceneType::Title);
	void RequestMoveSceneByFade(const bool bIsNext, const ESceneType SceneType);

	// ResultWidget Close or Open
	void OpenResult();
	void CloseResult();

	// Apply Sound Volume To Option Widget
	float GetBGMVolumeByGameInstance() const;
	float GetEffectVolumeByGameInstance() const;

	// Apply Current Data
	const TArray<FShopCatalogRow*>& GetCurrentAdvance() const;
	const TArray<FPlayerSkillRow*>& GetCurrentRandomSkill() const;
	const TArray<FCharacterDataRow*>& GetCharacterData() const;
	const int32 GetCurrentMoney() const;
	bool CheckClearedMission(int32 Index) const;

	// UI Button Click Event
	void ClickedCloseCoverWidget() const;
	void ClickedMoveToTitle();
	void ClickedMoveToMain();
	void ClickedMoveToNext();
	void ClickedMoveToMission(const ESceneType SceneType);
	void ClickedRetry();
	void ClickedQuitGame() const;
	void ClickedBGMVolume(const float Volume) const;
	void ClickedEffectVolume(const float Volume) const;
	void ClickedSelectCharacter(const ECharacterType CharacterType) const;
	void ClickedCancleSelectCharacter();
	void ClickedSelectSkill(const int32 Index) const;
	void ClickedSkipStory() const;
	void ClickedNextStory() const;
	const FShopCatalogRow* ClickedBuyAgree(const FName& TargetRow, const int32 UpdateValue) const;

	const TArray<TScriptInterface<IGameStateObserver>>& GetUIObservers() const;

private:
	//Initialize Widgets : Load WidgetClass from UIHandleSettings(DeveloperSettings), and Create UserWidget from WidgetClass
	void CreateBaseWidgets(const UUIHandleSettings* UISettings);
	void CreateCoverWidgets(const UUIHandleSettings* UISettings);

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
