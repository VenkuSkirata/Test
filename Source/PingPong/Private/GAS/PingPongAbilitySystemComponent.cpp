#include "GAS/PingPongAbilitySystemComponent.h"


void UPingPongAbilitySystemComponent::ReceiveDamage(UPingPongAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage)
{
	ReceivedDamageDelegate.Broadcast(SourceASC, UnmitigatedDamage, MitigatedDamage);
}
