#pragma once

#include "CoreMinimal.h"
#include "PingPong/PingPong.h"
#include "Abilities/GameplayAbility.h"
#include "PingPongGameplayAbility.generated.h"

UCLASS()
class PINGPONG_API UPingPongGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EAbilityID AbilityID = EAbilityID::None;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EAbilityID AbilityInputID = EAbilityID::None;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	bool ActivateAbilityOnGranted = false;


public:
	UPingPongGameplayAbility();

	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
};
