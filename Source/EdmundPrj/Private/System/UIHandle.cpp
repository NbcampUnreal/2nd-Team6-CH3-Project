// Fill out your copyright notice in the Description page of Project Settings.


#include "System/UIHandle.h"
#include "UI/BaseWidget.h"
#include "System/EdmundGameInstance.h"
#include "System/Settings/UIHandleSettings.h"

//void UUIHandle::Initialize(FSubsystemCollectionBase& Collection)
//{
//	Super::Initialize(Collection);
//
//	
//}

void UUIHandle::InitUIHandle(UEdmundGameInstance* NewGameInstance)
{
	EdmundGameInstance = NewGameInstance;

	const UUIHandleSettings* UISettings = GetDefault<UUIHandleSettings>();

	LoadBaseWidgets(UISettings);
	LoadCoverWidgets(UISettings);
	CreateBaseWidgets();
	CreateCoverWidgets();
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

void UUIHandle::OpenOption()
{
	//checkf(IsValid(OptionWidget), TEXT("OptionWidget is not valid"));
	//AddWidgetToViewport(OptionWidget);
	RequestChangeCursorMode(true, FInputModeUIOnly());
}

void UUIHandle::CloseOption()
{
	//checkf(IsValid(OptionWidget), TEXT("OptionWidget is not valid"));
	//RemoveWidgetFromViewport(OptionWidget);
	RequestChangeCursorMode(false, FInputModeGameOnly());
}

void UUIHandle::OpenText()
{
	//checkf(IsValid(TextWidget), TEXT("TextWidget is not valid"));
	//AddWidgetToViewport(TextWidget);
	RequestChangeCursorMode(true, FInputModeUIOnly());
}

void UUIHandle::CloseText()
{
	//checkf(IsValid(TextWidget), TEXT("TextWidget is not valid"));
	//RemoveWidgetFromViewport(TextWidget);
	RequestChangeCursorMode(false, FInputModeGameOnly());
}

void UUIHandle::OpenShop()
{
	//checkf(IsValid(ShopWidget), TEXT("ShopWidget is not valid"));
	//AddWidgetToViewport(ShopWidget);
	RequestChangeCursorMode(true, FInputModeUIOnly());
}

void UUIHandle::CloseShop()
{
	//checkf(IsValid(ShopWidget), TEXT("ShopWidget is not valid"));
	//RemoveWidgetFromViewport(ShopWidget);
	RequestChangeCursorMode(false, FInputModeGameOnly());
}

void UUIHandle::OpenResult()
{
	//checkf(IsValid(ResultWidget), TEXT("ResultWidget is not valid"));
	//AddWidgetToViewport(ResultWidget);
	RequestChangeCursorMode(true, FInputModeUIOnly());
}

void UUIHandle::CloseResult()
{
	//checkf(IsValid(ResultWidget), TEXT("ResultWidget is not valid"));
	//RemoveWidgetFromViewport(ResultWidget);
	RequestChangeCursorMode(false, FInputModeGameOnly());
}

void UUIHandle::OpenCharacterList()
{
	//checkf(IsValid(CharacterListWidget), TEXT("CharacterListWidget is not valid"));
	//AddWidgetToViewport(CharacterListWidget);
	RequestChangeCursorMode(true, FInputModeUIOnly());
}

void UUIHandle::CloseCharacterList()
{
	//checkf(IsValid(CharacterListWidget), TEXT("CharacterListWidget is not valid"));
	//RemoveWidgetFromViewport(CharacterListWidget);
	RequestChangeCursorMode(false, FInputModeGameOnly());
}

void UUIHandle::OpenSkillList()
{
	//checkf(IsValid(SkillListWidget), TEXT("SkillListWidget is not valid"));
	//AddWidgetToViewport(SkillListWidget);
	RequestChangeCursorMode(true, FInputModeUIOnly());
}

void UUIHandle::CloseSkillList()
{
	//checkf(IsValid(SkillListWidget), TEXT("SkillListWidget is not valid"));
	//RemoveWidgetFromViewport(SkillListWidget);
	RequestChangeCursorMode(false, FInputModeGameOnly());
}

void UUIHandle::OpenMissionList()
{
	//checkf(IsValid(MissionListWidget), TEXT("MissionListWidget is not valid"));
	//RemoveWidgetFromViewport(MissionListWidget);
	RequestChangeCursorMode(true, FInputModeGameOnly());
}

void UUIHandle::CloseMissionList()
{
	//checkf(IsValid(MissionListWidget), TEXT("SkillListWidget is not valid"));
	//RemoveWidgetFromViewport(MissionListWidget);
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

void UUIHandle::LoadBaseWidgets(const UUIHandleSettings* UISettings)
{
	checkf(IsValid(UISettings), TEXT("UI Settings is invalid"));

	TitleWidgetClass = UISettings->TitleWidgetClass;
	MainWidgetClass = UISettings->MainWidgetClass;
	InGameWidgetClass = UISettings->InGameWidgetClass;
	EndingWidgetClass = UISettings->EndingWidgetClass;
}

void UUIHandle::LoadCoverWidgets(const UUIHandleSettings* UISettings)
{
	checkf(IsValid(UISettings), TEXT("UI Settings is invalid"));

	FadeWidgetClass = UISettings->FadeWidgetClass;
	OptionWidgetClass = UISettings->OptionWidgetClass;
	ShopWidgetClass = UISettings->ShopWidgetClass;
	ResultWidgetClass = UISettings->ResultWidgetClass;
	TextWidgetClass = UISettings->TextWidgetClass;
	CharacterListWidgetClass = UISettings->CharacterListWidgetClass;
	SkillListWidgetClass = UISettings->SkillListWidgetClass;
	MissionListWidgetClass = UISettings->MissionListWidgetClass;
}

void UUIHandle::CreateBaseWidgets()
{
	checkf(IsValid(EdmundGameInstance), TEXT("EdmundGameInstance is invalid"));

	checkf(IsValid(TitleWidgetClass), TEXT("TitleWidgetClass is invalid"));
	TitleWidget = CreateWidget<UBaseWidget>(EdmundGameInstance, TitleWidgetClass);
	TitleWidget->InitWidget(this);

	checkf(IsValid(MainWidgetClass), TEXT("MainWidgetClass is invalid"));
	MainWidget = CreateWidget<UBaseWidget>(EdmundGameInstance, MainWidgetClass);
	MainWidget->InitWidget(this);

	checkf(IsValid(InGameWidgetClass), TEXT("InGmaeWidgetClass is invalid"));
	InGameWidget = CreateWidget<UBaseWidget>(EdmundGameInstance, InGameWidgetClass);
	InGameWidget->InitWidget(this);

	checkf(IsValid(EndingWidgetClass), TEXT("EndingWidgetClass is invalid"));
	EndingWidget = CreateWidget<UBaseWidget>(EdmundGameInstance, EndingWidgetClass);
	EndingWidget->InitWidget(this);
}

void UUIHandle::CreateCoverWidgets()
{
	checkf(IsValid(EdmundGameInstance), TEXT("EdmundGameInstance is invalid"));

	checkf(IsValid(FadeWidgetClass), TEXT("FadeWidgetClass is invalid"));
	FadeWidget = CreateWidget<UBaseWidget>(EdmundGameInstance, FadeWidgetClass);
	FadeWidget->InitWidget(this);

	checkf(IsValid(OptionWidgetClass), TEXT("OptionWidgetClass is invalid"));
	OptionWidget = CreateWidget<UBaseWidget>(EdmundGameInstance, OptionWidgetClass);
	OptionWidget->InitWidget(this);

	checkf(IsValid(ResultWidgetClass), TEXT("ResultWidgetClass is invalid"));
	ResultWidget = CreateWidget<UBaseWidget>(EdmundGameInstance, ResultWidgetClass);
	ResultWidget->InitWidget(this);

	checkf(IsValid(ShopWidgetClass), TEXT("ShopWidgetClass is invalid"));
	ShopWidget = CreateWidget<UBaseWidget>(EdmundGameInstance, ShopWidgetClass);
	ShopWidget->InitWidget(this);

	checkf(IsValid(TextWidgetClass), TEXT("TextWidgetClass is invalid"));
	TextWidget = CreateWidget<UBaseWidget>(EdmundGameInstance, TextWidgetClass);
	TextWidget->InitWidget(this);

	checkf(IsValid(CharacterListWidgetClass), TEXT("CharacterListWidgetClass is invalid"));
	CharacterListWidget = CreateWidget<UBaseWidget>(EdmundGameInstance, CharacterListWidgetClass);
	CharacterListWidget->InitWidget(this);

	checkf(IsValid(SkillListWidgetClass), TEXT("SkillListWidgetClass is invalid"));
	SkillListWidget = CreateWidget<UBaseWidget>(EdmundGameInstance, SkillListWidgetClass);
	SkillListWidget->InitWidget(this);

	checkf(IsValid(MissionListWidgetClass), TEXT("MissionListWidgetClass is invalid"));
	MissionListWidget = CreateWidget<UBaseWidget>(EdmundGameInstance, MissionListWidgetClass);
	MissionListWidget->InitWidget(this);
}
