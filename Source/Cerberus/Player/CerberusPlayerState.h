// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "ModularPlayerState.h"
#include "CerberusPlayerState.generated.h"

class UCerberusAbilitySystemComponent;
class UAbilitySystemComponent;

/**
 * ACerberusPlayerState
 *
 * Base player state class used by the project.
 * The AbilitySystemComponent for the player character is initialized here.
 */
UCLASS(Config = Game)
class CERBERUS_API ACerberusPlayerState : public AModularPlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ACerberusPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category="Cerberus|PlayerState")
	UCerberusAbilitySystemComponent* GetCerberusAbilitySystemComponent() const { return AbilitySystemComponent; }
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	//~AActor interface
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
	//~End of AActor interface
	
protected:
	// UFUNCTION()
	// void OnRep_PawnData();

	/**
	* @brief Fortnite goes a few steps further with its optimizations. It actually 
			does not replicate the UAbilitySystemComponent at all for simulated 
			proxies. The component and attribute subobjects are skipped inside 
			::ReplicateSubobjects() on the owning fortnite player state class. We 
			do push the bare minimum replicated data from the ability system 
			component to a structure on the pawn itself (basically, a subset of 
			attribute values and a white list subset of tags that we replicate down 
			in a bitmask). We call this a “proxy”. On the receiving side we take the 
			proxy data, replicated on the pawn, and push it back into ability system 
			component on the player state. So you do have an ASC for each player in 
			FNBR, it just doesn’t directly replicate: instead it replicates data via
			a minimal proxy struct on the pawn and then routes back to the ASC on 
			receiving side. This is advantage since its A) a more minimal set of data 
			B) takes advantage of pawn relevancy.

			- Dave Ratti

			See here for an implementation - https://vorixo.github.io/devtricks/gas-replication-proxy/#21-define-the-replication-proxy-struct
	 * @param Channel 
	 * @param Bunch 
	 * @param RepFlags 
	 * @return 
	 */
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

private:
	UPROPERTY(VisibleAnywhere, Category= "Cerberus|PlayerState")
	UCerberusAbilitySystemComponent* AbilitySystemComponent;
	
};
