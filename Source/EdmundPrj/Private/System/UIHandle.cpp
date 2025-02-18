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
		break;

	case EWidgetType::ShopWidget:
		CurrentCoverWidget = ShopWidget;
		break;

	case EWidgetType::ResultWidget:
		CurrentCoverWidget = ResultWidget;
		break;

	case EWidgetType::TextWidget:
		CurrentCoverWidget = TextWidget;
		break;

	case EWidgetType::CharacterListWidget:
		CurrentCoverWidget = CharacterListWidget;
		break;

	case EWidgetType::SkillListWidget:
		CurrentCoverWidget = SkillListWidget;
		break;

	case EWidgetType::MissionListWidget:
		CurrentCoverWidget = MissionListWidget;
		break;

	default:
		checkNoEntry();
		break;
	}

	AddWidgetToViewport(CurrentCoverWidget);
	CurrentCoverWidget->Action();
	CurrentCoverWidget->PlayAddAnim();
}

void UUIHandle::RemoveCoverFromViewport()
{
	if (IsValid(CurrentCoverWidget))
	{
		RemoveWidgetFromViewport(CurrentCoverWidget);
		CurrentCoverWidget = nullptr;
	}
}

void UUIHandle::FadeIn()
{
	checkf(IsValid(FadeWidget), TEXT("FadeWidget is not valid"));

	FadeWidget->AddToViewport(20);
	FadeWidget->Action();

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			RemoveWidgetFromViewport(FadeWidget);
		}), 0.5f, false);
}

void UUIHandle::FadeOut()
{
	checkf(IsValid(FadeWidget), TEXT("FadeWidget is not valid"));

	FadeWidget->AddToViewport(20);
	FadeWidget->Action();
	RequestChangeCursorMode(true, FInputModeUIOnly());
}

void UUIHandle::OpenOption()
{
	checkf(IsValid(OptionWidget), TEXT("OptionWidget is not valid"));
	AddWidgetToViewport(OptionWidget);
	RequestChangeCursorMode(true, FInputModeUIOnly());
}

void UUIHandle::CloseOption()
{
	checkf(IsValid(OptionWidget), TEXT("OptionWidget is not valid"));
	RemoveWidgetFromViewport(OptionWidget);
	RequestChangeCursorMode(false, FInputModeGameOnly());
}

void UUIHandle::OpenText()
{
	checkf(IsValid(TextWidget), TEXT("TextWidget is not valid"));
	AddWidgetToViewport(TextWidget);
	RequestChangeCursorMode(true, FInputModeUIOnly());
}

void UUIHandle::CloseText()
{
	checkf(IsValid(TextWidget), TEXT("TextWidget is not valid"));
	RemoveWidgetFromViewport(TextWidget);
	RequestChangeCursorMode(false, FInputModeGameOnly());
}

void UUIHandle::OpenShop()
{
	checkf(IsValid(ShopWidget), TEXT("ShopWidget is not valid"));
	AddWidgetToViewport(ShopWidget);
	RequestChangeCursorMode(true, FInputModeUIOnly());
}

void UUIHandle::CloseShop()
{
	checkf(IsValid(ShopWidget), TEXT("ShopWidget is not valid"));
	RemoveWidgetFromViewport(ShopWidget);
	RequestChangeCursorMode(false, FInputModeGameOnly());
}

void UUIHandle::OpenResult()
{
	checkf(IsValid(ResultWidget), TEXT("ResultWidget is not valid"));
	AddWidgetToViewport(ResultWidget);
	RequestChangeCursorMode(true, FInputModeUIOnly());
}

void UUIHandle::CloseResult()
{
	checkf(IsValid(ResultWidget), TEXT("ResultWidget is not valid"));
	RemoveWidgetFromViewport(ResultWidget);
	RequestChangeCursorMode(false, FInputModeGameOnly());
}

void UUIHandle::OpenCharacterList()
{
	checkf(IsValid(CharacterListWidget), TEXT("CharacterListWidget is not valid"));
	AddWidgetToViewport(CharacterListWidget);
	RequestChangeCursorMode(true, FInputModeUIOnly());
}

