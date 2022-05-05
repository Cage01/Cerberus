#pragma once

#include "GameplayEffectTypes.h"
#include "CerberusGameplayEffectContext.generated.h"


class UAbilitySystemComponent;
class ICerberusAbilitySourceInterface;
class UPhysicalMaterial;

USTRUCT()
struct FCerberusGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

	FCerberusGameplayEffectContext() : FGameplayEffectContext() { }
	FCerberusGameplayEffectContext(AActor* InInstigator, AActor* InEffectCauser) : FGameplayEffectContext() { }

	/** Returns the wrapped FLyraGameplayEffectContext from the handle, or nullptr if it doesn't exist or is the wrong type */
	static CERBERUS_API FCerberusGameplayEffectContext* ExtractEffectContext(struct FGameplayEffectContextHandle Handle);

	/** Sets the object used as the ability source */
	void SetAbilitySource(const ICerberusAbilitySourceInterface* InObject, float InSourceLevel);

	/** Returns the ability source interface associated with the source object. Only valid on the authority */
	const ICerberusAbilitySourceInterface* GetAbilitySource() const;

	virtual FGameplayEffectContext* Duplicate() const override
	{
		FCerberusGameplayEffectContext* NewContext = new FCerberusGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	/** Overridden to serialize new fields */
	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override;

	/** Returns the physical material from the hit result if there is one */
	const UPhysicalMaterial* GetPhysicalMaterial() const;

public:
	/** ID to allow the identification of multiple bullets that were a part of the same cartridge */
	UPROPERTY()
	int32 CartridgeID = -1;

protected:
	/** Ability Source object (should implement ILyraAbilitySourceInterface). NOT replicated currently */
	UPROPERTY()
	TWeakObjectPtr<const UObject> AbilitySourceObject;
};

template<>
struct TStructOpsTypeTraits<FCerberusGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FCerberusGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};