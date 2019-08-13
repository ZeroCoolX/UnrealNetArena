// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ArenaNetGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class UNREALNETARENA_API UArenaNetGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UArenaNetGameInstance(const FObjectInitializer &ObjectInitializer);

	virtual void Init() override;

	UFUNCTION(Exec)
	void Host();

	UFUNCTION(Exec)
	void Join(const FString& destination);

private:
	TSubclassOf<class UUserWidget> MenuClass;

};
