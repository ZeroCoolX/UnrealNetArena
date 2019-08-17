// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"

ALobbyGameMode::ALobbyGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonBP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL) {
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	++NumberOfPlayers;
	if (NumberOfPlayers >= 3) {
		UWorld* world = GetWorld();
		if (!world) { return; }
	
		// Allows the transition map to be used
		bUseSeamlessTravel = true;

		// Boot up a server with the main level, and join it
		world->ServerTravel("/Game/ThirdPersonBP/Maps/ThirdPersonExampleMap?listen");

		UE_LOG(LogTemp, Warning, TEXT("Reach 3 players!"));
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	--NumberOfPlayers;
}
