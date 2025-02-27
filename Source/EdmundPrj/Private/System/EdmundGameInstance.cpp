// Fill out your copyright notice in the Description page of Project Settings.


#include "System/EdmundGameInstance.h"
#include "System/SceneHandle.h"
#include "System/UIHandle.h"
#include "System/SoundHandle.h"
#include "System/DataHandle.h"
#include "Kismet/GameplayStatics.h"
#include "System/EdmundGameState.h"
#include "System/EdmundGameMode.h"
#include "System/DataStructure/MissionDataRow.h"
#include "System/DataStructure/SpawnerDataRow.h"

void UEdmundGameInstance::Init()
{
	Super::Init();

	SceneHandle = GetSubsystem<USceneHandle>();
	UIHandle = GetSubsystem<UUIHandle>();
	SoundHandle = GetSubsystem<USoundHandle>();
	DataHandle = GetSubsystem<UDataHandle>();

	checkf(IsValid(SceneHandle), TEXT("Fail Get SceneHandle"));
	checkf(IsValid(UIHandle), TEXT("Fail Get UIHandle"));
	checkf(IsValid(SoundHandle), TEXT("Fail Get SoundHandle"));
	checkf(IsValid(DataHandle), TEXT("Fail Get DataHandle"));

	SceneHandle->InitSceneHandle();
	DataHandle->InitDataHandle(this);
	UIHandle->InitUIHandle(this);
	SoundHandle->InitSoundHandle(this);
}

void UEdmundGameInstance::RequestGameStart(AEdmundGameMode* NewGameMode, AEdmundGameState* NewGameState)
{
	EdmundGameMode = NewGameMode;
	EdmundGameState = NewGameState;

	ApplyCurrentDataToGameMode();
}

void UEdmundGameInstance::ApplyCurrentDataToGameMode()
{
	checkf(IsValid(EdmundGameMode), TEXT("EdmundGameMode is invalid"));
	BindGameStateObserver();

	ESceneType CurrentScene = GetCurrentSceneName();
	const TArray<FMissionDataRow*> CurrentMissionData = GetCurrentMissionData(CurrentScene);
	const TArray<FSpawnerDataRow*> CurrentSpawnerData = GetCurrentSpawnerData(CurrentScene);

	EdmundGameMode->InitGameMode(this, CurrentMissionData, CurrentSpawnerData);

	StartMission(CurrentScene);
}

void UEdmundGameInstance::StartMission(ESceneType SceneType)
{
	OnUIByScene();
	PlayBGMByScene();
	EdmundGameMode->StartMission(SceneType);
}

void UEdmundGameInstance::BindGameStateObserver() const
{
	checkf(IsValid(EdmundGameState), TEXT("EdmundGameState is invalid"));
	checkf(UIHandle, TEXT("UIHandle is invalid"));

	for (auto& Observer : UIHandle->GetUIObservers())
	{
		EdmundGameState->RegisterGameStateObserver(Observer);
	}
}

void UEdmundGameInstance::QuitGame() const
{
	//게임이 종료될 때 필요한 기능 처리.
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

void UEdmundGameInstance::OnUIByScene() const
{
	checkf(SceneHandle, TEXT("SceneHandle is invalid"));
	checkf(UIHandle, TEXT("UIHandle is invalid"));

	UIHandle->FadeIn();
	SceneHandle->CheckCurrentScene();
	UIHandle->AddToViewportBySceneType(SceneHandle->GetCurrentScene());
}

void UEdmundGameInstance::OnPause() const
{
	checkf(UIHandle, TEXT("UIHandle is invalid"));
	UIHandle->AddToViewportByCoverType(EWidgetType::OptionWidget);
}

void UEdmundGameInstance::ChangeCursorMode(const bool bIsVisible) const
{
	checkf(IsValid(EdmundGameState), TEXT("EdmundGameState is invalid"));
	EdmundGameState->ChangeCursorMode(bIsVisible);
}

void UEdmundGameInstance::ChangeInputMode(const FInputModeDataBase& InputMode) const
{
	checkf(IsValid(EdmundGameState), TEXT("EdmundGameState is invalid"));
	EdmundGameState->ChangeInputMode(InputMode);
}

void UEdmundGameInstance::EndMission(const bool bIsClear) const
{
	checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));
	UIHandle->AddToViewportByCoverType(EWidgetType::ResultWidget);

	if (bIsClear)
	{
		checkf(IsValid(DataHandle), TEXT("DataHandle is Invalid"));
		DataHandle->UpdateClearMission(GetCurrentSceneName());
	}
}

void UEdmundGameInstance::DestroyedGameState() 
{
	EdmundGameMode = nullptr;
	EdmundGameState = nullptr;
}

void UEdmundGameInstance::RequestSceneMove(const bool bIsNext, ESceneType SceneType) const
{
	checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));
	UIHandle->FadeOut(bIsNext, SceneType);
}

void UEdmundGameInstance::MoveScene(const ESceneType SceneType) const
{
	checkf(IsValid(SceneHandle), TEXT("SceneHandle is invalid"));
	SceneHandle->OpenScene(SceneType);
}

void UEdmundGameInstance::MoveNextScene() const
{
	checkf(IsValid(SceneHandle), TEXT("SceneHandle is invalid"));
	SceneHandle->MoveNextScene();
}

