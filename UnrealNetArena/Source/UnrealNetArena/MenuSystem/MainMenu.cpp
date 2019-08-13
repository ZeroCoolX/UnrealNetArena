// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "Components/Button.h"
#include "GameFramework/PlayerController.h"

bool UMainMenu::Initialize() {
	if (!Super::Initialize()) { return false; }

	if (!ensure(HostBtn)) { return false; }
	HostBtn->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	if (!ensure(JoinBtn)) { return false; }
	JoinBtn->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

	return true;
}

void UMainMenu::Setup()
{
	this->AddToViewport();

	UWorld* world = GetWorld();
	if (!world) { return; }

	APlayerController* pController = world->GetFirstPlayerController();
	if (!pController) { return; }

	FInputModeUIOnly inputModeData;
	inputModeData.SetWidgetToFocus(this->TakeWidget());
	inputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);		// Allow the mouse to move outside of the game screen

	pController->SetInputMode(inputModeData);
	pController->bShowMouseCursor = true;
}

void UMainMenu::Teardown()
{
	this->RemoveFromViewport();

	UWorld* world = GetWorld();
	if (!world) { return; }

	APlayerController* pController = world->GetFirstPlayerController();
	if (!pController) { return; }

	FInputModeGameOnly inputModeData;	
	pController->SetInputMode(inputModeData);

	pController->bShowMouseCursor = false;
}

void UMainMenu::HostServer()
{
	if (!MenuInterface) { return; }

	MenuInterface->Host();
	UE_LOG(LogTemp, Warning, TEXT("Hosting Server!"));
}

void UMainMenu::JoinServer()
{
	UE_LOG(LogTemp, Warning, TEXT("Joining Server!"));
}
