// Fill out your copyright notice in the Description page of Project Settings.


#include "System/UIHandle.h"
#include "UI/BaseWidget.h"
#include "System/EdmundGameInstance.h"
#include "System/Settings/UIHandleSettings.h"
#include "GameFramework/GameUserSettings.h"


void UUIHandle::InitUIHandle(UEdmundGameInstance* NewGameInstance)
{
	UGameUserSettings* GameUserSettings = GEngine->GetGameUserSettings();
	GameUserSettings->SetFullscreenMode(EWindowMode::Windowed);
	GameUserSettings->SetScreenResolution(FIntPoint(1920, 1080));//(1280, 720));
	GameUserSettings->ApplySettings(false);

	EdmundGameInstance = NewGameInstance;
	UISettings = GetDefault<UUIHandleSettings>();
	checkf(IsValid(UISettings), TEXT("UI Settings is invalid"));
}

void UUIHandle::AddToViewportBySceneType(ESceneType SceneType)
{
	

	if (IsValid(CurrentBaseWidget))
	{
		RemoveWidgetFromViewport(CurrentBaseWidget);
		CurrentBaseWidget = nullptr;
		CurrentCoverWidget = nullptr;
		ViewCount = 0;
	}

	bBaseCursorMode = false;

	switch (SceneType)
	{
	case ESceneType::Title:
		CheckValidOfTitleWidget();
		CurrentBaseWidget = TitleWidget;
		RequestChangeCursorMode(true, FInputModeUIOnly());
		bBaseCursorMode = true;
		break;

	case ESceneType::Main:
		CheckValidOfMainWidget();
		CurrentBaseWidget = MainWidget;
		RequestChangeCursorMode(true, FInputModeUIOnly());
		bBaseCursorMode = true;
		break;

	case ESceneType::Mission1:
		CheckValidOfInGameWidget();
		CurrentBaseWidget = InGameWidget;
		bBaseCursorMode = false;
		RequestChangeCursorMode(false, FInputModeGameOnly());

		break;

	case ESceneType::Mission2:
		CheckValidOfInGameWidget();
		CurrentBaseWidget = InGameWidget;
		bBaseCursorMode = false;
		RequestChangeCursorMode(false, FInputModeGameOnly());
		break;

	case ESceneType::Mission3:
		CheckValidOfInGameWidget();
		CurrentBaseWidget = InGameWidget;
		bBaseCursorMode = false;
		RequestChangeCursorMode(false, FInputModeGameOnly());
		break;

	case ESceneType::Infinity:
		CheckValidOfInGameWidget();
		CurrentBaseWidget = InGameWidget;
		bBaseCursorMode = false;
		RequestChangeCursorMode(false, FInputModeGameOnly());
		break;

	case ESceneType::Ending:
		CheckValidOfEndingWidget();
		CurrentBaseWidget = EndingWidget;
		RequestChangeCursorMode(true, FInputModeUIOnly());
		bBaseCursorMode = true;
		CurrentBaseWidget->PlayAddAnim();
		break;

	default:
		checkNoEntry();
		break;
	}

	CurrentBaseWidget->Update();
	AddWidgetToViewport(CurrentBaseWidget);
}

void UUIHandle::AddToViewportByCoverType(const EWidgetType WidgetType)
{
	if (IsValid(CurrentCoverWidget))
	{
		CurrentCoverWidget->PlayRemoveAnim();
		return;
	}

	switch (WidgetType)
	{
	case EWidgetType::OptionWidget:
		CheckValidOfOptionWidget();
		CurrentCoverWidget = OptionWidget;
		OpenOption();
		break;

	case EWidgetType::ShopWidget:
		CheckValidOfShopWidget();
		CurrentCoverWidget = ShopWidget;
		OpenShop();
		break;

	case EWidgetType::TextWidget:
		CheckValidOfTextWidget();
		CurrentCoverWidget = TextWidget;
		OpenText();
		break;

	case EWidgetType::CharacterListWidget:
		CheckValidOfCharacterListWidget();
		CurrentCoverWidget = CharacterListWidget;
		OpenCharacterList();
		break;

	case EWidgetType::SkillListWidget:
		CheckValidOfSkillListWidget();
		CurrentCoverWidget = SkillListWidget;
		OpenSkillList();
		break;

	case EWidgetType::MissionListWidget:
		CheckValidOfMissionListWidget();
		CurrentCoverWidget = MissionListWidget;
		OpenMissionList();
		break;

	default:
		checkNoEntry();
		break;
	}

	AddWidgetToViewport(CurrentCoverWidget);
	CurrentCoverWidget->Action();
	CurrentCoverWidget->PlayAddAnim();
}

