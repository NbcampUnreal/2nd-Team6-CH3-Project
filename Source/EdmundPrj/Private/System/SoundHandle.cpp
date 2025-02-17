// Fill out your copyright notice in the Description page of Project Settings.


#include "System/SoundHandle.h"

void USoundHandle::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UE_LOG(LogTemp, Warning, TEXT("Init SoundHandle"));
}