ESceneType UEdmundGameInstance::GetCurrentSceneName() const
{
	checkf(IsValid(SceneHandle), TEXT("SceneHandle is invalid"));
	return SceneHandle->GetCurrentScene();
}

const TArray<FMissionDataRow*>& UEdmundGameInstance::GetCurrentMissionData(ESceneType SceneType) const
{
	checkf(IsValid(DataHandle), TEXT("DataHandle is Invalid"));
	return DataHandle->GetMissionDataBySceneType(SceneType);
}

const TArray<FSpawnerDataRow*>& UEdmundGameInstance::GetCurrentSpawnerData(ESceneType SceneType) const
{
	checkf(IsValid(DataHandle), TEXT("DataHandle is Invalid"));
	return DataHandle->GetSpawnerDataBySceneType(SceneType);
}

const TArray<FShopCatalogRow*>& UEdmundGameInstance::GetAdvanceState() const
{
	checkf(IsValid(DataHandle), TEXT("DataHandle is Invalid"));
	return DataHandle->GetCurrentAdvance();
}

const FShopCatalogRow* UEdmundGameInstance::GetAdvanceState(const FName& TargetRow) const
{
	checkf(IsValid(DataHandle), TEXT("DataHandle is Invalid"));
	return DataHandle->GetCurrentAdvanceByRowName(TargetRow);
}

bool UEdmundGameInstance::UpdateAdvanceState(const FName RowName, const int32 UpdateValue) const
{
	checkf(IsValid(DataHandle), TEXT("DataHandle is Invalid"));
	return DataHandle->UpdateCurrentAdvance(RowName, UpdateValue);
}

ECharacterType UEdmundGameInstance::GetPlayerType() const
{
	checkf(IsValid(DataHandle), TEXT("DataHandle is Invalid"));
	return DataHandle->GetPlayerType();
}

void UEdmundGameInstance::SetPlayerType(const ECharacterType Type) const
{
	checkf(IsValid(DataHandle), TEXT("DataHandle is Invalid"));
	DataHandle->SetPlayerType(Type);
}

void UEdmundGameInstance::CancleSelectedType()
{
	checkf(IsValid(EdmundGameState), TEXT("EdmundGameState is invalid"));
	EdmundGameState->CancleSelectedCharacter();
}

const TArray<FCharacterDataRow*>& UEdmundGameInstance::GetCharacterData() const
{
	checkf(IsValid(DataHandle), TEXT("DataHandle is Invalid"));
	return DataHandle->GetCharacterData();
}

void UEdmundGameInstance::AddPossessMoney(const int32 Value) const
{
	checkf(IsValid(DataHandle), TEXT("DataHandle is Invalid"));
	DataHandle->AddMoney(Value);
}

int32 UEdmundGameInstance::GetPossessMoney() const
{
	checkf(IsValid(DataHandle), TEXT("DataHandle is Invalid"));
	return DataHandle->GetMoney();
}

bool UEdmundGameInstance::CheckClearedMission(const int32 Index) const
{
	checkf(IsValid(DataHandle), TEXT("DataHandle is Invalid"));
	return DataHandle->GetIsClearedMission(Index);
}

void UEdmundGameInstance::OnSkillListUI() const
{
	checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));
	UIHandle->AddToViewportByCoverType(EWidgetType::SkillListWidget);
}

void UEdmundGameInstance::ApplySelectSkill(const int32 Index) const
{
	checkf(IsValid(EdmundGameState), TEXT("EdmundGameState is invalid"));
	EdmundGameState->ApplySelectedSkill(Index);
}

const TArray<FPlayerSkillRow*>& UEdmundGameInstance::GetPlayerSkillData() const
{
	checkf(IsValid(DataHandle), TEXT("DataHandle is Invalid"));
	return DataHandle->GetPlayerSkillData();
}

const TArray<FPlayerSkillRow*>& UEdmundGameInstance::GetRandomSkillSet() const
{
	checkf(IsValid(EdmundGameState), TEXT("EdmundGameState is invalid"));
	return EdmundGameState->GetRandomSkillSet();
}

void UEdmundGameInstance::SetBGMVolume(const float Volume) const
{
	checkf(IsValid(SoundHandle), TEXT("SoundHandle is invalid"));
	SoundHandle->UpdateBGMVolume(Volume);
}

float UEdmundGameInstance::GetBGMVolume() const
{
	checkf(IsValid(SoundHandle), TEXT("SoundHandle is invalid"));
	return SoundHandle->GetBGMVolume();
}

void UEdmundGameInstance::SetEffectVolume(const float Volume) const
{
	checkf(IsValid(SoundHandle), TEXT("SoundHandle is invalid"));
	SoundHandle->UpdateEffectVolume(Volume);
}

float UEdmundGameInstance::GetEffectVolume() const
{
	checkf(IsValid(SoundHandle), TEXT("SoundHandle is invalid"));
	return SoundHandle->GetEffectVolume();
}

void UEdmundGameInstance::PlayBGMByScene() const
{

}

void UEdmundGameInstance::PlayUISound(const int32 Index) const
{

}

void UEdmundGameInstance::PlayEffectSound(const UAudioComponent* AudioComp, const ESoundType SoundType, const int32 Index) const
{

}
