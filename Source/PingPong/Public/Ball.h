#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ball.generated.h"

UCLASS()
class PINGPONG_API ABall : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ping Pong")
	class USphereComponent*	CollisionSphere;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ping Pong")
	class UStaticMeshComponent*		MeshComponent;

	FVector BallVelocity;


public:
	ABall();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Ping Pong")
	FORCEINLINE FVector const&		GetBallVelocity() const { return BallVelocity; }
	UFUNCTION(BlueprintCallable, Category = "Ping Pong")
	FORCEINLINE void				SetBallVelocity(FVector const& NewVelocity) { BallVelocity = NewVelocity; }
};
