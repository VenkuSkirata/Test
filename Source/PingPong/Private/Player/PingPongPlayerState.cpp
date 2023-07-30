
#include "Player/PingPongPlayerState.h"
#include "GAS/PingPongAbilitySystemComponent.h"
#include "GAS/PingPongPlayerAttributeSet.h"

APingPongPlayerState::APingPongPlayerState()
{
    AbilitySystemComponent = CreateDefaultSubobject<UPingPongAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

    AttributeSet = CreateDefaultSubobject<UPingPongPlayerAttributeSet>(TEXT("AttributeSet"));
    NetUpdateFrequency = 100.0f;

    DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
}

UAbilitySystemComponent* APingPongPlayerState::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

UPingPongPlayerAttributeSet* APingPongPlayerState::GetAttributeSet() const
{
    return AttributeSet;
}

void APingPongPlayerState::ShowAbilityConfirmCancelText(bool ShowText)
{
    // HUD
}

float APingPongPlayerState::GetBallsScore() const
{
    return AttributeSet->GetScore();
}

void APingPongPlayerState::BeginPlay()
{
    Super::BeginPlay();

    if (AbilitySystemComponent)
    {
        LivesChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
            AttributeSet->GetScoreAttribute()).AddUObject(this, &APingPongPlayerState::ScoreChanged
            );

        AbilitySystemComponent->RegisterGameplayTagEvent(
            FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun")), EGameplayTagEventType::NewOrRemoved
        ).AddUObject(this, &APingPongPlayerState::StunTagChanged);
    }
}

void APingPongPlayerState::ScoreChanged(const FOnAttributeChangeData& Data)
{
    UE_LOG(LogTemp, Warning, TEXT("APingPongPlayerState::ScoreChanged()"));
}

void APingPongPlayerState::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
    if (NewCount > 0)
    {
        FGameplayTagContainer AbilityTagsToCancel;
        AbilityTagsToCancel.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability")));

        FGameplayTagContainer AbilityTagsToIgnore;
        AbilityTagsToIgnore.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.NotCanceledByStun")));

        AbilitySystemComponent->CancelAbilities(&AbilityTagsToCancel, &AbilityTagsToIgnore);
    }
}
