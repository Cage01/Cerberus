// Fill out your copyright notice in the Description page of Project Settings.


#include "CerberusFoodItem.h"

#include "Cerberus/CerberusGameplayTags.h"
#include "Cerberus/AbilitySystem/CerberusAbilitySystemComponent.h"
#include "Cerberus/Character/CerberusCharacter.h"
#include "Cerberus/System/CerberusAssetManager.h"
#include "Cerberus/System/CerberusGameData.h"

void UCerberusFoodItem::Use(ACerberusCharacter* Character)
{
	Super::Use(Character);

	if (Character)
	{
		UCerberusAbilitySystemComponent* CerberusASC = Character->GetCerberusAbilitySystemComponent();
		check(CerberusASC)

		TSubclassOf<UGameplayEffect> HealGE = UCerberusAssetManager::GetSubclass(UCerberusGameData::Get().HealGameplayEffect_SetByCaller);
		FGameplayEffectSpecHandle SpecHandle = CerberusASC->MakeOutgoingSpec(HealGE, 1.0f, CerberusASC->MakeEffectContext());

		float HealAmount = 10.0f;
		if (SpecHandle.IsValid())
		{
			SpecHandle.Data->SetSetByCallerMagnitude(FCerberusGameplayTags::Get().SetByCaller_Heal, HealAmount);
			CerberusASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}		
	}
	
	
}
