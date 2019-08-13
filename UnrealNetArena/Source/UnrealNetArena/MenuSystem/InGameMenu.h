// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "InGameMenu.generated.h"

/**
 * 
 */
UCLASS()
class UNREALNETARENA_API UInGameMenu : public UMenuWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UButton* BackBtn;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* LeaveBtn;

protected:
	virtual bool Initialize();

private:
	UFUNCTION()
	void BackToGame();

	UFUNCTION()
	void BackToMainMenu();
};
