#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Pawn.h"

#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"

#include "PingPongPlayerPawn.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPawnDiedDelegate, APingPongPlayerPawn*, Pawn);

UCLASS()
class PINGPONG_API APingPongPlayerPawn : public APawn, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Ping Pong")
	FPawnDiedDelegate OnCharacterDied;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ping Pong")
	class UStaticMeshComponent*		SphereComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ping Pong")
	class UFloatingPawnMovement*	MovementComponent;


protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ping Pong")
	TSubclassOf<class UGameplayEffect> DefaultAttributes;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ping Pong")
	TArray<TSubclassOf<class UGameplayEffect> > StartupEffects;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ping Pong")
	TArray<TSubclassOf<class UPingPongGameplayAbility> > CharacterAbilities;

	TWeakObjectPtr<class UPingPongAbilitySystemComponent> AbilitySystemComponent;
	TWeakObjectPtr<class UPingPongPlayerAttributeSet> AttributeSet;

	FGameplayTag DeathTag;
	FGameplayTag EffectRemoveOnDeathTag;
	

public:
	APingPongPlayerPawn();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void RemovePawnAbilities();

	
	UFUNCTION(BlueprintCallable, Category = "Ping Pong")
	virtual uint8 GetAbilityLevel(  EAbilityID AbilityID ) const;

	UFUNCTION(BlueprintCallable, Category = "Ping Pong")
	virtual void Die();
	UFUNCTION(BlueprintCallable, Category = "Ping Pong")
	virtual void FinishDying();

	UFUNCTION(BlueprintCallable, Category = "Ping Pong")
	float GetScore() const;
	UFUNCTION(BlueprintCallable, Category = "Ping Pong")
	virtual void SetScore(float NewScore);

	
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "Ping Pong")
	void LoadSphere();
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "Ping Pong")
	void UnloadSphere();
		
	UFUNCTION(BlueprintCallable, Category = "Ping Pong")
	virtual FVector GetBallSpawnLocation();


protected:
	virtual void AddPawnAbilities();
	virtual void InitializeAttributes();
	virtual void AddStartupEffects();

	virtual void BIndASCInput();
};
