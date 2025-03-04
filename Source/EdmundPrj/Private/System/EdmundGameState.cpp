// Fill out your copyright notice in the Description page of Project Settings.


#include "System/EdmundGameState.h"
#include "System/EdmundGameInstance.h"
#include "System/EdmundGameMode.h"
#include "System/MainLevelPlayerController.h"
#include "System/DataStructure/PlayerSkillRow.h"
#include "System/Observer/GameStateObserver.h"
#include "Player/BaseCharacter.h"
#include "System/MissionHandle.h"
#include "System/SpawnerHandle.h"
#include "Player/SkillManager.h"
#include "Kismet/GameplayStatics.h"

AEdmundGameState::AEdmundGameState() : Super()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEdmundGameState::BeginPlay()
{
	Super::BeginPlay();

	EdmundGameInstance = GetWorld()->GetGameInstance<UEdmundGameInstance>();
	EdmundGameMode = GetWorld()->GetAuthGameMode<AEdmundGameMode>();
	PlayerController = GetWorld()->GetPlayerControllerIterator()->Get();

	checkf(IsValid(EdmundGameInstance), TEXT("GameInstance is invalid"));
	EdmundGameInstance->RequestGameStart(EdmundGameMode, this);
}

void AEdmundGameState::Tick(float DeltaTime)
{
	if (StoryIndex < 0)
	{
		SetActorTickEnabled(false);
		return;
	}

	CurrentTime += DeltaTime;

	if (CurrentTime >= IntervalTime * SlowTime)
	{
		CurrentTime = 0;
		
		CurrentText = StoryText.ToString().LeftChop(StoryIndex);
		NotifyPrintText(CurrentText);
		--StoryIndex;
	}
}

void AEdmundGameState::BeginDestroy()
{
	Super::BeginDestroy();
}

void AEdmundGameState::ChangeCursorMode(bool bIsValid)
{
	checkf(IsValid(PlayerController), TEXT("PlayerController is invalid"));
	PlayerController->bShowMouseCursor = bIsValid;
}

void AEdmundGameState::ChangeInputMode(const FInputModeDataBase& InputMode)
{
	checkf(IsValid(PlayerController), TEXT("PlayerController is invalid"));
	PlayerController->SetInputMode(InputMode);
	PlayerController->FlushPressedKeys();
}

void AEdmundGameState::AddCurrentLevelMoney(int32 Money)
{
	if (Money < 0)
	{
		return;
	}

	CurrentLevelMoney += Money;
}

void AEdmundGameState::PrintStoryText(const FText& TargetText)
{
	StoryText = TargetText;
	StoryLastIndex = StoryText.ToString().Len();
	StoryIndex = StoryLastIndex;
	CurrentTime = 0;

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), SlowTime);
	SetActorTickEnabled(true);
}

void AEdmundGameState::OnEndedCurrentStory()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
	SetActorTickEnabled(false);
}

void AEdmundGameState::StopPrintStory()
{
	if (StoryIndex < 0)
	{
		if (!EdmundGameMode->CheckRemainCurrentStory())
		{
			OnEndedCurrentStory();
		}
	}
	else
	{
		StoryIndex = 0;
	}
}

void AEdmundGameState::SkipCurrentStory()
{
	EdmundGameMode->OnEndedCurrentStory();
	StoryIndex = 0;
	OnEndedCurrentStory();
}

void AEdmundGameState::InitMainLevelPlayerController()
{
	checkf(IsValid(PlayerController), TEXT("PlayerController is invalid"));
	AMainLevelPlayerController* MainLevelPlayerController = Cast<AMainLevelPlayerController>(PlayerController);
	checkf(IsValid(MainLevelPlayerController), TEXT("MainLevelPlayerController is Invalie"));
	MainLevelPlayerController->InitMainLevelCharacters(EdmundGameInstance->GetCharacterData(), EdmundGameInstance->GetPlayerType(), this);
}

