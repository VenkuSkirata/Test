#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "PingPongAbilitySystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FReceivedDamageDelegate, UPingPongAbilitySystemComponent*, SourceASC, float, UnmitigatedDamage, float, MitigatedDamage);

UCLASS()
class PINGPONG_API UPingPongAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	bool PawnAbilitiesGiven = false;
	bool StartupEffectsApplied = false;

	FReceivedDamageDelegate ReceivedDamageDelegate;


public:
	virtual void ReceiveDamage(UPingPongAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage);
};
