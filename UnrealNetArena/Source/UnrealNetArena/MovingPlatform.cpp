// Fill out your copyright notice in the Description page of Project Settings.

#include "MovingPlatform.h"


AMovingPlatform::AMovingPlatform() {
	PrimaryActorTick.bCanEverTick = true;
	// Allow movement
	SetMobility(EComponentMobility::Movable);

	Speed = 5.f;
}

void AMovingPlatform::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	
	if (Role == ROLE_Authority) {	// HasAuthority() does the same thing
		FVector location = GetActorLocation();
		location += FVector(Speed * DeltaTime, 0.f, 0.f);
		SetActorLocation(location);
	}
}

