// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "EdmundPrj/Public/NPC/BaseNPC.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBaseNPC() {}

// Begin Cross Module References
EDMUNDPRJ_API UClass* Z_Construct_UClass_ABaseNPC();
EDMUNDPRJ_API UClass* Z_Construct_UClass_ABaseNPC_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_ACharacter();
UPackage* Z_Construct_UPackage__Script_EdmundPrj();
// End Cross Module References

// Begin Class ABaseNPC
void ABaseNPC::StaticRegisterNativesABaseNPC()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(ABaseNPC);
UClass* Z_Construct_UClass_ABaseNPC_NoRegister()
{
	return ABaseNPC::StaticClass();
}
struct Z_Construct_UClass_ABaseNPC_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "HideCategories", "Navigation" },
		{ "IncludePath", "NPC/BaseNPC.h" },
		{ "ModuleRelativePath", "Public/NPC/BaseNPC.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ABaseNPC>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_ABaseNPC_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_ACharacter,
	(UObject* (*)())Z_Construct_UPackage__Script_EdmundPrj,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ABaseNPC_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_ABaseNPC_Statics::ClassParams = {
	&ABaseNPC::StaticClass,
	"Game",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	0,
	0,
	0x009000A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ABaseNPC_Statics::Class_MetaDataParams), Z_Construct_UClass_ABaseNPC_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_ABaseNPC()
{
	if (!Z_Registration_Info_UClass_ABaseNPC.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ABaseNPC.OuterSingleton, Z_Construct_UClass_ABaseNPC_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_ABaseNPC.OuterSingleton;
}
template<> EDMUNDPRJ_API UClass* StaticClass<ABaseNPC>()
{
	return ABaseNPC::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(ABaseNPC);
ABaseNPC::~ABaseNPC() {}
// End Class ABaseNPC

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_jucho_Desktop_UnrealPrj_2nd_Team6_CH3_Project_EdmundPrj_Source_EdmundPrj_Public_NPC_BaseNPC_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_ABaseNPC, ABaseNPC::StaticClass, TEXT("ABaseNPC"), &Z_Registration_Info_UClass_ABaseNPC, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ABaseNPC), 1323896060U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jucho_Desktop_UnrealPrj_2nd_Team6_CH3_Project_EdmundPrj_Source_EdmundPrj_Public_NPC_BaseNPC_h_1032878231(TEXT("/Script/EdmundPrj"),
	Z_CompiledInDeferFile_FID_Users_jucho_Desktop_UnrealPrj_2nd_Team6_CH3_Project_EdmundPrj_Source_EdmundPrj_Public_NPC_BaseNPC_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jucho_Desktop_UnrealPrj_2nd_Team6_CH3_Project_EdmundPrj_Source_EdmundPrj_Public_NPC_BaseNPC_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
