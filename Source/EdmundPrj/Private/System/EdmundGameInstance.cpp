// Fill out your copyright notice in the Description page of Project Settings.


#include "System/EdmundGameInstance.h"
#include "System/SceneHandle.h"
#include "System/UIHandle.h"
#include "System/SoundHandle.h"
#include "System/DataHandle.h"
#include "Kismet/GameplayStatics.h"
#include "System/EdmundGameState.h"

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
	UIHandle->InitUIHandle(this);
	SoundHandle->InitSoundHandle(this);
	DataHandle->InitDataHandle(this);
}

void UEdmundGameInstance::StartedGameState()
{
	EdmundGameState = GetWorld()->GetGameState<AEdmundGameState>();

	BindGameStateObserver();
	OnUIByScene();
	PlayBGMByScene();
}

void UEdmundGameInstance::BindGameStateObserver() const
{
	checkf(IsValid(EdmundGameState), TEXT("EdmundGameState is invalid"));
	checkf(UIHandle, TEXT("UIHandle is invalid"));

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

void UEdmundGameInstance::ChangeSelectMode(const bool bIsSelect) const
{
	checkf(IsValid(EdmundGameState), TEXT("EdmundGameState is invalid"));
	EdmundGameState->ChangeSelectMode(bIsSelect);
}

void UEdmundGameInstance::EndMission() const
{
	checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));
	// 소지금 등 데이터 저장 필요
	//UIHandle->AddToViewportByCoverType(EWidgetType::ResultWidget);
	UIHandle->AddToViewportByCoverType(EWidgetType::SkillListWidget);
}

void UEdmundGameInstance::DestroyedGameState() 
{
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

UDataTable* UEdmundGameInstance::GetDataTable(const ETableType TableType) const
{
	return nullptr;
}

TMap<FName, int32>* UEdmundGameInstance::GetPlayerInfo() const
{
	return nullptr;
}

void UEdmundGameInstance::UpdatePlayerInfo(const FName Type, const int32 Price) const
{
}

ECharacterType UEdmundGameInstance::GetPlayerType() const
{
	return ECharacterType();
}

void UEdmundGameInstance::SetPlayerType(const ECharacterType Type) const
{
}

void UEdmundGameInstance::AddPossessMoney(const int32 Value) const
{
}

int32 UEdmundGameInstance::GetPossessMoney() const
{
	return int32();
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
