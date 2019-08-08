// Fill out your copyright notice in the Description page of Project Settings.

#include "ArenaNetGameInstance.h"
#include "Engine/Engine.h"

UArenaNetGameInstance::UArenaNetGameInstance(const FObjectInitializer &ObjectInitialize)
{
	UE_LOG(LogTemp, Warning, TEXT("From Constructor"));
}

void UArenaNetGameInstance::Init()
{
	Super::Init();
	UE_LOG(LogTemp, Warning, TEXT("From Init"));
}

void UArenaNetGameInstance::Host() {
	UEngine* engine = GetEngine();
	if (!engine) { return; }

	engine->AddOnScreenDebugMessage(0, 5.f, FColor::Green, TEXT("Hosting Game"));
}

void UArenaNetGameInstance::Join(const FString& destination)
{
	UEngine* engine = GetEngine();
	if (!engine) { return; }

	engine->AddOnScreenDebugMessage(0, 5.f, FColor::Green, FString::Printf(TEXT("Joining %s"), *destination));
}