void UUIHandle::CloseCharacterList()
{
	checkf(IsValid(CharacterListWidget), TEXT("CharacterListWidget is not valid"));
	RemoveWidgetFromViewport(CharacterListWidget);
	RequestChangeCursorMode(false, FInputModeGameOnly());
}

void UUIHandle::OpenSkillList()
{
	checkf(IsValid(SkillListWidget), TEXT("SkillListWidget is not valid"));
	AddWidgetToViewport(SkillListWidget);
	RequestChangeCursorMode(true, FInputModeUIOnly());
}

void UUIHandle::CloseSkillList()
{
	checkf(IsValid(SkillListWidget), TEXT("SkillListWidget is not valid"));
	RemoveWidgetFromViewport(SkillListWidget);
	RequestChangeCursorMode(false, FInputModeGameOnly());
}

void UUIHandle::OpenMissionList()
{
	checkf(IsValid(MissionListWidget), TEXT("MissionListWidget is not valid"));
	RemoveWidgetFromViewport(MissionListWidget);
	RequestChangeCursorMode(true, FInputModeGameOnly());
}

void UUIHandle::CloseMissionList()
{
	checkf(IsValid(MissionListWidget), TEXT("SkillListWidget is not valid"));
	RemoveWidgetFromViewport(MissionListWidget);
	RequestChangeCursorMode(false, FInputModeGameOnly());
}

void UUIHandle::ClickedCloseCoverWidget() const
{
	checkf(IsValid(CurrentCoverWidget), TEXT("Current Cover Widget is not valid"));
	CurrentCoverWidget->PlayRemoveAnim();
}

void UUIHandle::ClickedMoveToTitle() const
{
	checkf(IsValid(EdmundGameInstance), TEXT("GameInstance is not valid"));
	EdmundGameInstance->MoveScene(ESceneType::Title);
}

void UUIHandle::ClickedMoveToMain() const
{
	checkf(IsValid(EdmundGameInstance), TEXT("GameInstance is not valid"));
	EdmundGameInstance->MoveScene(ESceneType::Main);
}

void UUIHandle::ClickedMoveToNext() const
{
	checkf(IsValid(EdmundGameInstance), TEXT("GameInstance is not valid"));
	EdmundGameInstance->MoveNextScene();
}

void UUIHandle::ClickedMoveToMission(const int32 Index) const
{
	checkf(IsValid(EdmundGameInstance), TEXT("GameInstance is not valid"));

	switch (Index)
	{
	case 0:
		EdmundGameInstance->MoveScene(ESceneType::Mission1);
		break;

	case 1:
		EdmundGameInstance->MoveScene(ESceneType::Mission2);
		break;

	case 2:
		EdmundGameInstance->MoveScene(ESceneType::Mission3);
		break;

	default:
		checkNoEntry();
		break;
	}
}

void UUIHandle::ClickedQuitGame() const
{
	checkf(IsValid(EdmundGameInstance), TEXT("EdmundGameInstance is Not valid"));
	EdmundGameInstance->QuitGame();
}

void UUIHandle::ClickedBGMVolume(const float Volume) const
{
	checkf(IsValid(EdmundGameInstance), TEXT("EdmundGameInstance is Not valid"));
	EdmundGameInstance->SetBGMVolume(Volume);
}

void UUIHandle::ClickedEffectVolume(const float Volume) const
{
	checkf(IsValid(EdmundGameInstance), TEXT("EdmundGameInstance is Not valid"));
	EdmundGameInstance->SetEffectVolume(Volume);
}

void UUIHandle::RequestChangeCursorMode(const bool bIsVisible, const FInputModeDataBase& InputMode)
{
	checkf(IsValid(EdmundGameInstance), TEXT("EdmundGameInstance is Not valid"));

	if (!bBaseCursorMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("Change CursorMode"));
		EdmundGameInstance->ChangeCursorMode(bIsVisible);
		EdmundGameInstance->ChangeInputMode(InputMode);
	}
}

void UUIHandle::AddWidgetToViewport(UBaseWidget* Widget)
{
	checkf(IsValid(Widget), TEXT("Widget is not valid"));

	Widget->AddToViewport(ViewCount);
	++ViewCount;
}

