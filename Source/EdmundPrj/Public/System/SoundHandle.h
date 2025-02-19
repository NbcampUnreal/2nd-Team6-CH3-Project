// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SoundHandle.generated.h"

class UEdmundGameInstance;

UCLASS()
class EDMUNDPRJ_API USoundHandle : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	//virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	void InitSoundHandle(UEdmundGameInstance* NewGameInstance);

	// Update Sound Volume By UI Sound Slider
	void UpdateBGMVolume(const float VolumeValue);
	void UpdateEffectVolume(const float VolumeValue);

	// Sound Volume Getter
	float GetBGMVolume() const;
	float GetEffectVolume() const;

private:
	TObjectPtr<UEdmundGameInstance> EdmundGameInstance = nullptr;
	float BGMVolume = 0.8f;
	float EffectVolume = 0.8f;
};
