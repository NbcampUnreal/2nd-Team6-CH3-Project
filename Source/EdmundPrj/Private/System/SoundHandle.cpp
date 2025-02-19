// Fill out your copyright notice in the Description page of Project Settings.


#include "System/SoundHandle.h"
#include "System/EdmundGameInstance.h"

//void USoundHandle::Initialize(FSubsystemCollectionBase& Collection)
//{
//	Super::Initialize(Collection);
//
//	
//}

void USoundHandle::InitSoundHandle(UEdmundGameInstance* NewGameInstance)
{
	EdmundGameInstance = NewGameInstance;
}

void USoundHandle::UpdateBGMVolume(const float VolumeValue)
{
	BGMVolume = VolumeValue;
	//UE_LOG(LogTemp, Warning, TEXT("BGM Volume updated : %f"), BGMVolume);
}

void USoundHandle::UpdateEffectVolume(const float VolumeValue)
{
	EffectVolume = VolumeValue;
	//UE_LOG(LogTemp, Warning, TEXT("Effect Volume updated : %f"), EffectVolume);
}

float USoundHandle::GetBGMVolume() const
{
	return BGMVolume;
}

float USoundHandle::GetEffectVolume() const
{
	return EffectVolume;
}
