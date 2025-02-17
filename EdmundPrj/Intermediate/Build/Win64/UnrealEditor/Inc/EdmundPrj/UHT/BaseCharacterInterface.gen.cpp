// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "EdmundPrj/Public/Player/BaseCharacterInterface.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBaseCharacterInterface() {}

// Begin Cross Module References
COREUOBJECT_API UClass* Z_Construct_UClass_UInterface();
EDMUNDPRJ_API UClass* Z_Construct_UClass_UBaseCharacterInterface();
EDMUNDPRJ_API UClass* Z_Construct_UClass_UBaseCharacterInterface_NoRegister();
UPackage* Z_Construct_UPackage__Script_EdmundPrj();
// End Cross Module References

// Begin Interface UBaseCharacterInterface
void UBaseCharacterInterface::StaticRegisterNativesUBaseCharacterInterface()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UBaseCharacterInterface);
UClass* Z_Construct_UClass_UBaseCharacterInterface_NoRegister()
{
	return UBaseCharacterInterface::StaticClass();
}
struct Z_Construct_UClass_UBaseCharacterInterface_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/Player/BaseCharacterInterface.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<IBaseCharacterInterface>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UBaseCharacterInterface_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UInterface,
	(UObject* (*)())Z_Construct_UPackage__Script_EdmundPrj,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UBaseCharacterInterface_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UBaseCharacterInterface_Statics::ClassParams = {
	&UBaseCharacterInterface::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	0,
	0,
	0x000840A1u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UBaseCharacterInterface_Statics::Class_MetaDataParams), Z_Construct_UClass_UBaseCharacterInterface_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UBaseCharacterInterface()
{
	if (!Z_Registration_Info_UClass_UBaseCharacterInterface.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UBaseCharacterInterface.OuterSingleton, Z_Construct_UClass_UBaseCharacterInterface_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UBaseCharacterInterface.OuterSingleton;
}
template<> EDMUNDPRJ_API UClass* StaticClass<UBaseCharacterInterface>()
{
	return UBaseCharacterInterface::StaticClass();
}
UBaseCharacterInterface::UBaseCharacterInterface(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UBaseCharacterInterface);
UBaseCharacterInterface::~UBaseCharacterInterface() {}
// End Interface UBaseCharacterInterface

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_jucho_Desktop_UnrealPrj_2nd_Team6_CH3_Project_EdmundPrj_Source_EdmundPrj_Public_Player_BaseCharacterInterface_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UBaseCharacterInterface, UBaseCharacterInterface::StaticClass, TEXT("UBaseCharacterInterface"), &Z_Registration_Info_UClass_UBaseCharacterInterface, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UBaseCharacterInterface), 1066498001U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jucho_Desktop_UnrealPrj_2nd_Team6_CH3_Project_EdmundPrj_Source_EdmundPrj_Public_Player_BaseCharacterInterface_h_1470712723(TEXT("/Script/EdmundPrj"),
	Z_CompiledInDeferFile_FID_Users_jucho_Desktop_UnrealPrj_2nd_Team6_CH3_Project_EdmundPrj_Source_EdmundPrj_Public_Player_BaseCharacterInterface_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jucho_Desktop_UnrealPrj_2nd_Team6_CH3_Project_EdmundPrj_Source_EdmundPrj_Public_Player_BaseCharacterInterface_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
