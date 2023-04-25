// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusConsumableItem.h"

#include "Cerberus/CerberusLogChannels.h"
#include "Cerberus/AbilitySystem/CerberusAbilitySystemComponent.h"
#include "Cerberus/Character/CerberusCharacter.h"
#include "Cerberus/UniversalComponents/CerberusInventoryComponent.h"
#include "Cerberus/Player/CerberusPlayerState.h"


void UCerberusConsumableItem::Use(ACerberusCharacter* Character)
{

	if (Character)
	{
		//Do stuff

		if (UCerberusInventoryComponent* Inventory = Character->InventoryComponent)
		{
			UE_LOG(LogTemp, Warning, TEXT("UCerberusConsumableItem: Item being used"))
			Inventory->ConsumeItem(this, 1);
		}
	}

	
	//Super::Use(Character);

	// bool bConsumed = false;
	// if (UCerberusAbilitySystemComponent* CerberusASC = Character->GetCerberusAbilitySystemComponent())
	// {
	// 	int32 consumed = 0;
	// 	if (GetOwningActor()->HasAuthority())
	// 	{
	// 		for (TSubclassOf<UGameplayEffect> Effect : Effects)
	// 		{
	// 			if (Effect)
	// 			{
	// 				FGameplayEffectContextHandle EffectContext = CerberusASC->MakeEffectContext();
	// 				EffectContext.AddInstigator(Character->GetCerberusPlayerState(), Character);
	// 				EffectContext.AddSourceObject(Character);
	//
	// 				FGameplayEffectSpecHandle SpecHandle = CerberusASC->MakeOutgoingSpec(Effect, 1, EffectContext);
	// 				if (SpecHandle.IsValid())
	// 				{
	// 					FActiveGameplayEffectHandle GEHandle = CerberusASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	// 					if (!GEHandle.WasSuccessfullyApplied())
	// 						UE_LOG(LogCerberusAbilitySystem, Error, TEXT("UCerberusConsumableItem: Gameplay effect was not sucessfully applied"))
	// 					else
	// 					{
	// 						bConsumed = true;
	// 						UE_LOG(LogCerberusAbilitySystem, Warning, TEXT("%d consumables consumed"), consumed);
	// 					}
	//
	// 				}	
	// 			}
	// 		}
	// 	}
	// }
}
