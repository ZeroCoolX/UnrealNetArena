// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "GameFramework/PlayerController.h"
#include "Components/EditableTextBox.h"

bool UMainMenu::Initialize() {
	if (!Super::Initialize()) { return false; }

	// Bind Buttons
	if (!ensure(HostBtn)) { return false; }
	HostBtn->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	if (!ensure(JoinBtn)) { return false; }
	JoinBtn->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

	if (!ensure(BackBtn)) { return false; }
	BackBtn->OnClicked.AddDynamic(this, &UMainMenu::BackToMainMenu);

	if (!ensure(ConnectBtn)) { return false; }
	ConnectBtn->OnClicked.AddDynamic(this, &UMainMenu::ConnectToServer);

	return true;
}

void UMainMenu::HostServer()
{
	if (!MenuInterface) { return; }

	MenuInterface->Host();
	UE_LOG(LogTemp, Warning, TEXT("Hosting Server!"));
}

void UMainMenu::ConnectToServer()
{
	if (!MenuInterface) { return; }
	if (!IpInputBox) { return; }

	const FString& destination {IpInputBox->GetText().ToString()};
	MenuInterface->Join(destination);
	UE_LOG(LogTemp, Warning, TEXT("Connecting to Server at %s"), *destination);
}

void UMainMenu::OpenJoinMenu()
{
	if (!ensure(MenuSwitcher)) { return; }else { UE_LOG(LogTemp, Warning, TEXT("MenuSwitcher is nullptr!")); }
	if (!ensure(JoinMenu)) { return; }else{ UE_LOG(LogTemp, Warning, TEXT("JoinMenu is nullptr!")); }
	MenuSwitcher->SetActiveWidget(JoinMenu);
}

void UMainMenu::BackToMainMenu()
{
	if (!ensure(MenuSwitcher)) { return; }
	if (!ensure(MainMenu)) { return; }
	MenuSwitcher->SetActiveWidget(MainMenu);
}
