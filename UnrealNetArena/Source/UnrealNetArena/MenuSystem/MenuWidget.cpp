// Fill out your copyright notice in the Description page of Project Settings.

#include "MenuWidget.h"

void UMenuWidget::Setup()
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

void UMenuWidget::Teardown()
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