void AEdmundGameState::InitSoundMap(const TMap<ESoundType, TObjectPtr<USoundBase>> PlayerSound, const TMap<EMonsterType, TMap<ESoundType, TObjectPtr<USoundBase>>> MonsterSound, const TMap<ENpcType, TMap<ESoundType, TObjectPtr<USoundBase>>> NpcSound, const TMap<EItemType, TMap<ESoundType, TObjectPtr<USoundBase>>> ItemSound)
{
	PlayerSoundSet = PlayerSound;
	MonsterSoundSet = MonsterSound;
	NpcSoundSet = NpcSound;
	ItemSoundSet = ItemSound;
}

void AEdmundGameState::InitSkillData(const TArray<FPlayerSkillRow*> PlayerSkillDataSet)
{
	SkillDataSet = PlayerSkillDataSet;

	for (FPlayerSkillRow* PlayerSkillRow : SkillDataSet)
	{
		CurrentSkillMap.Add({ PlayerSkillRow, 0 });
	}

	RandomSkillSet.Empty();
}

void AEdmundGameState::CalculateSkillList()
{
	int32 RandomNum = FMath::RandRange(0, SkillDataSet.Num() - 1);

	FPlayerSkillRow* RandomSkill = SkillDataSet[RandomNum];

	if (RandomSkillSet.Contains(RandomSkill))
	{
		CalculateSkillList();
	}
	else
	{
		if (RandomSkill->MaxLevel == CurrentSkillMap[RandomSkill])
		{
			CalculateSkillList();
		}
		else
		{
			RandomSkillSet.Add(RandomSkill);
		}
	}
}

void AEdmundGameState::CreateRandomSkillSet()
{
	RandomSkillSet.Empty();

	for (int32 i = 0; i < 3; i++)
	{
		CalculateSkillList();
	}

	NotifyCreateRandomSkill();
	EdmundGameInstance->OnSkillListUI();
	EdmundGameInstance->PlayUISound(EUISoundType::LevelUp);
}

const TArray<FPlayerSkillRow*>& AEdmundGameState::GetRandomSkillSet() const
{
	return RandomSkillSet;
}

void AEdmundGameState::ApplySelectedSkill(const int32 Index)
{
	if (Index >= RandomSkillSet.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected Skill Index is invalid"));
	}

	FPlayerSkillRow* SelectedSkill = RandomSkillSet[Index];

	++CurrentSkillMap[SelectedSkill];

	UE_LOG(LogTemp, Warning, TEXT("Selected Skill Name is %s"), *SelectedSkill->SkillName.ToString());
	USkillManager* SkillManager = PlayerPawn->FindComponentByClass<USkillManager>();
	if (IsValid(SkillManager) && SelectedSkill != nullptr)
	{
		SkillManager->ActivateSkill(*SelectedSkill);
	}
}

void AEdmundGameState::SetSelectedCharacter(AActor* Character)
{
	checkf(IsValid(Character), TEXT("Selected Character is invalid"));

	ABaseCharacter* TargetActor = Cast<ABaseCharacter>(Character);
	NotifySelectCharacterType(TargetActor->GetCharacterType());
}

void AEdmundGameState::CancleSelectedCharacter()
{
	AMainLevelPlayerController* MainLevelPlayerController = Cast<AMainLevelPlayerController>(PlayerController);
	checkf(IsValid(MainLevelPlayerController), TEXT("MainLevelPlayerController is Invalie"));
	MainLevelPlayerController->SetTargetToNull();
}

void AEdmundGameState::CheckClosedPlayerType(ECharacterType Type)
{
	AMainLevelPlayerController* MainLevelPlayerController = Cast<AMainLevelPlayerController>(PlayerController);
	checkf(IsValid(MainLevelPlayerController), TEXT("MainLevelPlayerController is Invalie"));
	MainLevelPlayerController->CompareType(Type);
}

