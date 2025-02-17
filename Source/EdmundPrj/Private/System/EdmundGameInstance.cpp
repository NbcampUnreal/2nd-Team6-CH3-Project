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

	if (IsValid(SceneHandle))
	{
		UE_LOG(LogTemp, Warning, TEXT("Success Get SceneHandle"));
	}

	if (IsValid(UIHandle))
	{
		UE_LOG(LogTemp, Warning, TEXT("Success Get UIHandle"));
	}

	if (IsValid(SoundHandle))
	{
		UE_LOG(LogTemp, Warning, TEXT("Success Get SoundHandle"));
	}

	if (IsValid(DataHandle))
	{
		UE_LOG(LogTemp, Warning, TEXT("Success Get DataHandle"));
	}
}

void UEdmundGameInstance::StartedGameState()
{
	EdmundGameState = GetWorld()->GetGameState<AEdmundGameState>();

	BindGameStateObserver();
	OnUIByScene();
	PlayBGMByScene();
}

void UEdmundGameInstance::BindGameStateObserver()
{
	if (!IsValid(EdmundGameState))
	{
		return;
	}

	if (!IsValid(UIHandle))
	{
		return;
	}
}

void UEdmundGameInstance::QuitGame()
{
	//게임이 종료될 때 필요한 기능 처리.
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

void UEdmundGameInstance::ActiveFade(bool bIsFadeIn)
{
	if (!IsValid(UIHandle))
	{
		return;
	}
}

void UEdmundGameInstance::OnUIByScene()
{
	if (!IsValid(SceneHandle))
	{
		return;
	}
	ActiveFade(true);
	SceneHandle->CheckCurrentScene();
}

void UEdmundGameInstance::OnPause()
{

}

void UEdmundGameInstance::ChangeCursorMode(bool bIsVisible)
{
	if (!IsValid(EdmundGameState))
	{
		return;
	}
}

void UEdmundGameInstance::ChangeInputMode(const FInputModeDataBase& InputMode)
{
	if (!IsValid(EdmundGameState))
	{
		return;
	}
}

void UEdmundGameInstance::EndMission()
{
	
}

void UEdmundGameInstance::DestroyedGameState()
{
	EdmundGameState = nullptr;
}

void UEdmundGameInstance::MoveScene(ESceneName SceneName)
{
	if (!IsValid(SceneHandle))
	{
		return;
	}
	ActiveFade(false);
	SceneHandle->OpenScene(SceneName);
}

void UEdmundGameInstance::MoveNextScene()
{
	if (!IsValid(SceneHandle))
	{
		return;
	}
	SceneHandle->MoveNextScene();
}

ESceneName UEdmundGameInstance::GetCurrentSceneName() const
{
	return ESceneName();
}

UDataTable* UEdmundGameInstance::GetDataTable(ETableName TableName) const
{
	return nullptr;
}

TMap<FName, int32>* UEdmundGameInstance::GetPlayerInfo() const
{
	return nullptr;
}

void UEdmundGameInstance::UpdatePlayerInfo(FName Type, int32 Price)
{
}

ECharacterType UEdmundGameInstance::GetPlayerType() const
{
	return ECharacterType();
}

void UEdmundGameInstance::SetPlayerType(ECharacterType Type)
{
}

void UEdmundGameInstance::AddPossessMoney(int32 Value)
{
}

int32 UEdmundGameInstance::GetPossessMoney() const
{
	return int32();
}

void UEdmundGameInstance::SetBGMVolume(float Volume)
{
}

void UEdmundGameInstance::SetEffectVolume(float Volume)
{
}

void UEdmundGameInstance::PlayBGMByScene()
{
}

void UEdmundGameInstance::PlayUISound(int32 Index)
{
}

void UEdmundGameInstance::PlayEffectSound(UAudioComponent* AudioComp, ESoundCategory SoundCategory, int32 Index)
{
}
