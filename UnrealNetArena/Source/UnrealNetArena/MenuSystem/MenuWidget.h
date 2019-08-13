// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "MenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALNETARENA_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	inline void SetMenuInterface(IMenuInterface* menuInterface) { this->MenuInterface = menuInterface; }
	void Setup();
	void Teardown();

protected:
	IMenuInterface* MenuInterface;
};