void AEdmundGameState::SetMissionHandle(AMissionHandle* NewMissionHandle)
{
	MissionHandle = NewMissionHandle;
}

void AEdmundGameState::SetSpawnerHandle(ASpawnerHandle* NewSpawnerHandle)
{
	SpawnerHandle = NewSpawnerHandle;
}

void AEdmundGameState::SetEffectVolume(const float Volume)
{
	EffectVolume = Volume;
}

void AEdmundGameState::PlayPlayerSound(UAudioComponent* AudioComp, ESoundType SoundType)
{
	if (!IsValid(AudioComp))
	{
		return;
	}

	if (!PlayerSoundSet.Contains(SoundType))
	{
		return;
	}

	USoundBase* SoundSource = PlayerSoundSet[SoundType];

	if (!IsValid(SoundSource))
	{
		return;
	}

	AudioComp->SetSound(SoundSource);
	AudioComp->SetVolumeMultiplier(EffectVolume);
	AudioComp->Play();
}

void AEdmundGameState::PlayMonsterSound(UAudioComponent* AudioComp, EMonsterType MonsterType, ESoundType SoundType)
{
	if (!IsValid(AudioComp))
	{
		return;
	}

	if (!MonsterSoundSet.Contains(MonsterType))
	{
		return;
	}

	if (!MonsterSoundSet[MonsterType].Contains(SoundType))
	{
		return;
	}

	USoundBase* SoundSource = MonsterSoundSet[MonsterType][SoundType];

	if (!IsValid(SoundSource))
	{
		return;
	}

	AudioComp->SetSound(SoundSource);
	AudioComp->SetVolumeMultiplier(EffectVolume);
	AudioComp->Play();
}

void AEdmundGameState::PlayNpcSound(UAudioComponent* AudioComp, ENpcType NpcType, ESoundType SoundType)
{
	if (!IsValid(AudioComp))
	{
		return;
	}

	if (!NpcSoundSet.Contains(NpcType))
	{
		return;
	}

	if (!NpcSoundSet[NpcType].Contains(SoundType))
	{
		return;
	}

	USoundBase* SoundSource = NpcSoundSet[NpcType][SoundType];

	if (!IsValid(SoundSource))
	{
		return;
	}

	AudioComp->SetSound(SoundSource);
	AudioComp->SetVolumeMultiplier(EffectVolume);
	AudioComp->Play();
}

void AEdmundGameState::PlayItemSound(UAudioComponent* AudioComp, EItemType ItemType, ESoundType SoundType)
{
	if (!IsValid(AudioComp))
	{
		return;
	}

	if (!ItemSoundSet.Contains(ItemType))
	{
		return;
	}

	if (!ItemSoundSet[ItemType].Contains(SoundType))
	{
		return;
	}

	USoundBase* SoundSource = ItemSoundSet[ItemType][SoundType];

	if (!IsValid(SoundSource))
	{
		return;
	}

	if (AudioComp->IsPlaying())
	{
		AudioComp->Stop();
	}

	AudioComp->SetSound(SoundSource);
	AudioComp->SetVolumeMultiplier(EffectVolume);
	AudioComp->Play();
}

const TArray<FShopCatalogRow*>& AEdmundGameState::GetPlayerAdvancedData() const
{
	checkf(IsValid(EdmundGameInstance), TEXT("Game Instance is invalid"));
	return EdmundGameInstance->GetAdvanceState();
}

void AEdmundGameState::OnPressedPauseKey()
{
	checkf(IsValid(EdmundGameInstance), TEXT("Game Instance is invalid"));
	EdmundGameInstance->OnPause();
	PlayerController->SetPause(true);
	ChangeCursorMode(true);
	ChangeInputMode(FInputModeUIOnly());
}

void AEdmundGameState::RequestOnPause()
{
	PlayerController->SetPause(true);
}

void AEdmundGameState::RequestEndPause()
{
	PlayerController->SetPause(false);
}

