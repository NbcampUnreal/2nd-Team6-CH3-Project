// Fill out your copyright notice in the Description page of Project Settings.


#include "System/UIHandle.h"
#include "UI/BaseWidget.h"
#include "System/EdmundGameInstance.h"
#include "System/Settings/UIHandleSettings.h"

void UUIHandle::InitUIHandle(UEdmundGameInstance* NewGameInstance)
{
	EdmundGameInstance = NewGameInstance;

	const UUIHandleSettings* UISettings = GetDefault<UUIHandleSettings>();
	checkf(IsValid(UISettings), TEXT("UI Settings is invalid"));
	CreateBaseWidgets(UISettings);
	CreateCoverWidgets(UISettings);
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

	switch (SceneType)
	{
	case ESceneType::Title:
		CurrentBaseWidget = TitleWidget;
		RequestChangeCursorMode(true, FInputModeUIOnly());
		bBaseCursorMode = true;
		break;

	case ESceneType::Main:
		CurrentBaseWidget = MainWidget;
		RequestChangeCursorMode(true, FInputModeUIOnly());
		bBaseCursorMode = true;
		break;

	case ESceneType::Mission1:
		CurrentBaseWidget = InGameWidget;
		bBaseCursorMode = false;
		RequestChangeCursorMode(false, FInputModeGameOnly());
		break;

	case ESceneType::Mission2:
		CurrentBaseWidget = InGameWidget;
		bBaseCursorMode = false;
		RequestChangeCursorMode(false, FInputModeGameOnly());
		break;

	case ESceneType::Mission3:
		CurrentBaseWidget = InGameWidget;
		bBaseCursorMode = false;
		RequestChangeCursorMode(false, FInputModeGameOnly());
		break;

	case ESceneType::Infinity:
		CurrentBaseWidget = InGameWidget;
		bBaseCursorMode = false;
		RequestChangeCursorMode(false, FInputModeGameOnly());
		break;

	case ESceneType::Ending:
		CurrentBaseWidget = EndingWidget;
		RequestChangeCursorMode(true, FInputModeUIOnly());
		bBaseCursorMode = true;
		break;

	default:
		checkNoEntry();
		break;
	}

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
		CurrentCoverWidget = OptionWidget;
		OpenOption();
		break;

	case EWidgetType::ShopWidget:
		CurrentCoverWidget = ShopWidget;
		OpenShop();
		break;

	case EWidgetType::ResultWidget:
		CurrentCoverWidget = ResultWidget;
		OpenResult();
		break;

	case EWidgetType::TextWidget:
		CurrentCoverWidget = TextWidget;
		OpenText();
		break;

	case EWidgetType::CharacterListWidget:
		CurrentCoverWidget = CharacterListWidget;
		OpenCharacterList();
		break;

	case EWidgetType::SkillListWidget:
		CurrentCoverWidget = SkillListWidget;
		OpenSkillList();
		break;

	case EWidgetType::MissionListWidget:
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

	case EWidgetType::ResultWidget:
		CloseResult();
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

	if (IsValid(CurrentCoverWidget))
	{
		RemoveWidgetFromViewport(CurrentCoverWidget);
		CurrentCoverWidget = nullptr;
	}
}

void UUIHandle::FadeIn()
{
	checkf(IsValid(FadeWidget), TEXT("FadeWidget is invalid"));

	FadeWidget->AddToViewport(20);
	FadeWidget->PlayAddAnim();
}

void UUIHandle::FadeOut(const bool bIsNext, const ESceneType SceneType)
{
	checkf(IsValid(FadeWidget), TEXT("FadeWidget is invalid"));

	FadeWidget->AddToViewport(20);
	FadeWidget->PlayRemoveAnim(bIsNext, SceneType);
	RequestChangeCursorMode(true, FInputModeUIOnly());
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
	RequestChangeCursorMode(true, FInputModeUIOnly());
}

void UUIHandle::CloseOption()
{
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
	CurrentBaseWidget->PlayRemoveAnim();
	RequestChangeCursorMode(true, FInputModeUIOnly());
}

void UUIHandle::CloseShop()
{
	CurrentBaseWidget->PlayAddAnim();
	RequestChangeCursorMode(false, FInputModeGameOnly());
}

