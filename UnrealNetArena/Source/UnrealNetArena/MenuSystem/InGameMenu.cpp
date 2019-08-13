// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameMenu.h"
#include "Components/Button.h"

bool UInGameMenu::Initialize() {
	if (!Super::Initialize()) { return false; }

	// Bind Buttons
	if (!ensure(BackBtn)) { return false; }
	BackBtn->OnClicked.AddDynamic(this, &UInGameMenu::BackToGame);

	if (!ensure(LeaveBtn)) { return false; }
	LeaveBtn->OnClicked.AddDynamic(this, &UInGameMenu::BackToMainMenu);

	return true;
}

void UInGameMenu::BackToGame() {
	Teardown();
}

void UInGameMenu::BackToMainMenu() {
	if (!MenuInterface) { return; }

	Teardown();
	MenuInterface->LoadMainMenu();
}

