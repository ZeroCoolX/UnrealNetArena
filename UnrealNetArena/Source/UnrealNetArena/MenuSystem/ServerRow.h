// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerRow.generated.h"

/**
 * 
 */
UCLASS()
class UNREALNETARENA_API UServerRow : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ServerName;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ConnectedPlayerFraction;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HostUsername;

	UPROPERTY(BlueprintReadOnly)
	bool Selected = false;

	void Setup(class UMainMenu* parent, uint32 index);

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* RowButton;

	UPROPERTY()
	class UMainMenu* Parent;

	uint32 RowIndex;

	UFUNCTION()
	void OnClicked();
};
