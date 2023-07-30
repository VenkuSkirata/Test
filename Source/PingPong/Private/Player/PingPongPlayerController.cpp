// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PingPongPlayerController.h"
#include "Player/PingPongPlayerState.h"
#include "Player/PingPongPlayerPawn.h"

#include "GAS/PingPongAbilitySystemComponent.h"

#include "Ball.h"

#include "Kismet/KismetMathLibrary.h"
#include "Components/SphereComponent.h"

APingPongPlayerController::APingPongPlayerController()
{
	bAutoManageActiveCameraTarget = false;
	bReplicates = true;
	bReplicateUsingRegisteredSubObjectList = true;
}

void APingPongPlayerController::OnPossess(APawn* NewPawn)
{
	Super::OnPossess(NewPawn);

	APingPongPlayerState* PS = GetPlayerState<APingPongPlayerState>();
	if (PS)
	{
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, NewPawn);
	}
}

void APingPongPlayerController::MoveVertical(float Scale)
{
	auto pawn = Cast<APingPongPlayerPawn>(GetPawn());
	if (pawn)
	{
		pawn->AddMovementInput(UKismetMathLibrary::GetRightVector(FRotator(0.0f, 1.0f, 0.0f)), Scale);
	}
}

void APingPongPlayerController::FireBall()
{
	auto pawn = Cast<APingPongPlayerPawn>(GetPawn());
	if (pawn)
	{
		FVector BallSpawnLocation = pawn->GetBallSpawnLocation();
		FVector BallNewVelocity = UKismetMathLibrary::GetForwardVector(
			FRotator(0.0f, pawn->GetActorRotation().Yaw, 0.0f)) * 10.0f;
		
		pawn->UnloadSphere();
		
		auto FiredBall = GWorld->SpawnActor<ABall>(BallSpawnLocation, FRotator());
		if (FiredBall)
		{
			FiredBall->SetBallVelocity(BallNewVelocity);
		}
	}
}