void UUIHandle::RequestRemoveCoverFromViewport(const EWidgetType WidgetType)
{
	if (!IsValid(CurrentCoverWidget))
	{
		return;
	}

	CurrentCoverWidget->PlayRemoveAnim();

	switch (WidgetType)
	{
	case EWidgetType::OptionWidget:
		CloseOption();
		break;

	case EWidgetType::TextWidget:
		CloseText();
		break;

	case EWidgetType::CharacterListWidget:
		CloseCharacterList();
		break;

	case EWidgetType::SkillListWidget:
		CloseSkillList();
		break;

	case EWidgetType::ShopWidget:
		CloseShop();
		break;

	case EWidgetType::MissionListWidget:
		CloseMissionList();
		break;
	}
}

void UUIHandle::RemoveCoverFromViewport(const EWidgetType WidgetType)
{
	if (WidgetType == EWidgetType::FadeWidget)
	{
		RemoveWidgetFromViewport(FadeWidget);
		return;
	}

	if (WidgetType == EWidgetType::ResultWidget)
	{
		RemoveWidgetFromViewport(ResultWidget);
		return;
	}

	if (IsValid(CurrentCoverWidget))
	{
		RemoveWidgetFromViewport(CurrentCoverWidget);
		CurrentCoverWidget = nullptr;
	}
}

void UUIHandle::FadeIn()
{
	CheckValidOfFadeWidget();
	checkf(IsValid(FadeWidget), TEXT("FadeWidget is invalid"));

	FadeWidget->AddToViewport(20);
	FadeWidget->PlayAddAnim();
}

void UUIHandle::FadeOut(const bool bIsNext, const ESceneType SceneType)
{
	CheckValidOfFadeWidget();
	checkf(IsValid(FadeWidget), TEXT("FadeWidget is invalid"));

	FadeWidget->AddToViewport(20);
	FadeWidget->PlayRemoveAnim(bIsNext, SceneType);
	RequestChangeCursorMode(true, FInputModeUIOnly());
}

void UUIHandle::OpenResult()
{
	checkf(IsValid(EdmundGameInstance), TEXT("GameInstance is invalid"));
	EdmundGameInstance->RequestPause();

	CheckValidOfResultWidget();
	checkf(IsValid(ResultWidget), TEXT("ResultWidget is invalid"));
	ResultWidget->AddToViewport(10);
	ResultWidget->PlayAddAnim();
	RequestChangeCursorMode(true, FInputModeUIOnly());
}

void UUIHandle::CloseResult()
{
	checkf(IsValid(ResultWidget), TEXT("ResultWidget is invalid"));
	ResultWidget->PlayRemoveAnim();
	RequestChangeCursorMode(false, FInputModeGameOnly());
}

void UUIHandle::RequestMoveSceneByFade(const bool bIsNext, const ESceneType SceneType)
{
	if (bIsNext)
	{
		EdmundGameInstance->MoveNextScene();
	}
	else
	{
		EdmundGameInstance->MoveScene(SceneType);
	}
}

float UUIHandle::GetBGMVolumeByGameInstance() const
{
	checkf(IsValid(EdmundGameInstance), TEXT("EdmundGameInstance is invalid"));
	return EdmundGameInstance->GetBGMVolume();
}

float UUIHandle::GetEffectVolumeByGameInstance() const
{
	checkf(IsValid(EdmundGameInstance), TEXT("EdmundGameInstance is invalid"));
	return EdmundGameInstance->GetEffectVolume();
}

const TArray<FShopCatalogRow*>& UUIHandle::GetCurrentAdvance() const
{
	checkf(IsValid(EdmundGameInstance), TEXT("EdmundGameInstance is invalid"));
	return EdmundGameInstance->GetAdvanceState();
}

const TArray<FPlayerSkillRow*>& UUIHandle::GetCurrentRandomSkill() const
{
	checkf(IsValid(EdmundGameInstance), TEXT("EdmundGameInstance is invalid"));
	return EdmundGameInstance->GetRandomSkillSet();
}

