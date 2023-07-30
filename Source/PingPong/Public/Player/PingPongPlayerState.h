#pragma once

#include "CoreMinimal.h"

#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "GameplayEffectTypes.h"

#include "GameFramework/PlayerState.h"
#include "PingPongPlayerState.generated.h"

UCLASS()
class PINGPONG_API APingPongPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	FGameplayTag DeadTag;
	FDelegateHandle LivesChangedDelegateHandle;


protected:
	UPROPERTY()
	class UPingPongAbilitySystemComponent* AbilitySystemComponent;
	UPROPERTY()
	class UPingPongPlayerAttributeSet* AttributeSet;


public:
	APingPongPlayerState();

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	class UPingPongPlayerAttributeSet* GetAttributeSet() const;

	UFUNCTION(BlueprintCallable, Category = "Ping Pong")
	void ShowAbilityConfirmCancelText(bool ShowText);

	UFUNCTION(BlueprintCallable, Category = "Ping Pong")
	float GetBallsScore() const;

protected:
	virtual void BeginPlay() override;

	virtual void ScoreChanged(const FOnAttributeChangeData& Data);
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
};
