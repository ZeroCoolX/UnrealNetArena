// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "MenuInterface.h"
#include "MainMenu.generated.h"

USTRUCT()
struct FServerData {
	GENERATED_BODY()

	FString Name;
	uint16 CurrentPlayers;
	uint16 MaxPlayerSize;
	FString HostUsername;
};

/**
 * 
 */
UCLASS()
class UNREALNETARENA_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()

public:
	UMainMenu(const FObjectInitializer &ObjectInitialize);

	void SetServerList(TArray<FServerData> servers);

	void SelectServerIndex(uint32 sIndex);

protected:
	virtual bool Initialize();

private:
	TSubclassOf<class UUserWidget> ServerRowClass;

	UPROPERTY(meta = (BindWidget))
	class UButton* HostBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* ServersBtn;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* ServerHostName;

	UPROPERTY(meta = (BindWidget))
	class UButton* CancelHostBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* CreateBtn;

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

	UPROPERTY(meta = (BindWidget))
	class UWidget* HostMenu;

	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void ConnectToServer();

	UFUNCTION()
	void OpenServersMenu();

	UFUNCTION()
	void OpenHostMenu();

	UFUNCTION()
	void BackToMainMenu();	

	UFUNCTION()
	void QuitGame();

	TOptional<uint32> SelectedServerIndex;

	void UpdateChildren();
};