void AEdmundGameState::RequestInteraction()
{
	checkf(IsValid(MissionHandle), TEXT("Mission Handle is invalid"));
	MissionHandle->OnPressedKeyFromPlayer();
}

void AEdmundGameState::EndCurrentLevel(bool bIsClear)
{
	checkf(IsValid(EdmundGameInstance), TEXT("GameInstance is invalid"));

	int32 TotalMoney = EdmundGameInstance->GetPossessMoney();

	if (bIsClear)
	{
		EdmundGameInstance->AddPossessMoney(MissionClearMoney);
		NotifyResultValue(CurrentLevelMoney, TotalMoney, MissionClearMoney);
		NotifyIsGameClear(true);
	}
	else
	{
		NotifyResultValue(CurrentLevelMoney, TotalMoney, 0);
		NotifyIsGameClear(false);
	}
	
	EdmundGameInstance->AddPossessMoney(CurrentLevelMoney);
}

void AEdmundGameState::RegisterGameStateObserver(const TScriptInterface<IGameStateObserver> Observer)
{
	Observers.Add(Observer);
}

void AEdmundGameState::UnregisterGameStateObserver(const TScriptInterface<IGameStateObserver> Observer)
{
	Observers.Remove(Observer);
}

void AEdmundGameState::NotifyUpdateNotifyText(const FString& NotifyText)
{
	for (TScriptInterface<IGameStateObserver> Observer : Observers)
	{
		if (!IsValid(Observer.GetObject()))
		{
			continue;
		}
		Observer->ChangedNotifyText(NotifyText);
	}
}

void AEdmundGameState::NotifyUpdateMissionText(const FString& MissionText)
{
	for (TScriptInterface<IGameStateObserver> Observer : Observers)
	{
		if (!IsValid(Observer.GetObject()))
		{
			continue;
		}
		Observer->ChangedMissionText(MissionText);
	}
}

void AEdmundGameState::NotifyPlayerHp(const int32 MaxHp, const int32 CurrentHp)
{
	if (EdmundGameInstance->GetCurrentSceneName() == ESceneType::Main)
	{
		return;
	}

	for (TScriptInterface<IGameStateObserver> Observer : Observers)
	{
		if (!IsValid(Observer.GetObject()))
		{
			continue;
		}
		Observer->ChangedPlayerHp(MaxHp, CurrentHp);
	}

	if (CurrentHp <= 0)
	{
		EdmundGameMode->FailMission();
	}
}

void AEdmundGameState::NotifyPlayerOther(const int32 MaxValue, const int32 CurrentValue)
{
	if (EdmundGameInstance->GetCurrentSceneName() == ESceneType::Main)
	{
		return;
	}

	for (TScriptInterface<IGameStateObserver> Observer : Observers)
	{
		if (!IsValid(Observer.GetObject()))
		{
			continue;
		}
		Observer->ChangedPlayerOther(MaxValue, CurrentValue);
	}
}

void AEdmundGameState::NotifyPlayerAmmo(const int32 MaxAmmo, const int32 CurrentAmmo)
{
	if (EdmundGameInstance->GetCurrentSceneName() == ESceneType::Main)
	{
		return;
	}

	for (TScriptInterface<IGameStateObserver> Observer : Observers)
	{
		if (!IsValid(Observer.GetObject()))
		{
			continue;
		}
		Observer->ChangedPlayerAmmo(MaxAmmo, CurrentAmmo);
	}
}

void AEdmundGameState::NotifyPlayerExp(const int32 MaxExp, const int32 CurrentExp)
{
	if (EdmundGameInstance->GetCurrentSceneName() == ESceneType::Main)
	{
		return;
	}

	for (TScriptInterface<IGameStateObserver> Observer : Observers)
	{
		if (!IsValid(Observer.GetObject()))
		{
			continue;
		}
		Observer->ChangedPlayerExp(MaxExp, CurrentExp);
	}
}

