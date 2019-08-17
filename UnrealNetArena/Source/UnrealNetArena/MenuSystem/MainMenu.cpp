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
	HostBtn->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

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

	MenuInterface->Host();
	UE_LOG(LogTemp, Warning, TEXT("Hosting Server!"));
}

void UMainMenu::SetServerList(TArray<FString> serverNames)
{
	UWorld* world = GetWorld();
	if (!ensure(world)) { return; }

	// Always clear to refresh
	ServerList->ClearChildren();

	uint32 i = 0;
	for (const FString& name : serverNames) {
		// Create a new widget in the world
		auto row = CreateWidget<UServerRow>(world, ServerRowClass);
		if (!ensure(row)) { return; }

		row->ServerName->SetText(FText::FromString(name));
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
	if (!ensure(MenuSwitcher)) { return; }else { UE_LOG(LogTemp, Warning, TEXT("MenuSwitcher is nullptr!")); }
	if (!ensure(JoinMenu)) { return; }else{ UE_LOG(LogTemp, Warning, TEXT("JoinMenu is nullptr!")); }
	MenuSwitcher->SetActiveWidget(JoinMenu);

	// Refresh the servers for anyone going into the server list
	if (ensure(MenuInterface)) {
		MenuInterface->RefreshServerList();
	}
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