void UUIHandle::OpenResult()
{
	RequestChangeCursorMode(true, FInputModeUIOnly());
}

void UUIHandle::CloseResult()
{
	RequestChangeCursorMode(false, FInputModeGameOnly());
}

void UUIHandle::OpenCharacterList()
{
	CurrentBaseWidget->PlayRemoveAnim();
	RequestChangeCursorMode(true, FInputModeUIOnly());

	checkf(IsValid(EdmundGameInstance), TEXT("GameInstance is invalid"));
	EdmundGameInstance->ChangeSelectMode(true);
	EdmundGameInstance->ChangeCursorMode(true);
	EdmundGameInstance->ChangeInputMode(FInputModeGameAndUI());
}

void UUIHandle::CloseCharacterList()
{
	CurrentBaseWidget->PlayAddAnim();
	RequestChangeCursorMode(false, FInputModeGameOnly());

	checkf(IsValid(EdmundGameInstance), TEXT("GameInstance is invalid"));
	EdmundGameInstance->ChangeSelectMode(false);
	EdmundGameInstance->ChangeCursorMode(true);
	EdmundGameInstance->ChangeInputMode(FInputModeUIOnly());
}

void UUIHandle::OpenSkillList()
{
	RequestChangeCursorMode(true, FInputModeUIOnly());
}

void UUIHandle::CloseSkillList()
{
	RequestChangeCursorMode(false, FInputModeGameOnly());
}

void UUIHandle::OpenMissionList()
{
	RequestChangeCursorMode(true, FInputModeGameOnly());
}

void UUIHandle::CloseMissionList()
{
	RequestChangeCursorMode(false, FInputModeGameOnly());
}

void UUIHandle::ClickedCloseCoverWidget() const
{
	checkf(IsValid(CurrentCoverWidget), TEXT("Current Cover Widget is invalid"));
	CurrentCoverWidget->PlayRemoveAnim();
}

void UUIHandle::ClickedMoveToTitle() const
{
	checkf(IsValid(EdmundGameInstance), TEXT("GameInstance is invalid"));
	EdmundGameInstance->RequestSceneMove(false, ESceneType::Title);
}

void UUIHandle::ClickedMoveToMain() const
{
	checkf(IsValid(EdmundGameInstance), TEXT("GameInstance is invalid"));
	EdmundGameInstance->RequestSceneMove(false, ESceneType::Main);
}

void UUIHandle::ClickedMoveToNext() const
{
	checkf(IsValid(EdmundGameInstance), TEXT("GameInstance is invalid"));
	EdmundGameInstance->RequestSceneMove(true);
}

void UUIHandle::ClickedMoveToMission(const ESceneType SceneType) const
{
	checkf(IsValid(EdmundGameInstance), TEXT("GameInstance is invalid"));
	EdmundGameInstance->RequestSceneMove(false, SceneType);
}

void UUIHandle::ClickedQuitGame() const
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

void UUIHandle::ClickedSelectCharacter(const ECharacterType CharacterType) const
{
	checkf(IsValid(EdmundGameInstance), TEXT("EdmundGameInstance is invalid"));
	EdmundGameInstance->SetPlayerType(CharacterType);
}

