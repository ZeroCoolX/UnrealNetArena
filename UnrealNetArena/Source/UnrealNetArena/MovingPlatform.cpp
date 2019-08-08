// Fill out your copyright notice in the Description page of Project Settings.

#include "MovingPlatform.h"


AMovingPlatform::AMovingPlatform() {
	PrimaryActorTick.bCanEverTick = true;
	// Allow movement
	SetMobility(EComponentMobility::Movable);

	Speed = 5.f;
}

void AMovingPlatform::BeginPlay() {
	Super::BeginPlay();

	// Allowing this object to be replicated
	if (Role == ROLE_Authority) {
		SetReplicates(true);
		SetReplicateMovement(true);
	}
}

void AMovingPlatform::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	
	if (Role == ROLE_Authority) {															// HasAuthority() does the same thing
		FVector location = GetActorLocation();
		FVector globalTargetLocation = GetTransform().TransformPosition(TargetLocation);	// from local to global space
		FVector direction = (globalTargetLocation - location).GetSafeNormal();

		location += direction * Speed * DeltaTime;
		SetActorLocation(location);
	}
}

