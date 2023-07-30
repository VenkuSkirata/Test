#include "Player/PingPongPlayerPawn.h"

#include "GAS/PingPongAbilitySystemComponent.h"
#include "GAS/PingPongPlayerAttributeSet.h"
#include "GAS/PingPongGameplayAbility.h"
#include "Player/PingPongPlayerController.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

APingPongPlayerPawn::APingPongPlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	bAlwaysRelevant = true;
	bReplicates = true;
	bReplicateUsingRegisteredSubObjectList = true;
	
	DeathTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
	EffectRemoveOnDeathTag = FGameplayTag::RequestGameplayTag(FName("State.RemoveOnDeath"));
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SphereComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallComponent"));
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->SetRelativeLocation(FVector(90.0f, 0.0f, 50.0f));
	SphereComponent->SetVisibility(false);

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));
	MovementComponent->SetIsReplicated(true);
}

void APingPongPlayerPawn::BeginPlay()
{
	Super::BeginPlay();
}

void APingPongPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APingPongPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(
		FName("Vertical"), 
		Cast<APingPongPlayerController>(GetController()),
		&APingPongPlayerController::MoveVertical
	);

	PlayerInputComponent->BindAction(
		FName("Fire"),
		EInputEvent::IE_Released, 
		Cast<APingPongPlayerController>(GetController()),
		&APingPongPlayerController::FireBall
	);
}

UAbilitySystemComponent* APingPongPlayerPawn::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

void APingPongPlayerPawn::RemovePawnAbilities()
{
	if (GetLocalRole() != ROLE_Authority || AbilitySystemComponent.IsValid() || !AbilitySystemComponent->PawnAbilitiesGiven)
		return;

	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
	for (const FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities())
	{
		if ((Spec.SourceObject == this) && CharacterAbilities.Contains(Spec.Ability->GetClass()))
		{
			AbilitiesToRemove.Add(Spec.Handle);
		}
	}

	for (int32 i = 0; i < AbilitiesToRemove.Num(); ++i)
	{
		AbilitySystemComponent->ClearAbility(AbilitiesToRemove[i]);
	}

	AbilitySystemComponent->PawnAbilitiesGiven = false;
}

uint8 APingPongPlayerPawn::GetAbilityLevel(EAbilityID AbilityID) const
{
	return static_cast<uint8>(AbilityID);
}

void APingPongPlayerPawn::Die()
{
	RemovePawnAbilities();

	OnCharacterDied.Broadcast(this);

	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->CancelAbilities();

		FGameplayTagContainer EffectTagsToRemove;
		EffectTagsToRemove.AddTag(EffectRemoveOnDeathTag);
		int32 NumEffectsRemoved = AbilitySystemComponent->RemoveActiveEffectsWithTags(EffectTagsToRemove);
		AbilitySystemComponent->AddLooseGameplayTag(DeathTag);
	}

	FinishDying();
}

void APingPongPlayerPawn::FinishDying()
{
	Destroy();
}

float APingPongPlayerPawn::GetScore() const
{
	float result = -1.0f;

	if (AttributeSet.IsValid())
	{
		result = AttributeSet->GetScore();
	}

	return result;
}

void APingPongPlayerPawn::SetScore(float NewScore)
{
	if (AttributeSet.IsValid())
	{
		AttributeSet->SetScore(NewScore);
	}
}

void APingPongPlayerPawn::LoadSphere_Implementation()
{
	SphereComponent->SetVisibility(true);
}

void APingPongPlayerPawn::UnloadSphere_Implementation()
{
	SphereComponent->SetVisibility(false);
}

FVector APingPongPlayerPawn::GetBallSpawnLocation()
{
	return SphereComponent->GetComponentLocation();
}

void APingPongPlayerPawn::AddPawnAbilities()
{
	if (GetLocalRole() != ROLE_Authority || AbilitySystemComponent.IsValid() || !AbilitySystemComponent->PawnAbilitiesGiven)
		return;

	for (TSubclassOf<UPingPongGameplayAbility>& StartupAbility : CharacterAbilities)
	{
		AbilitySystemComponent->GiveAbility(
			FGameplayAbilitySpec(StartupAbility,
				GetAbilityLevel(StartupAbility.GetDefaultObject()->AbilityID),
				static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID),
				this
			)
		);
	}

	AbilitySystemComponent->PawnAbilitiesGiven = true;
}

void APingPongPlayerPawn::InitializeAttributes()
{
	if (!AbilitySystemComponent.IsValid())
	{
		return;
	}

	if (!DefaultAttributes)
	{
		UE_LOG(LogTemp, Error, TEXT("%s is missing DefaultAttributes. Please fill the character's blueprint."), *FString(__FUNCTION__), *GetName());
		return;
	}

	FGameplayEffectContextHandle NewContext = AbilitySystemComponent->MakeEffectContext();
	NewContext.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(
		DefaultAttributes, GetScore(), NewContext
	);

	if (NewHandle.IsValid())
	{
		AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
	}
}

void APingPongPlayerPawn::AddStartupEffects()
{
	if (GetLocalRole() != ROLE_Authority
		|| !AbilitySystemComponent.IsValid()
		|| AbilitySystemComponent->StartupEffectsApplied)
		return;

	FGameplayEffectContextHandle NewContext = AbilitySystemComponent->MakeEffectContext();
	NewContext.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect> GameplayEffect : StartupEffects)
	{
		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(
			GameplayEffect, GetScore(), NewContext
		);

		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
		}
	}

	AbilitySystemComponent->StartupEffectsApplied = true;
}

void APingPongPlayerPawn::BIndASCInput()
{
}

