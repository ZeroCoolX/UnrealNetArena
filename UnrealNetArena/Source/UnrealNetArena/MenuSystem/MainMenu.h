// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "MenuInterface.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class UNREALNETARENA_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()

public:
	UMainMenu(const FObjectInitializer &ObjectInitialize);

	void SetServerList(TArray<FString> serverNames);

	inline void SelectServerIndex(uint32 sIndex) { SelectedServerIndex = sIndex; }

protected:
	virtual bool Initialize();

private:
	TSubclassOf<class UUserWidget> ServerRowClass;

	UPROPERTY(meta = (BindWidget))
	class UButton* HostBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* ServersBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* QuitBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* ConnectBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* BackBtn;

	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* ServerList;

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
	void OpenServersMenu();

	UFUNCTION()
	void BackToMainMenu();	

	UFUNCTION()
	void QuitGame();

	TOptional<uint32> SelectedServerIndex;
};
