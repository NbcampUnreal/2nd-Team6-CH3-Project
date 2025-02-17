// Fill out your copyright notice in the Description page of Project Settings.


#include "System/DataHandle.h"

void UDataHandle::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UE_LOG(LogTemp, Warning, TEXT("Init DataHandle"));
}
