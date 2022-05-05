#include "CerberusGameplayEffectContext.h"

#include "CerberusAbilitySourceInterface.h"

FCerberusGameplayEffectContext* FCerberusGameplayEffectContext::ExtractEffectContext(FGameplayEffectContextHandle Handle)
{
	FGameplayEffectContext* BaseEffectContext = Handle.Get();
	if ((BaseEffectContext != nullptr) && BaseEffectContext->GetScriptStruct()->IsChildOf(FCerberusGameplayEffectContext::StaticStruct()))
	{
		return (FCerberusGameplayEffectContext*)BaseEffectContext;
	}

	return nullptr;
}

void FCerberusGameplayEffectContext::SetAbilitySource(const ICerberusAbilitySourceInterface* InObject, float InSourceLevel)
{
	AbilitySourceObject = MakeWeakObjectPtr(Cast<const UObject>(InObject));
	//SourceLevel = InSourceLevel;
}

const ICerberusAbilitySourceInterface* FCerberusGameplayEffectContext::GetAbilitySource() const
{
	return Cast<ICerberusAbilitySourceInterface>(AbilitySourceObject.Get());
}

bool FCerberusGameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	FGameplayEffectContext::NetSerialize(Ar, Map, bOutSuccess);

	// Not serialized for post-activation use:
	// CartridgeID
	
	return true;
}

const UPhysicalMaterial* FCerberusGameplayEffectContext::GetPhysicalMaterial() const
{
	if (const FHitResult* HitResultPrt = GetHitResult())
	{
		return HitResultPrt->PhysMaterial.Get();
	}

	return nullptr;
}
