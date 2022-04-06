// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "ShooterCoreRuntime/Public/Accolades/LyraAccoladeDefinition.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeLyraAccoladeDefinition() {}
// Cross Module References
	SHOOTERCORERUNTIME_API UScriptStruct* Z_Construct_UScriptStruct_FLyraAccoladeDefinitionRow();
	UPackage* Z_Construct_UPackage__Script_ShooterCoreRuntime();
	ENGINE_API UScriptStruct* Z_Construct_UScriptStruct_FTableRowBase();
	ENGINE_API UClass* Z_Construct_UClass_USoundBase_NoRegister();
	COREUOBJECT_API UClass* Z_Construct_UClass_UObject_NoRegister();
	GAMEPLAYTAGS_API UScriptStruct* Z_Construct_UScriptStruct_FGameplayTag();
	GAMEPLAYTAGS_API UScriptStruct* Z_Construct_UScriptStruct_FGameplayTagContainer();
	SHOOTERCORERUNTIME_API UClass* Z_Construct_UClass_ULyraAccoladeDefinition_NoRegister();
	SHOOTERCORERUNTIME_API UClass* Z_Construct_UClass_ULyraAccoladeDefinition();
	ENGINE_API UClass* Z_Construct_UClass_UDataAsset();
// End Cross Module References

static_assert(std::is_polymorphic<FLyraAccoladeDefinitionRow>() == std::is_polymorphic<FTableRowBase>(), "USTRUCT FLyraAccoladeDefinitionRow cannot be polymorphic unless super FTableRowBase is polymorphic");

	static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_LyraAccoladeDefinitionRow;
class UScriptStruct* FLyraAccoladeDefinitionRow::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_LyraAccoladeDefinitionRow.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_LyraAccoladeDefinitionRow.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FLyraAccoladeDefinitionRow, Z_Construct_UPackage__Script_ShooterCoreRuntime(), TEXT("LyraAccoladeDefinitionRow"));
	}
	return Z_Registration_Info_UScriptStruct_LyraAccoladeDefinitionRow.OuterSingleton;
}
template<> SHOOTERCORERUNTIME_API UScriptStruct* StaticStruct<FLyraAccoladeDefinitionRow>()
{
	return FLyraAccoladeDefinitionRow::StaticStruct();
}
	struct Z_Construct_UScriptStruct_FLyraAccoladeDefinitionRow_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_DisplayName_MetaData[];
#endif
		static const UECodeGen_Private::FTextPropertyParams NewProp_DisplayName;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Sound_MetaData[];
#endif
		static const UECodeGen_Private::FSoftObjectPropertyParams NewProp_Sound;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Icon_MetaData[];
#endif
		static const UECodeGen_Private::FSoftObjectPropertyParams NewProp_Icon;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_DisplayDuration_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_DisplayDuration;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_LocationTag_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_LocationTag;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_AccoladeTags_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_AccoladeTags;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_CancelAccoladesWithTag_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_CancelAccoladesWithTag;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UECodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FLyraAccoladeDefinitionRow_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/Accolades/LyraAccoladeDefinition.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FLyraAccoladeDefinitionRow_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FLyraAccoladeDefinitionRow>();
	}
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FLyraAccoladeDefinitionRow_Statics::NewProp_DisplayName_MetaData[] = {
		{ "Category", "LyraAccoladeDefinitionRow" },
		{ "Comment", "// The message to display\n" },
		{ "ModuleRelativePath", "Public/Accolades/LyraAccoladeDefinition.h" },
		{ "ToolTip", "The message to display" },
	};
