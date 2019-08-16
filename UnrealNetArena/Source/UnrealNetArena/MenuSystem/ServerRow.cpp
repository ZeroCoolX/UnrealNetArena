// Fill out your copyright notice in the Description page of Project Settings.

#include "ServerRow.h"
#include "MainMenu.h"
#include "Components/Button.h"

void UServerRow::Setup(class UMainMenu* parent, uint32 index)
{
	Parent = parent;
	RowIndex = index;
	RowButton->OnClicked.AddDynamic(this, &UServerRow::OnClicked);
}

void UServerRow::OnClicked()
{
	Parent->SelectServerIndex(RowIndex);
}