void UUIHandle::RemoveWidgetFromViewport(UBaseWidget* Widget)
{
	checkf(IsValid(Widget), TEXT("Widget is not valid"));

	Widget->RemoveFromViewport();
	--ViewCount;
}

void UUIHandle::LoadBaseWidgets(const UUIHandleSettings* UISettings)
{
	checkf(IsValid(UISettings), TEXT("UI Settings is not valid"));

	TitleWidgetClass = UISettings->TitleWidgetClass;
	MainWidgetClass = UISettings->MainWidgetClass;
	InGameWidgetClass = UISettings->InGameWidgetClass;
	EndingWidgetClass = UISettings->EndingWidgetClass;
}

void UUIHandle::LoadCoverWidgets(const UUIHandleSettings* UISettings)
{
	checkf(IsValid(UISettings), TEXT("UI Settings is not valid"));

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
	checkf(IsValid(EdmundGameInstance), TEXT("EdmundGameInstance is Not valid"));

	checkf(IsValid(TitleWidgetClass), TEXT("TitleWidgetClass is Not valid"));
	TitleWidget = CreateWidget<UBaseWidget>(EdmundGameInstance, TitleWidgetClass);
	TitleWidget->InitWidget(this);

	checkf(IsValid(MainWidgetClass), TEXT("MainWidgetClass is Not valid"));
	MainWidget = CreateWidget<UBaseWidget>(EdmundGameInstance, MainWidgetClass);
	MainWidget->InitWidget(this);

	checkf(IsValid(InGameWidgetClass), TEXT("InGmaeWidgetClass is Not valid"));
	InGameWidget = CreateWidget<UBaseWidget>(EdmundGameInstance, InGameWidgetClass);
	InGameWidget->InitWidget(this);

	checkf(IsValid(EndingWidgetClass), TEXT("EndingWidgetClass is Not valid"));
	EndingWidget = CreateWidget<UBaseWidget>(EdmundGameInstance, EndingWidgetClass);
	EndingWidget->InitWidget(this);
}

void UUIHandle::CreateCoverWidgets()
{
	checkf(IsValid(EdmundGameInstance), TEXT("EdmundGameInstance is Not valid"));

	checkf(IsValid(FadeWidgetClass), TEXT("FadeWidgetClass is Not valid"));
	FadeWidget = CreateWidget<UBaseWidget>(EdmundGameInstance, FadeWidgetClass);
	FadeWidget->InitWidget(this);

	checkf(IsValid(OptionWidgetClass), TEXT("OptionWidgetClass is Not valid"));
	OptionWidget = CreateWidget<UBaseWidget>(EdmundGameInstance, OptionWidgetClass);
	OptionWidget->InitWidget(this);

	checkf(IsValid(ResultWidgetClass), TEXT("ResultWidgetClass is Not valid"));
	ResultWidget = CreateWidget<UBaseWidget>(EdmundGameInstance, ResultWidgetClass);
	ResultWidget->InitWidget(this);

	checkf(IsValid(ShopWidgetClass), TEXT("ShopWidgetClass is Not valid"));
	ShopWidget = CreateWidget<UBaseWidget>(EdmundGameInstance, ShopWidgetClass);
	ShopWidget->InitWidget(this);

	checkf(IsValid(TextWidgetClass), TEXT("TextWidgetClass is Not valid"));
	TextWidget = CreateWidget<UBaseWidget>(EdmundGameInstance, TextWidgetClass);
	TextWidget->InitWidget(this);

	checkf(IsValid(CharacterListWidgetClass), TEXT("CharacterListWidgetClass is Not valid"));
	CharacterListWidget = CreateWidget<UBaseWidget>(EdmundGameInstance, CharacterListWidgetClass);
	CharacterListWidget->InitWidget(this);

	checkf(IsValid(SkillListWidgetClass), TEXT("SkillListWidgetClass is Not valid"));
	SkillListWidget = CreateWidget<UBaseWidget>(EdmundGameInstance, SkillListWidgetClass);
	SkillListWidget->InitWidget(this);

	checkf(IsValid(MissionListWidgetClass), TEXT("MissionListWidgetClass is Not valid"));
	MissionListWidget = CreateWidget<UBaseWidget>(EdmundGameInstance, MissionListWidgetClass);
	MissionListWidget->InitWidget(this);
}
