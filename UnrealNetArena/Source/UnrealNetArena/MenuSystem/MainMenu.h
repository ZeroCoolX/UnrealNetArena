// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class UNREALNETARENA_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	inline void SetMenuInterface(IMenuInterface* menuInterface) { this->MenuInterface = menuInterface; }

	void Setup();
	void Teardown();

protected:
	virtual bool Initialize();

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* HostBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* ConnectBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* BackBtn;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* IpInputBox;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* MainMenu;

	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void ConnectToServer();

	UFUNCTION()
	void OpenJoinMenu();

	UFUNCTION()
	void BackToMainMenu();
	
	IMenuInterface* MenuInterface;
};
