// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/PingPongPlayerAttributeSet.h"
#include "Net/UnrealNetwork.h"

void UPingPongPlayerAttributeSet::OnRep_Score(const FGameplayAttributeData& OldScore)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPingPongPlayerAttributeSet, Score, OldScore);
}

void UPingPongPlayerAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UPingPongPlayerAttributeSet, Score, COND_None, REPNOTIFY_Always);
}
