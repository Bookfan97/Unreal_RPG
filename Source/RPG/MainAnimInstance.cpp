// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAnimInstance.h"

#include "GameFramework/PawnMovementComponent.h"
#include "PlayerCharacter.h"

void UMainAnimInstance::NativeInitializeAnimation()
{
	//Check if null and set pawn
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
		if (Pawn)
		{
			PlayerCharacter = Cast<APlayerCharacter>(Pawn);
		}
	}
}

void UMainAnimInstance::UpdateAnimationProperties()
{
	//Check valid pawn
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
	}

	if (Pawn)
	{
		//Set character speed and update MovementSpeed
		FVector Speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);
		MovementSpeed = LateralSpeed.Size();

		//Update whether the player is in the air
		bIsInAir = Pawn->GetMovementComponent()->IsFalling();

		if (PlayerCharacter == nullptr)
		{
			PlayerCharacter = Cast<APlayerCharacter>(Pawn);
		}
	}
}