const TArray<FCharacterDataRow*>& UUIHandle::GetCharacterData() const
{
	checkf(IsValid(EdmundGameInstance), TEXT("EdmundGameInstance is invalid"));
	return EdmundGameInstance->GetCharacterData();
}

const int32 UUIHandle::GetCurrentMoney() const
{
	checkf(IsValid(EdmundGameInstance), TEXT("EdmundGameInstance is invalid"));
	return EdmundGameInstance->GetPossessMoney();
}

bool UUIHandle::CheckClearedMission(int32 Index) const
{
	checkf(IsValid(EdmundGameInstance), TEXT("EdmundGameInstance is invalid"));
	return EdmundGameInstance->CheckClearedMission(Index);
}

void UUIHandle::OpenOption()
{
	RequestPlayUISound(EUISoundType::Open);
	EdmundGameInstance->RequestPause();
	RequestChangeCursorMode(true, FInputModeUIOnly());
}

void UUIHandle::CloseOption()
{
	RequestPlayUISound(EUISoundType::Open);
	checkf(IsValid(EdmundGameInstance), TEXT("GameInstance is invalid"));
	EdmundGameInstance->RequestUnpause();
	RequestChangeCursorMode(false, FInputModeGameOnly());
}

void UUIHandle::OpenText()
{
	RequestChangeCursorMode(true, FInputModeUIOnly());
}

void UUIHandle::CloseText()
{
	RequestChangeCursorMode(false, FInputModeGameOnly());
}

void UUIHandle::OpenShop()
{
	RequestPlayUISound(EUISoundType::Open);
	CurrentBaseWidget->PlayRemoveAnim();
	RequestChangeCursorMode(true, FInputModeUIOnly());
}

void UUIHandle::CloseShop()
{
	CurrentBaseWidget->PlayAddAnim();
	RequestChangeCursorMode(false, FInputModeGameOnly());
	RequestPlayUISound(EUISoundType::Open);
}

void UUIHandle::OpenCharacterList()
{
	CurrentBaseWidget->PlayRemoveAnim();
	RequestChangeCursorMode(true, FInputModeUIOnly());

	checkf(IsValid(EdmundGameInstance), TEXT("GameInstance is invalid"));
	EdmundGameInstance->CheckClosedPlayerType();
	EdmundGameInstance->ChangeCursorMode(true);
	EdmundGameInstance->ChangeInputMode(FInputModeGameAndUI());
	RequestPlayUISound(EUISoundType::Open);
}

void UUIHandle::CloseCharacterList()
{
	CurrentBaseWidget->PlayAddAnim();
	RequestChangeCursorMode(false, FInputModeGameOnly());

	checkf(IsValid(EdmundGameInstance), TEXT("GameInstance is invalid"));
	EdmundGameInstance->CheckClosedPlayerType();
	EdmundGameInstance->ChangeCursorMode(true);
	EdmundGameInstance->ChangeInputMode(FInputModeUIOnly());
	RequestPlayUISound(EUISoundType::Open);
}

void UUIHandle::OpenSkillList()
{
	EdmundGameInstance->RequestPause();
	RequestChangeCursorMode(true, FInputModeUIOnly());
}

void UUIHandle::CloseSkillList()
{
	EdmundGameInstance->RequestUnpause();
	RequestChangeCursorMode(false, FInputModeGameOnly());
}

void UUIHandle::OpenMissionList()
{
	RequestChangeCursorMode(true, FInputModeGameOnly());
	RequestPlayUISound(EUISoundType::Open);

}

void UUIHandle::CloseMissionList()
{
	RequestChangeCursorMode(false, FInputModeGameOnly());
}

void UUIHandle::RequestPlayUISound(EUISoundType Type)
{
	checkf(IsValid(EdmundGameInstance), TEXT("GameInstance is invalid"));
	EdmundGameInstance->PlayUISound(Type);
}

void UUIHandle::ClickedCloseCoverWidget() const
{
	checkf(IsValid(CurrentCoverWidget), TEXT("Current Cover Widget is invalid"));
	CurrentCoverWidget->PlayRemoveAnim();
}

void UUIHandle::ClickedMoveToTitle()
{
	FadeOut(false, ESceneType::Title);
}

