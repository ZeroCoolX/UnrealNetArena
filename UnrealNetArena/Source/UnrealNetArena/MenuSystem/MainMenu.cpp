// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/PlayerController.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "ServerRow.h"


UMainMenu::UMainMenu(const FObjectInitializer &ObjectInitialize) {
	// Find the ServerRow blueprint
	ConstructorHelpers::FClassFinder<UUserWidget> serverRowBPClass(TEXT("/Game/Arena/UI/ServerRow_WBP"));
	if (!ensure(serverRowBPClass.Class)) {
		UE_LOG(LogTemp, Error, TEXT("Failure to find ServerRow_WBP"));
		return;
	}
	ServerRowClass = serverRowBPClass.Class;
}

bool UMainMenu::Initialize() {
	if (!Super::Initialize()) { return false; }

	// Bind Buttons
	if (!ensure(HostBtn)) { return false; }
	HostBtn->OnClicked.AddDynamic(this, &UMainMenu::OpenHostMenu);

	if (!ensure(CreateBtn)) { return false; }
	CreateBtn->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	if (!ensure(CancelHostBtn)) { return false; }
	CancelHostBtn->OnClicked.AddDynamic(this, &UMainMenu::BackToMainMenu);

	if (!ensure(ServersBtn)) { return false; }
	ServersBtn->OnClicked.AddDynamic(this, &UMainMenu::OpenServersMenu);

	if (!ensure(BackBtn)) { return false; }
	BackBtn->OnClicked.AddDynamic(this, &UMainMenu::BackToMainMenu);

	if (!ensure(ConnectBtn)) { return false; }
	ConnectBtn->OnClicked.AddDynamic(this, &UMainMenu::ConnectToServer);

	if (!ensure(QuitBtn)) { return false; }
	QuitBtn->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);

	return true;
}

void UMainMenu::HostServer()
{
	if (!MenuInterface) { return; }

	FString serverName = ServerHostName->GetText().ToString();
	MenuInterface->Host(serverName);
	UE_LOG(LogTemp, Warning, TEXT("Hosting Server %s!"), *serverName);
}

void UMainMenu::SetServerList(TArray<FServerData> servers)
{
	UWorld* world = GetWorld();
	if (!ensure(world)) { return; }

	// Always clear to refresh
	ServerList->ClearChildren();

	uint32 i = 0;
	for (const FServerData& srvr : servers) {
		// Create a new widget in the world
		auto row = CreateWidget<UServerRow>(world, ServerRowClass);
		if (!ensure(row)) { return; }

		// TODO: move into either struct or new method
		FString trimmedName = srvr.Name.Left(16);

		FString trimmedHostUsername = srvr.HostUsername;
		int32 charIndex = 0;
		if (srvr.HostUsername.FindChar('-', charIndex)) {
			trimmedHostUsername = srvr.HostUsername.Left(charIndex);
		}

		row->ServerName->SetText(FText::FromString(FString::Printf(TEXT("%s"), *trimmedName)));
		row->HostUsername->SetText(FText::FromString(FString::Printf(TEXT("%s"), *trimmedHostUsername)));
		row->ConnectedPlayerFraction->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), srvr.CurrentPlayers, srvr.MaxPlayerSize)));

		row->Setup(this, i++);

		// Add it to the list 
		ServerList->AddChild(row);
	}
}

void UMainMenu::SelectServerIndex(uint32 sIndex) { 
	SelectedServerIndex = sIndex; 
	UpdateChildren();
}

void UMainMenu::UpdateChildren()
{
	for (int32 rowAt = 0; rowAt < ServerList->GetChildrenCount(); ++rowAt) {
		auto row = Cast<UServerRow>(ServerList->GetChildAt(rowAt));
		// Only continue processing is this was successful
		if (row) {
			// If the row index is the same as the selected server index that means THIS row is selected and only this row
			row->Selected = (SelectedServerIndex.IsSet() && SelectedServerIndex.GetValue() == rowAt);
		}
	}
}

void UMainMenu::ConnectToServer()
{
	if (SelectedServerIndex.IsSet() && MenuInterface) {
		UE_LOG(LogTemp, Warning, TEXT("Selected index is %d"), SelectedServerIndex.GetValue());
		MenuInterface->Join(SelectedServerIndex.GetValue());
	}else{
		UE_LOG(LogTemp, Warning, TEXT("Selected index not set...Don't do anything"));
	}
}

void UMainMenu::OpenServersMenu()
{
	if (!ensure(MenuSwitcher)) { return; }
	if (!ensure(JoinMenu)) { return; }
	MenuSwitcher->SetActiveWidget(JoinMenu);

	// Refresh the servers for anyone going into the server list
	if (ensure(MenuInterface)) {
		MenuInterface->RefreshServerList();
	}
}

void UMainMenu::OpenHostMenu()
{
	if (!ensure(MenuSwitcher)) { return; }
	if (!ensure(HostMenu)) { return; }
	MenuSwitcher->SetActiveWidget(HostMenu);
}

void UMainMenu::BackToMainMenu()
{
	if (!ensure(MenuSwitcher)) { return; }
	if (!ensure(MainMenu)) { return; }
	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::QuitGame()
{
	UWorld* world = GetWorld();
	if (!world) { return; }

	APlayerController* pController = world->GetFirstPlayerController();
	if (!pController) { return; }
	pController->ConsoleCommand("quit");
}
