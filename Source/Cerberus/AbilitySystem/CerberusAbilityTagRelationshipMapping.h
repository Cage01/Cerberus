#pragma once
#include "GameplayTagContainer.h"
#include "UObject/ObjectMacros.h"
#include "CerberusAbilityTagRelationshipMapping.generated.h"

/** Struct that defines the relationship between different ability tags */
USTRUCT()
struct FCerberusAbilityTagRelationship
{
	GENERATED_BODY()

	/** The tag that this container relationship is about. Single tag, but abilities can have multiple of these */
	UPROPERTY(EditAnywhere, Category = Ability, meta=(Categories = "Gameplay.Action"))
	FGameplayTag AbilityTag;

	/** The other ability tags that will be blocked by any ability using this tag */
	UPROPERTY(EditAnywhere, Category = Ability)
	FGameplayTagContainer AbilityTagsToBlock;

	/** The other ability tags that will be canceled by any ability using this tag */
	UPROPERTY(EditAnywhere, Category = Ability)
	FGameplayTagContainer AbilityTagsToCancel;

	/** If an ability has the tag, this is implicitly added to the activation required tags of the ability */
	UPROPERTY(EditAnywhere, Category = Ability)
	FGameplayTagContainer ActivationRequiredTags;

	/** If an ability has the tag, this is implicitly added to the activation blocked tags of the ability */
	UPROPERTY(EditAnywhere, Category = Ability)
	FGameplayTagContainer ActivationBlockedTags;
};

/** Mapping of how ability tags block or cancel other abilities */
UCLASS()
class UCerberusAbilityTagRelationshipMapping : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UCerberusAbilityTagRelationshipMapping();
	
};

