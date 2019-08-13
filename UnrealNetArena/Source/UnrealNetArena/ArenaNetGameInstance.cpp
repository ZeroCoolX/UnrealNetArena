// Fill out your copyright notice in the Description page of Project Settings.

#include "ArenaNetGameInstance.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "PlatformTrigger.h"

UArenaNetGameInstance::UArenaNetGameInstance(const FObjectInitializer &ObjectInitialize)
{
	// find and Store the Blueprint class
	ConstructorHelpers::FClassFinder<UUserWidget> menuBPClass(TEXT("/Game/Arena/UI/MainMenu_WBP"));
	if (!ensure(menuBPClass.Class)) { 
		UE_LOG(LogTemp, Error, TEXT("Failure to find BP class"));
		return; 
	}
	
	//MainMenu->bIsFocusable = true;
	MenuClass = menuBPClass.Class;
}

void UArenaNetGameInstance::Init()
{
	Super::Init();
	UE_LOG(LogTemp, Warning, TEXT("Found BP class %s"), *MenuClass->GetName());
}

void UArenaNetGameInstance::LoadMenu()
{
	if (!ensure(MenuClass)) { return; }
	
	UUserWidget* menu = CreateWidget<UUserWidget>(this, MenuClass);
	if (!ensure(menu)) { return; }
	// Add the widget to the visible viewport
	menu->AddToViewport();

	// Access the player controller for input capture
	APlayerController* pController = GetFirstLocalPlayerController();
	if (!pController) { return; }

	FInputModeUIOnly inputModeData;
	// Assign which widget to use (our main menu)
	inputModeData.SetWidgetToFocus(menu->TakeWidget());
	// Allow the mouse to move outside of the game screen
	inputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	pController->SetInputMode(inputModeData);

	// Make the cursor visible
	pController->bShowMouseCursor = true;
}

void UArenaNetGameInstance::Host() {
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
	UEngine* engine = GetEngine();
	if (!engine) { return; }

	engine->AddOnScreenDebugMessage(0, 5.f, FColor::Green, FString::Printf(TEXT("Joining %s:7777"), *destination));

	APlayerController* pController = GetFirstLocalPlayerController();
	if (!pController) { return; }
	// join a server if one exists
	pController->ClientTravel(FString::Printf(TEXT("%s:7777"), *destination), ETravelType::TRAVEL_Absolute);
}