void UUIHandle::ClickedMoveToMain()
{
	FadeOut(false, ESceneType::Main);
}

void UUIHandle::ClickedMoveToNext()
{
	FadeOut(true);
}

void UUIHandle::ClickedMoveToMission(const ESceneType SceneType)
{
	FadeOut(false, SceneType);
}

void UUIHandle::ClickedRetry()
{
	checkf(IsValid(EdmundGameInstance), TEXT("EdmundGameInstance is invalid"));
	ESceneType CurrentScene = EdmundGameInstance->GetCurrentSceneName();
	ClickedMoveToMission(CurrentScene);
}

void UUIHandle::ClickedQuitGame()
{
	checkf(IsValid(EdmundGameInstance), TEXT("EdmundGameInstance is invalid"));
	EdmundGameInstance->QuitGame();
}

void UUIHandle::ClickedBGMVolume(const float Volume) const
{
	checkf(IsValid(EdmundGameInstance), TEXT("EdmundGameInstance is invalid"));
	EdmundGameInstance->SetBGMVolume(Volume);
}

void UUIHandle::ClickedEffectVolume(const float Volume) const
{
	checkf(IsValid(EdmundGameInstance), TEXT("EdmundGameInstance is invalid"));
	EdmundGameInstance->SetEffectVolume(Volume);
}

void UUIHandle::ClickedSelectCharacter(const ECharacterType CharacterType)
{
	checkf(IsValid(EdmundGameInstance), TEXT("EdmundGameInstance is invalid"));
	EdmundGameInstance->SetPlayerType(CharacterType);
}

void UUIHandle::ClickedCancleSelectCharacter()
{
	checkf(IsValid(EdmundGameInstance), TEXT("EdmundGameInstance is invalid"));
	EdmundGameInstance->CancleSelectedType();
}

void UUIHandle::ClickedSelectSkill(const int32 Index)
{
	checkf(IsValid(EdmundGameInstance), TEXT("EdmundGameInstance is invalid"));
	EdmundGameInstance->ApplySelectSkill(Index);
}

void UUIHandle::ClickedSkipStory()
{
	checkf(IsValid(EdmundGameInstance), TEXT("EdmundGameInstance is invalid"));
	EdmundGameInstance->SkipMissionStory();
}

void UUIHandle::ClickedNextStory()
{
	checkf(IsValid(EdmundGameInstance), TEXT("EdmundGameInstance is invalid"));
	EdmundGameInstance->MoveNextMissionStory();
}

const FShopCatalogRow* UUIHandle::ClickedBuyAgree(const FName& TargetRow, const int32 UpdateValue) const
{
	checkf(IsValid(EdmundGameInstance), TEXT("EdmundGameInstance is invalid"));
	bool bIsUpdated =  EdmundGameInstance->UpdateAdvanceState(TargetRow, UpdateValue);

	if (bIsUpdated)
	{
		return EdmundGameInstance->GetAdvanceState(TargetRow);
	}

	return nullptr;
}

const TArray<TScriptInterface<IGameStateObserver>>& UUIHandle::GetUIObservers() const
{
	return UIObservers;
}

void UUIHandle::RequestChangeCursorMode(const bool bIsVisible, const FInputModeDataBase& InputMode)
{
	checkf(IsValid(EdmundGameInstance), TEXT("EdmundGameInstance is invalid"));
	
	if (!bBaseCursorMode)
	{
		EdmundGameInstance->ChangeInputMode(InputMode);
		EdmundGameInstance->ChangeCursorMode(bIsVisible);
	}
}

void UUIHandle::AddWidgetToViewport(UBaseWidget* Widget)
{
	checkf(IsValid(Widget), TEXT("Target Widget is invalid"));

	Widget->AddToViewport(ViewCount);
	++ViewCount;
}

void UUIHandle::RemoveWidgetFromViewport(UBaseWidget* Widget)
{
	if (IsValid(Widget))
	{
		Widget->RemoveFromViewport();
		--ViewCount;
	}
}

UBaseWidget* UUIHandle::SpawnWidget(UClass* WidgetClass)
{
	UBaseWidget* TempWidget = nullptr;

	if (IsValid(WidgetClass))
	{
		TempWidget = CreateWidget<UBaseWidget>(EdmundGameInstance, WidgetClass);
		TempWidget->InitWidget(this);
	}

	return TempWidget;
}

