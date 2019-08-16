// Fill out your copyright notice in the Description page of Project Settings.

#include "ArenaNetGameInstance.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/MenuWidget.h"
#include "PlatformTrigger.h"
#include "OnlineSessionSettings.h"

const static FName SESSION_NAME = TEXT("Arena Net Game");

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

	// Setup online session interface for hosting
	IOnlineSubsystem* onlineSub = IOnlineSubsystem::Get();
	if (onlineSub) {
		UE_LOG(LogTemp, Warning, TEXT("Found subsystem %s"), *onlineSub->GetSubsystemName().ToString());

		// Hook up event delegates
		SessionInterface = onlineSub->GetSessionInterface();
		if (SessionInterface.IsValid()) {
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UArenaNetGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UArenaNetGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UArenaNetGameInstance::OnFindSessionsComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UArenaNetGameInstance::OnJoinSessionComplete);
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Unable to find online subsystem"));
	}
}

void UArenaNetGameInstance::LoadMenuWidget()
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
	if (SessionInterface.IsValid()) {
		auto existingSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if (existingSession) {
			// Destroy the existing session because someone wants to create a new one
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else {
			CreateSession();
		}
	}
}

void UArenaNetGameInstance::RefreshServerList()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid()) {
		//SessionSearch->bIsLanQuery = true;
		SessionSearch->MaxSearchResults = 100;	// Default is 1 so probability that is yours is low (because we're sharing the app id)
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

		UE_LOG(LogTemp, Warning, TEXT("Starting session search...."));
		SessionInterface->FindSessions(0,SessionSearch.ToSharedRef()); // used to store what sessions were found
	}
}

void UArenaNetGameInstance::CreateSession()
{
	if (SessionInterface.IsValid()) {
		FOnlineSessionSettings sessionSettings;
		sessionSettings.bIsLANMatch = false;
		sessionSettings.NumPublicConnections = 2;
		sessionSettings.bShouldAdvertise = true;	// Makes it visible to .FindSessions()
		sessionSettings.bUsesPresence = true;		// enables this on the server which tells steam to use lobbies instead of dedicated servers
		SessionInterface->CreateSession(0, SESSION_NAME, sessionSettings);
	}
}

void UArenaNetGameInstance::Join(uint32 serverIndex)
{
	if (!SessionInterface.IsValid() || !SessionSearch.IsValid()) { return; }

	if (Menu != nullptr) {
		Menu->Teardown();
	}

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[serverIndex]);
}

void UArenaNetGameInstance::LoadMainMenu() {
	APlayerController* pController = GetFirstLocalPlayerController();
	if (!pController) { return; }

	// Take player back to the menu - disconnecting from the server
	pController->ClientTravel("/Game/Arena/UI/MainMenu", ETravelType::TRAVEL_Absolute);
}

/* Event delegates */
void UArenaNetGameInstance::OnCreateSessionComplete(FName sessionName, bool success)
{
	// Indicates if the session could be created - if one already exists this will return false
	if (!success) {
		return;
		UE_LOG(LogTemp, Error, TEXT("Unable to create subsystem session"));
	}

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

void UArenaNetGameInstance::OnDestroySessionComplete(FName sessionName, bool success)
{
	// Ensures that on destruction a new one is created
	if (success) {
		CreateSession();
	}
}

void UArenaNetGameInstance::OnFindSessionsComplete(bool success)
{
	if (success && SessionSearch.IsValid() && Menu != nullptr) {
		TArray<FString> serverNames;
		// Collect all the sessions adding them to the list
		for (const FOnlineSessionSearchResult& sRes : SessionSearch->SearchResults) {
			UE_LOG(LogTemp, Warning, TEXT("Found Session [%s], %dms"), *sRes.GetSessionIdStr(), sRes.PingInMs);
			serverNames.Add(*sRes.GetSessionIdStr());
		}
		// Store the list to the Server Menu
		Menu->SetServerList(serverNames);
	}
}

void UArenaNetGameInstance::OnJoinSessionComplete(FName sessionName, EOnJoinSessionCompleteResult::Type joinResult)
{
	if (!SessionInterface.IsValid()) { return; }

	FString destination;
	if (!SessionInterface->GetResolvedConnectString(SESSION_NAME, destination)) {
		UE_LOG(LogTemp, Error, TEXT("Could not get connection string from session"));
		return;
	}

	UEngine* engine = GetEngine();
	if (!engine) { return; }

	engine->AddOnScreenDebugMessage(0, 5.f, FColor::Green, FString::Printf(TEXT("Joining %s:7777"), *destination));

	APlayerController* pController = GetFirstLocalPlayerController();
	if (!pController) { return; }
	// join a server if one exists
	pController->ClientTravel(FString::Printf(TEXT("%s:7777"), *destination), ETravelType::TRAVEL_Absolute);
}

