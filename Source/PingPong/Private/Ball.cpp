#include "Ball.h"
#include "Components/SphereComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"

ABall::ABall()
{
 	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bReplicateUsingRegisteredSubObjectList = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetupAttachment(RootComponent);
	CollisionSphere->SetRelativeScale3D(FVector(1.f));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallComponent"));
	MeshComponent->SetupAttachment(RootComponent);

	BallVelocity = FVector(0.f);
}

void ABall::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(GetActorLocation() + BallVelocity * DeltaTime);

	FVector const& SweepFrom = GetActorLocation();
	FVector const& SweepTo = GetActorLocation() + FVector(BallVelocity) * DeltaTime;

	FHitResult					SweepResult;
	const TArray<AActor*>		ActorsToIgnore({ this });

	UKismetSystemLibrary::SphereTraceSingle(this, SweepFrom, SweepTo,
		CollisionSphere->GetScaledSphereRadius(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		SweepResult,
		true
	);

	if (SweepResult.bBlockingHit)
	{
		BallVelocity = BallVelocity.MirrorByVector( SweepResult.ImpactNormal );
	}
}