// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusConsumableItem.h"

#include "Cerberus/CerberusLogChannels.h"
#include "Cerberus/AbilitySystem/CerberusAbilitySystemComponent.h"
#include "Cerberus/Character/CerberusCharacter.h"
#include "Cerberus/Inventory/CerberusInventoryComponent.h"
#include "Cerberus/Player/CerberusPlayerState.h"


void UCerberusConsumableItem::Use(ACerberusCharacter* Character)
{
	//Super::Use(Character);

	
	if (UCerberusAbilitySystemComponent* CerberusASC = Character->GetCerberusAbilitySystemComponent())
	{

		if (GetOwningActor()->HasAuthority())
		{
			for (TSubclassOf<UGameplayEffect> Effect : Effects)
			{
				if (Effect)
				{
					FGameplayEffectContextHandle EffectContext = CerberusASC->MakeEffectContext();
					EffectContext.AddInstigator(Character->GetCerberusPlayerState(), Character);
					EffectContext.AddSourceObject(Character);

					FGameplayEffectSpecHandle SpecHandle = CerberusASC->MakeOutgoingSpec(Effect, 1, EffectContext);
					if (SpecHandle.IsValid())
					{
						FActiveGameplayEffectHandle GEHandle = CerberusASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
						if (!GEHandle.WasSuccessfullyApplied())
							UE_LOG(LogCerberusAbilitySystem, Error, TEXT("UCerberusFoodItem: Gameplay effect was not sucessfully applied"))
					}	
				}
			}
		}
	}

	OwningInventory->RemoveItem(this);
}
