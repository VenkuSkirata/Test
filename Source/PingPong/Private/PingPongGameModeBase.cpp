#include "PingPongGameModeBase.h"

#include "Player/PingPongPlayerPawn.h"
#include "Player/PingPongPlayerState.h"
#include "Player/PingPongPlayerController.h"

#include "EngineUtils.h"
#include "Components/SphereComponent.h"


APingPongGameModeBase::APingPongGameModeBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefaultPawnClass = nullptr;
	PlayerStateClass = APingPongPlayerState::StaticClass();
	PlayerControllerClass = APingPongPlayerController::StaticClass();
}

void APingPongGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	TActorIterator<APingPongPlayerPawn> PawnsIterator(GetWorld());
	if (GetNumPlayers() == 2 && PawnsIterator)
	{
		APingPongPlayerPawn* pawn = (*PawnsIterator);
		if (pawn)
		{
			pawn->LoadSphere();
		}
	}
}