#endif
	const UECodeGen_Private::FTextPropertyParams Z_Construct_UScriptStruct_FLyraAccoladeDefinitionRow_Statics::NewProp_DisplayName = { "DisplayName", nullptr, (EPropertyFlags)0x0010000000000015, UECodeGen_Private::EPropertyGenFlags::Text, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FLyraAccoladeDefinitionRow, DisplayName), METADATA_PARAMS(Z_Construct_UScriptStruct_FLyraAccoladeDefinitionRow_Statics::NewProp_DisplayName_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FLyraAccoladeDefinitionRow_Statics::NewProp_DisplayName_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FLyraAccoladeDefinitionRow_Statics::NewProp_Sound_MetaData[] = {
		{ "Category", "LyraAccoladeDefinitionRow" },
		{ "Comment", "// The sound to play\n" },
		{ "ModuleRelativePath", "Public/Accolades/LyraAccoladeDefinition.h" },
		{ "ToolTip", "The sound to play" },
	};
#endif
	const UECodeGen_Private::FSoftObjectPropertyParams Z_Construct_UScriptStruct_FLyraAccoladeDefinitionRow_Statics::NewProp_Sound = { "Sound", nullptr, (EPropertyFlags)0x0014000000000015, UECodeGen_Private::EPropertyGenFlags::SoftObject, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FLyraAccoladeDefinitionRow, Sound), Z_Construct_UClass_USoundBase_NoRegister, METADATA_PARAMS(Z_Construct_UScriptStruct_FLyraAccoladeDefinitionRow_Statics::NewProp_Sound_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FLyraAccoladeDefinitionRow_Statics::NewProp_Sound_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FLyraAccoladeDefinitionRow_Statics::NewProp_Icon_MetaData[] = {
		{ "AllowedClasses", "Texture,MaterialInterface,SlateTextureAtlasInterface" },
		{ "Category", "LyraAccoladeDefinitionRow" },
		{ "Comment", "// The icon to display\x09\n" },
		{ "DisallowedClasses", "MediaTexture" },
		{ "DisplayThumbnail", "true" },
		{ "ModuleRelativePath", "Public/Accolades/LyraAccoladeDefinition.h" },
		{ "ToolTip", "The icon to display" },
	};
#endif
	const UECodeGen_Private::FSoftObjectPropertyParams Z_Construct_UScriptStruct_FLyraAccoladeDefinitionRow_Statics::NewProp_Icon = { "Icon", nullptr, (EPropertyFlags)0x0014000000000015, UECodeGen_Private::EPropertyGenFlags::SoftObject, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FLyraAccoladeDefinitionRow, Icon), Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(Z_Construct_UScriptStruct_FLyraAccoladeDefinitionRow_Statics::NewProp_Icon_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FLyraAccoladeDefinitionRow_Statics::NewProp_Icon_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FLyraAccoladeDefinitionRow_Statics::NewProp_DisplayDuration_MetaData[] = {
		{ "Category", "LyraAccoladeDefinitionRow" },
		{ "Comment", "// Duration (in seconds) to display this accolade\n" },
		{ "ModuleRelativePath", "Public/Accolades/LyraAccoladeDefinition.h" },
		{ "ToolTip", "Duration (in seconds) to display this accolade" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FLyraAccoladeDefinitionRow_Statics::NewProp_DisplayDuration = { "DisplayDuration", nullptr, (EPropertyFlags)0x0010000000000015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FLyraAccoladeDefinitionRow, DisplayDuration), METADATA_PARAMS(Z_Construct_UScriptStruct_FLyraAccoladeDefinitionRow_Statics::NewProp_DisplayDuration_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FLyraAccoladeDefinitionRow_Statics::NewProp_DisplayDuration_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FLyraAccoladeDefinitionRow_Statics::NewProp_LocationTag_MetaData[] = {
		{ "Category", "LyraAccoladeDefinitionRow" },
		{ "Comment", "// Location to display this accolade\n" },
		{ "ModuleRelativePath", "Public/Accolades/LyraAccoladeDefinition.h" },
		{ "ToolTip", "Location to display this accolade" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FLyraAccoladeDefinitionRow_Statics::NewProp_LocationTag = { "LocationTag", nullptr, (EPropertyFlags)0x0010000000000015, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FLyraAccoladeDefinitionRow, LocationTag), Z_Construct_UScriptStruct_FGameplayTag, METADATA_PARAMS(Z_Construct_UScriptStruct_FLyraAccoladeDefinitionRow_Statics::NewProp_LocationTag_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FLyraAccoladeDefinitionRow_Statics::NewProp_LocationTag_MetaData)) }; // 802167388
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FLyraAccoladeDefinitionRow_Statics::NewProp_AccoladeTags_MetaData[] = {
		{ "Category", "LyraAccoladeDefinitionRow" },
		{ "Comment", "// Tags associated with this accolade\n" },
		{ "ModuleRelativePath", "Public/Accolades/LyraAccoladeDefinition.h" },
		{ "ToolTip", "Tags associated with this accolade" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FLyraAccoladeDefinitionRow_Statics::NewProp_AccoladeTags = { "AccoladeTags", nullptr, (EPropertyFlags)0x0010000000000015, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FLyraAccoladeDefinitionRow, AccoladeTags), Z_Construct_UScriptStruct_FGameplayTagContainer, METADATA_PARAMS(Z_Construct_UScriptStruct_FLyraAccoladeDefinitionRow_Statics::NewProp_AccoladeTags_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FLyraAccoladeDefinitionRow_Statics::NewProp_AccoladeTags_MetaData)) }; // 506667518
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FLyraAccoladeDefinitionRow_Statics::NewProp_CancelAccoladesWithTag_MetaData[] = {
		{ "Category", "LyraAccoladeDefinitionRow" },
		{ "Comment", "// When this accolade is displayed, any existing displayed/pending accolades with any of\n// these tags will be removed (e.g., getting a triple-elim will suppress a double-elim)\n" },
		{ "ModuleRelativePath", "Public/Accolades/LyraAccoladeDefinition.h" },
		{ "ToolTip", "When this accolade is displayed, any existing displayed/pending accolades with any of\nthese tags will be removed (e.g., getting a triple-elim will suppress a double-elim)" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FLyraAccoladeDefinitionRow_Statics::NewProp_CancelAccoladesWithTag = { "CancelAccoladesWithTag", nullptr, (EPropertyFlags)0x0010000000000015, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FLyraAccoladeDefinitionRow, CancelAccoladesWithTag), Z_Construct_UScriptStruct_FGameplayTagContainer, METADATA_PARAMS(Z_Construct_UScriptStruct_FLyraAccoladeDefinitionRow_Statics::NewProp_CancelAccoladesWithTag_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FLyraAccoladeDefinitionRow_Statics::NewProp_CancelAccoladesWithTag_MetaData)) }; // 506667518
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FLyraAccoladeDefinitionRow_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FLyraAccoladeDefinitionRow_Statics::NewProp_DisplayName,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FLyraAccoladeDefinitionRow_Statics::NewProp_Sound,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FLyraAccoladeDefinitionRow_Statics::NewProp_Icon,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FLyraAccoladeDefinitionRow_Statics::NewProp_DisplayDuration,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FLyraAccoladeDefinitionRow_Statics::NewProp_LocationTag,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FLyraAccoladeDefinitionRow_Statics::NewProp_AccoladeTags,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FLyraAccoladeDefinitionRow_Statics::NewProp_CancelAccoladesWithTag,
	};
	const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FLyraAccoladeDefinitionRow_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_ShooterCoreRuntime,
		Z_Construct_UScriptStruct_FTableRowBase,
		&NewStructOps,
		"LyraAccoladeDefinitionRow",
		sizeof(FLyraAccoladeDefinitionRow),
		alignof(FLyraAccoladeDefinitionRow),
		Z_Construct_UScriptStruct_FLyraAccoladeDefinitionRow_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FLyraAccoladeDefinitionRow_Statics::PropPointers),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(Z_Construct_UScriptStruct_FLyraAccoladeDefinitionRow_Statics::Struct_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FLyraAccoladeDefinitionRow_Statics::Struct_MetaDataParams))
	};
	UScriptStruct* Z_Construct_UScriptStruct_FLyraAccoladeDefinitionRow()
	{
		if (!Z_Registration_Info_UScriptStruct_LyraAccoladeDefinitionRow.InnerSingleton)
		{
			UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_LyraAccoladeDefinitionRow.InnerSingleton, Z_Construct_UScriptStruct_FLyraAccoladeDefinitionRow_Statics::ReturnStructParams);
		}
		return Z_Registration_Info_UScriptStruct_LyraAccoladeDefinitionRow.InnerSingleton;
	}
	void ULyraAccoladeDefinition::StaticRegisterNativesULyraAccoladeDefinition()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(ULyraAccoladeDefinition);
	UClass* Z_Construct_UClass_ULyraAccoladeDefinition_NoRegister()
	{
		return ULyraAccoladeDefinition::StaticClass();
	}
	struct Z_Construct_UClass_ULyraAccoladeDefinition_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Sound_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_Sound;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Icon_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_Icon;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_AccoladeTags_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_AccoladeTags;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_CancelAccoladesWithTag_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_CancelAccoladesWithTag;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_ULyraAccoladeDefinition_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UDataAsset,
		(UObject* (*)())Z_Construct_UPackage__Script_ShooterCoreRuntime,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ULyraAccoladeDefinition_Statics::Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "Comment", "/**\n * \n */" },
		{ "IncludePath", "Accolades/LyraAccoladeDefinition.h" },
		{ "ModuleRelativePath", "Public/Accolades/LyraAccoladeDefinition.h" },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ULyraAccoladeDefinition_Statics::NewProp_Sound_MetaData[] = {
		{ "Category", "LyraAccoladeDefinition" },
		{ "Comment", "// The sound to play\n" },
		{ "ModuleRelativePath", "Public/Accolades/LyraAccoladeDefinition.h" },
		{ "ToolTip", "The sound to play" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ULyraAccoladeDefinition_Statics::NewProp_Sound = { "Sound", nullptr, (EPropertyFlags)0x0010000000000015, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ULyraAccoladeDefinition, Sound), Z_Construct_UClass_USoundBase_NoRegister, METADATA_PARAMS(Z_Construct_UClass_ULyraAccoladeDefinition_Statics::NewProp_Sound_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ULyraAccoladeDefinition_Statics::NewProp_Sound_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ULyraAccoladeDefinition_Statics::NewProp_Icon_MetaData[] = {
		{ "AllowedClasses", "Texture,MaterialInterface,SlateTextureAtlasInterface" },
		{ "Category", "LyraAccoladeDefinition" },
		{ "Comment", "// The icon to display\x09\n" },
		{ "DisallowedClasses", "MediaTexture" },
		{ "DisplayThumbnail", "true" },
		{ "ModuleRelativePath", "Public/Accolades/LyraAccoladeDefinition.h" },
		{ "ToolTip", "The icon to display" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ULyraAccoladeDefinition_Statics::NewProp_Icon = { "Icon", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ULyraAccoladeDefinition, Icon), Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(Z_Construct_UClass_ULyraAccoladeDefinition_Statics::NewProp_Icon_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ULyraAccoladeDefinition_Statics::NewProp_Icon_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ULyraAccoladeDefinition_Statics::NewProp_AccoladeTags_MetaData[] = {
		{ "Category", "LyraAccoladeDefinition" },
		{ "Comment", "// Tags associated with this accolade\n" },
		{ "ModuleRelativePath", "Public/Accolades/LyraAccoladeDefinition.h" },
		{ "ToolTip", "Tags associated with this accolade" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_ULyraAccoladeDefinition_Statics::NewProp_AccoladeTags = { "AccoladeTags", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ULyraAccoladeDefinition, AccoladeTags), Z_Construct_UScriptStruct_FGameplayTagContainer, METADATA_PARAMS(Z_Construct_UClass_ULyraAccoladeDefinition_Statics::NewProp_AccoladeTags_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ULyraAccoladeDefinition_Statics::NewProp_AccoladeTags_MetaData)) }; // 506667518
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ULyraAccoladeDefinition_Statics::NewProp_CancelAccoladesWithTag_MetaData[] = {
		{ "Category", "LyraAccoladeDefinition" },
		{ "Comment", "// When this accolade is displayed, any existing displayed/pending accolades with any of\n// these tags will be removed (e.g., getting a triple-elim will suppress a double-elim)\n" },
		{ "ModuleRelativePath", "Public/Accolades/LyraAccoladeDefinition.h" },
		{ "ToolTip", "When this accolade is displayed, any existing displayed/pending accolades with any of\nthese tags will be removed (e.g., getting a triple-elim will suppress a double-elim)" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_ULyraAccoladeDefinition_Statics::NewProp_CancelAccoladesWithTag = { "CancelAccoladesWithTag", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ULyraAccoladeDefinition, CancelAccoladesWithTag), Z_Construct_UScriptStruct_FGameplayTagContainer, METADATA_PARAMS(Z_Construct_UClass_ULyraAccoladeDefinition_Statics::NewProp_CancelAccoladesWithTag_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ULyraAccoladeDefinition_Statics::NewProp_CancelAccoladesWithTag_MetaData)) }; // 506667518
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_ULyraAccoladeDefinition_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULyraAccoladeDefinition_Statics::NewProp_Sound,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULyraAccoladeDefinition_Statics::NewProp_Icon,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULyraAccoladeDefinition_Statics::NewProp_AccoladeTags,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ULyraAccoladeDefinition_Statics::NewProp_CancelAccoladesWithTag,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_ULyraAccoladeDefinition_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ULyraAccoladeDefinition>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_ULyraAccoladeDefinition_Statics::ClassParams = {
		&ULyraAccoladeDefinition::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_ULyraAccoladeDefinition_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_ULyraAccoladeDefinition_Statics::PropPointers),
		0,
		0x000000A0u,
		METADATA_PARAMS(Z_Construct_UClass_ULyraAccoladeDefinition_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_ULyraAccoladeDefinition_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_ULyraAccoladeDefinition()
	{
		if (!Z_Registration_Info_UClass_ULyraAccoladeDefinition.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ULyraAccoladeDefinition.OuterSingleton, Z_Construct_UClass_ULyraAccoladeDefinition_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_ULyraAccoladeDefinition.OuterSingleton;
	}
	template<> SHOOTERCORERUNTIME_API UClass* StaticClass<ULyraAccoladeDefinition>()
	{
		return ULyraAccoladeDefinition::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(ULyraAccoladeDefinition);
	struct Z_CompiledInDeferFile_FID_Cerberus_Plugins_GameFeatures_ShooterCore_Source_ShooterCoreRuntime_Public_Accolades_LyraAccoladeDefinition_h_Statics
	{
		static const FStructRegisterCompiledInInfo ScriptStructInfo[];
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FStructRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Cerberus_Plugins_GameFeatures_ShooterCore_Source_ShooterCoreRuntime_Public_Accolades_LyraAccoladeDefinition_h_Statics::ScriptStructInfo[] = {
		{ FLyraAccoladeDefinitionRow::StaticStruct, Z_Construct_UScriptStruct_FLyraAccoladeDefinitionRow_Statics::NewStructOps, TEXT("LyraAccoladeDefinitionRow"), &Z_Registration_Info_UScriptStruct_LyraAccoladeDefinitionRow, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FLyraAccoladeDefinitionRow), 1001208819U) },
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Cerberus_Plugins_GameFeatures_ShooterCore_Source_ShooterCoreRuntime_Public_Accolades_LyraAccoladeDefinition_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_ULyraAccoladeDefinition, ULyraAccoladeDefinition::StaticClass, TEXT("ULyraAccoladeDefinition"), &Z_Registration_Info_UClass_ULyraAccoladeDefinition, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ULyraAccoladeDefinition), 2947181633U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Cerberus_Plugins_GameFeatures_ShooterCore_Source_ShooterCoreRuntime_Public_Accolades_LyraAccoladeDefinition_h_1343215731(TEXT("/Script/ShooterCoreRuntime"),
		Z_CompiledInDeferFile_FID_Cerberus_Plugins_GameFeatures_ShooterCore_Source_ShooterCoreRuntime_Public_Accolades_LyraAccoladeDefinition_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Cerberus_Plugins_GameFeatures_ShooterCore_Source_ShooterCoreRuntime_Public_Accolades_LyraAccoladeDefinition_h_Statics::ClassInfo),
		Z_CompiledInDeferFile_FID_Cerberus_Plugins_GameFeatures_ShooterCore_Source_ShooterCoreRuntime_Public_Accolades_LyraAccoladeDefinition_h_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Cerberus_Plugins_GameFeatures_ShooterCore_Source_ShooterCoreRuntime_Public_Accolades_LyraAccoladeDefinition_h_Statics::ScriptStructInfo),
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
