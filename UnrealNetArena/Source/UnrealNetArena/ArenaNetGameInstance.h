// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionInterface.h"
#include "MenuSystem/MenuInterface.h"
#include "ArenaNetGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class UNREALNETARENA_API UArenaNetGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()
	
public:
	UArenaNetGameInstance(const FObjectInitializer &ObjectInitializer);

	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
	void LoadMenuWidget();

	UFUNCTION(BlueprintCallable)
	void InGameLoadMenu();

	void StartSession();

	UFUNCTION(Exec)
	virtual void Host(FString serverName) override;

	UFUNCTION(Exec)
	virtual void Join(uint32 serverIndex) override;

	virtual void LoadMainMenu() override;

	virtual void RefreshServerList() override;

private:
	TSubclassOf<class UUserWidget> MenuClass;
	class UMainMenu* Menu;

	TSubclassOf<class UUserWidget> InGameMenuClass;

	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	void OnCreateSessionComplete(FName sessionName, bool success);
	void OnDestroySessionComplete(FName sessionName, bool success);
	void OnFindSessionsComplete(bool success);
	void OnJoinSessionComplete(FName sessionName, EOnJoinSessionCompleteResult::Type joinResult);

	FString DesiredServerName;

	void CreateSession();
};