void UUIHandle::ClickedSelectSkill(const int32 Index) const
{
	checkf(IsValid(EdmundGameInstance), TEXT("EdmundGameInstance is invalid"));
	EdmundGameInstance->ApplySelectSkill(Index);
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

void UUIHandle::ClickedRetry() const
{
	checkf(IsValid(EdmundGameInstance), TEXT("EdmundGameInstance is invalid"));
	ESceneType CurrentScene = EdmundGameInstance->GetCurrentSceneName();
	EdmundGameInstance->RequestSceneMove(false, CurrentScene);
}

void UUIHandle::RequestChangeCursorMode(const bool bIsVisible, const FInputModeDataBase& InputMode)
{
	checkf(IsValid(EdmundGameInstance), TEXT("EdmundGameInstance is invalid"));

	if (!bBaseCursorMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("Change CursorMode"));
		EdmundGameInstance->ChangeCursorMode(bIsVisible);
		EdmundGameInstance->ChangeInputMode(InputMode);
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

void UUIHandle::CreateBaseWidgets(const UUIHandleSettings* UISettings)
{
	checkf(IsValid(EdmundGameInstance), TEXT("EdmundGameInstance is invalid"));

	checkf(IsValid(UISettings->TitleWidgetClass), TEXT("TitleWidgetClass is invalid"));
	TitleWidget = CreateWidget<UBaseWidget>(EdmundGameInstance, UISettings->TitleWidgetClass);
	TitleWidget->InitWidget(this);

	checkf(IsValid(UISettings->MainWidgetClass), TEXT("MainWidgetClass is invalid"));
	MainWidget = CreateWidget<UBaseWidget>(EdmundGameInstance, UISettings->MainWidgetClass);
	MainWidget->InitWidget(this);

	checkf(IsValid(UISettings->InGameWidgetClass), TEXT("InGmaeWidgetClass is invalid"));
	InGameWidget = CreateWidget<UBaseWidget>(EdmundGameInstance, UISettings->InGameWidgetClass);
	InGameWidget->InitWidget(this);
	UIObservers.Add(InGameWidget);

	checkf(IsValid(UISettings->EndingWidgetClass), TEXT("EndingWidgetClass is invalid"));
	EndingWidget = CreateWidget<UBaseWidget>(EdmundGameInstance, UISettings->EndingWidgetClass);
	EndingWidget->InitWidget(this);
	UIObservers.Add(EndingWidget);
}

void UUIHandle::CreateCoverWidgets(const UUIHandleSettings* UISettings)
{
	checkf(IsValid(EdmundGameInstance), TEXT("EdmundGameInstance is invalid"));

	checkf(IsValid(UISettings->FadeWidgetClass), TEXT("FadeWidgetClass is invalid"));
	FadeWidget = CreateWidget<UBaseWidget>(EdmundGameInstance, UISettings->FadeWidgetClass);
	FadeWidget->InitWidget(this);

	checkf(IsValid(UISettings->OptionWidgetClass), TEXT("OptionWidgetClass is invalid"));
	OptionWidget = CreateWidget<UBaseWidget>(EdmundGameInstance, UISettings->OptionWidgetClass);
	OptionWidget->InitWidget(this);

	checkf(IsValid(UISettings->ResultWidgetClass), TEXT("ResultWidgetClass is invalid"));
	ResultWidget = CreateWidget<UBaseWidget>(EdmundGameInstance, UISettings->ResultWidgetClass);
	ResultWidget->InitWidget(this);
	UIObservers.Add(ResultWidget);

	checkf(IsValid(UISettings->ShopWidgetClass), TEXT("ShopWidgetClass is invalid"));
	ShopWidget = CreateWidget<UBaseWidget>(EdmundGameInstance, UISettings->ShopWidgetClass);
	ShopWidget->InitWidget(this);

	checkf(IsValid(UISettings->TextWidgetClass), TEXT("TextWidgetClass is invalid"));
	TextWidget = CreateWidget<UBaseWidget>(EdmundGameInstance, UISettings->TextWidgetClass);
	TextWidget->InitWidget(this);

	checkf(IsValid(UISettings->CharacterListWidgetClass), TEXT("CharacterListWidgetClass is invalid"));
	CharacterListWidget = CreateWidget<UBaseWidget>(EdmundGameInstance, UISettings->CharacterListWidgetClass);
	CharacterListWidget->InitWidget(this);

	checkf(IsValid(UISettings->SkillListWidgetClass), TEXT("SkillListWidgetClass is invalid"));
	SkillListWidget = CreateWidget<UBaseWidget>(EdmundGameInstance, UISettings->SkillListWidgetClass);
	SkillListWidget->InitWidget(this);
	UIObservers.Add(SkillListWidget);

	checkf(IsValid(UISettings->MissionListWidgetClass), TEXT("MissionListWidgetClass is invalid"));
	MissionListWidget = CreateWidget<UBaseWidget>(EdmundGameInstance, UISettings->MissionListWidgetClass);
	MissionListWidget->InitWidget(this);
}
