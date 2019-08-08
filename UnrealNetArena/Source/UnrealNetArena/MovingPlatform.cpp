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

	GlobalStartLocation = GetActorLocation();
	GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);

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
		float travelLength = (GlobalTargetLocation - GlobalStartLocation).Size();
		float lengthTravelled = (location - GlobalStartLocation).Size();

		// Simulates movement back and forth
		if (lengthTravelled >= travelLength) {
			VectorSwap(GlobalStartLocation, GlobalTargetLocation);
		}

		FVector direction = (GlobalTargetLocation - GlobalStartLocation).GetSafeNormal();
		FVector newLocation = (GetActorLocation() + direction * Speed * DeltaTime);
		SetActorLocation(newLocation);
	}
}

void AMovingPlatform::VectorSwap(FVector &v1, FVector &v2)
{
	FVector vTemp = v1;
	v1 = v2;
	v2 = vTemp;
}

