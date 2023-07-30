#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PingPongPlayerController.generated.h"

UCLASS()
class PINGPONG_API APingPongPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	APingPongPlayerController();

	virtual void OnPossess(APawn* NewPawn);

	UFUNCTION(BlueprintCallable, Category = "Ping Pong")
	void MoveVertical(float Scale);
	UFUNCTION(BlueprintCallable, Category = "Ping Pong")
	void FireBall();
};
