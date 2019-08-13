// Fill out your copyright notice in the Description page of Project Settings.

#include "ArenaNetGameInstance.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem//MainMenu.h"
#include "MenuSystem//MenuWidget.h"
#include "PlatformTrigger.h"

UArenaNetGameInstance::UArenaNetGameInstance(const FObjectInitializer &ObjectInitialize)
{
	// find and Store the Blueprint class
	ConstructorHelpers::FClassFinder<UUserWidget> menuBPClass(TEXT("/Game/Arena/UI/MainMenu_WBP"));
	if (!ensure(menuBPClass.Class)) { 
		UE_LOG(LogTemp, Error, TEXT("Failure to find MainMenu_WBP"));
		return; 
	}
		MenuClass = menuBPClass.Class;

	// find and Store the Blueprint class
	ConstructorHelpers::FClassFinder<UUserWidget> inGameMenuBPClass(TEXT("/Game/Arena/UI/InGameMenu_WBP"));
	if (!ensure(inGameMenuBPClass.Class)) {
		UE_LOG(LogTemp, Error, TEXT("Failure to find InGameMenu_WBP"));
		return;
	}
	InGameMenuClass = inGameMenuBPClass.Class;
}

void UArenaNetGameInstance::Init()
{
	Super::Init();
	UE_LOG(LogTemp, Warning, TEXT("Found BP class %s"), *MenuClass->GetName());
}

void UArenaNetGameInstance::LoadMenu()
{
	if (!ensure(MenuClass)) { return; }
	
	Menu = CreateWidget<UMainMenu>(this, MenuClass);
	if (!ensure(Menu)) { return; }
	Menu->Setup();
	Menu->SetMenuInterface(this);
}

void UArenaNetGameInstance::InGameLoadMenu()
{
	if (!ensure(InGameMenuClass)) { return; }

	UMenuWidget* inGameMenu = CreateWidget<UMenuWidget>(this, InGameMenuClass);
	if (!ensure(inGameMenu)) { return; }
	inGameMenu->Setup();
	inGameMenu->SetMenuInterface(this);
}

void UArenaNetGameInstance::Host() {
	if (Menu != nullptr) {
		Menu->Teardown();
	}

	UEngine* engine = GetEngine();
	if (!engine) { return; }

	// Display to user they're hosting a game
	engine->AddOnScreenDebugMessage(0, 5.f, FColor::Green, TEXT("Hosting Game"));

	UWorld* world = GetWorld();
	if (!world) { return; }
	// Boot up a server with the main level, and join it
	world->ServerTravel("/Game/ThirdPersonBP/Maps/ThirdPersonExampleMap?listen");
}

void UArenaNetGameInstance::Join(const FString& destination)
{
	if (Menu != nullptr) {
		Menu->Teardown();
	}

	UEngine* engine = GetEngine();
	if (!engine) { return; }

	engine->AddOnScreenDebugMessage(0, 5.f, FColor::Green, FString::Printf(TEXT("Joining %s:7777"), *destination));

	APlayerController* pController = GetFirstLocalPlayerController();
	if (!pController) { return; }
	// join a server if one exists
	pController->ClientTravel(FString::Printf(TEXT("%s:7777"), *destination), ETravelType::TRAVEL_Absolute);
}

void UArenaNetGameInstance::LoadMainMenu() {
	APlayerController* pController = GetFirstLocalPlayerController();
	if (!pController) { return; }

	// Take player back to the menu - disconnecting from the server
	pController->ClientTravel("/Game/Arena/UI/MainMenu", ETravelType::TRAVEL_Absolute);
}