void AEdmundGameState::NotifyPlayerLevel(const int32 LevelValue)
{
	if (EdmundGameInstance->GetCurrentSceneName() == ESceneType::Main)
	{
		return;
	}

	for (TScriptInterface<IGameStateObserver> Observer : Observers)
	{
		if (!IsValid(Observer.GetObject()))
		{
			continue;
		}
		Observer->ChangedPlayerLevel(LevelValue);
	}
}

void AEdmundGameState::NotifyOnStageProgress(const FString& ProgressText, bool bIsOn)
{
	for (TScriptInterface<IGameStateObserver> Observer : Observers)
	{
		if (!IsValid(Observer.GetObject()))
		{
			continue;
		}
		Observer->ChangedStageToProgress(ProgressText, bIsOn);
	}
}

void AEdmundGameState::NotifyUpdateStageProgress(const float Value)
{
	for (TScriptInterface<IGameStateObserver> Observer : Observers)
	{
		if (!IsValid(Observer.GetObject()))
		{
			continue;
		}
		Observer->ChangedBossHp(100, Value * 100);
	}
}

void AEdmundGameState::NotifyBossHp(const int32 MaxHp, const int32 CurrentHp)
{
	for (TScriptInterface<IGameStateObserver> Observer : Observers)
	{
		if (!IsValid(Observer.GetObject()))
		{
			continue;
		}
		Observer->ChangedBossHp(MaxHp, CurrentHp);
	}
}

void AEdmundGameState::NotifyOnOverlapedDefenceArea(const bool bIsOverlaped)
{
	NotifyOnStageProgress("For Complete", bIsOverlaped);
}

void AEdmundGameState::NotifyOnMissionInfo()
{
	for (TScriptInterface<IGameStateObserver> Observer : Observers)
	{
		if (!IsValid(Observer.GetObject()))
		{
			continue;
		}
		Observer->ChangedMissionInfoOnOff();
	}
}

void AEdmundGameState::NotifyPrintText(const FString& TargetText)
{
	for (TScriptInterface<IGameStateObserver> Observer : Observers)
	{
		if (!IsValid(Observer.GetObject()))
		{
			continue;
		}
		Observer->ChangedStoryText(TargetText);
	}
}

void AEdmundGameState::NotifyIsGameClear(const bool bIsClear)
{
	for (TScriptInterface<IGameStateObserver> Observer : Observers)
	{
		if (!IsValid(Observer.GetObject()))
		{
			continue;
		}
		Observer->ChangedIsGameClear(bIsClear);
	}
}

APlayerController* AEdmundGameState::GetPlayerController()
{
	return PlayerController;
}

void AEdmundGameState::SetPlayerPawn(AActor* NewPawn)
{
	if (IsValid(NewPawn))
	{
		PlayerPawn = NewPawn;
	}
}

AActor* AEdmundGameState::GetPlayerPawn()
{
	return PlayerPawn;
}

void AEdmundGameState::NotifyCreateRandomSkill() const
{
	for (TScriptInterface<IGameStateObserver> Observer : Observers)
	{
		if (!IsValid(Observer.GetObject()))
		{
			continue;
		}
		Observer->ChangedSkillList();
	}
}

void AEdmundGameState::NotifySelectCharacterType(ECharacterType CharacterType) const
{
	for (TScriptInterface<IGameStateObserver> Observer : Observers)
	{
		if (!IsValid(Observer.GetObject()))
		{
			continue;
		}
		Observer->ChangedCharacterType(CharacterType);
	}
}

void AEdmundGameState::NotifyResultValue(int32 CurrnetMoney, int32 TotalMoney, int32 MissionMoney) const
{
	for (TScriptInterface<IGameStateObserver> Observer : Observers)
	{
		if (!IsValid(Observer.GetObject()))
		{
			continue;
		}
		Observer->ChangedMissionStateToEnd(CurrnetMoney, TotalMoney, MissionMoney);
	}
}
