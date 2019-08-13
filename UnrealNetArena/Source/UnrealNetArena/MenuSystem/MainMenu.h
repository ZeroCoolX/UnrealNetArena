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

	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void JoinServer();
	
	IMenuInterface* MenuInterface;
};