void UUIHandle::CheckValidOfTitleWidget()
{
	if (IsValid(TitleWidget))
	{
		return;
	}

	if (!IsValid(UISettings))
	{
		return;
	}

	TitleWidget = SpawnWidget(UISettings->TitleWidgetClass);
}

void UUIHandle::CheckValidOfMainWidget()
{
	if (IsValid(MainWidget))
	{
		return;
	}

	if (!IsValid(UISettings))
	{
		return;
	}

	MainWidget = SpawnWidget(UISettings->MainWidgetClass);
}

void UUIHandle::CheckValidOfInGameWidget()
{
	if (IsValid(InGameWidget))
	{
		return;
	}

	if (!IsValid(UISettings))
	{
		return;
	}

	InGameWidget = SpawnWidget(UISettings->InGameWidgetClass);
	EdmundGameInstance->BindUIObserverToGameState(InGameWidget);
	UIObservers.Add(InGameWidget);
}

void UUIHandle::CheckValidOfEndingWidget()
{
	if (IsValid(EndingWidget))
	{
		return;
	}

	if (!IsValid(UISettings))
	{
		return;
	}

	EndingWidget = SpawnWidget(UISettings->EndingWidgetClass);
	EdmundGameInstance->BindUIObserverToGameState(EndingWidget);
	UIObservers.Add(EndingWidget);
}

void UUIHandle::CheckValidOfFadeWidget()
{
	if (IsValid(FadeWidget))
	{
		return;
	}

	if (!IsValid(UISettings))
	{
		return;
	}

	FadeWidget = SpawnWidget(UISettings->FadeWidgetClass);
}

void UUIHandle::CheckValidOfOptionWidget()
{
	if (IsValid(OptionWidget))
	{
		return;
	}

	if (!IsValid(UISettings))
	{
		return;
	}

	OptionWidget = SpawnWidget(UISettings->OptionWidgetClass);
}

void UUIHandle::CheckValidOfShopWidget()
{
	if (IsValid(ShopWidget))
	{
		return;
	}

	if (!IsValid(UISettings))
	{
		return;
	}

	ShopWidget = SpawnWidget(UISettings->ShopWidgetClass);
}

void UUIHandle::CheckValidOfTextWidget()
{
	if (IsValid(TextWidget))
	{
		return;
	}

	if (!IsValid(UISettings))
	{
		return;
	}

	TextWidget = SpawnWidget(UISettings->TextWidgetClass);
	EdmundGameInstance->BindUIObserverToGameState(TextWidget);
	UIObservers.Add(TextWidget);
}

void UUIHandle::CheckValidOfResultWidget()
{
	if (IsValid(ResultWidget))
	{
		return;
	}

	if (!IsValid(UISettings))
	{
		return;
	}

	ResultWidget = SpawnWidget(UISettings->ResultWidgetClass);
	EdmundGameInstance->BindUIObserverToGameState(ResultWidget);
	UIObservers.Add(ResultWidget);
}

void UUIHandle::CheckValidOfCharacterListWidget()
{
	if (IsValid(CharacterListWidget))
	{
		return;
	}

	if (!IsValid(UISettings))
	{
		return;
	}

	CharacterListWidget = SpawnWidget(UISettings->CharacterListWidgetClass);
	EdmundGameInstance->BindUIObserverToGameState(CharacterListWidget);
	UIObservers.Add(CharacterListWidget);
}

void UUIHandle::CheckValidOfSkillListWidget()
{
	if (IsValid(SkillListWidget))
	{
		return;
	}

	if (!IsValid(UISettings))
	{
		return;
	}

	SkillListWidget = SpawnWidget(UISettings->SkillListWidgetClass);

	checkf(IsValid(EdmundGameInstance), TEXT("GameInstance is invalid"));
	EdmundGameInstance->BindUIObserverToGameState(SkillListWidget);
	UIObservers.Add(SkillListWidget);
}

void UUIHandle::CheckValidOfMissionListWidget()
{
	if (IsValid(MissionListWidget))
	{
		return;
	}

	if (!IsValid(UISettings))
	{
		return;
	}

	MissionListWidget = SpawnWidget(UISettings->MissionListWidgetClass);
}
