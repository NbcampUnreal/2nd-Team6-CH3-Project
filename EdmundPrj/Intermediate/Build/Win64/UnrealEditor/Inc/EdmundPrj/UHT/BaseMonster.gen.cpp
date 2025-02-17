// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "EdmundPrj/Public/Monster/BaseMonster.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBaseMonster() {}

// Begin Cross Module References
EDMUNDPRJ_API UClass* Z_Construct_UClass_ABaseMonster();
EDMUNDPRJ_API UClass* Z_Construct_UClass_ABaseMonster_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_ACharacter();
UPackage* Z_Construct_UPackage__Script_EdmundPrj();
// End Cross Module References

// Begin Class ABaseMonster
void ABaseMonster::StaticRegisterNativesABaseMonster()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(ABaseMonster);
UClass* Z_Construct_UClass_ABaseMonster_NoRegister()
{
	return ABaseMonster::StaticClass();
}
struct Z_Construct_UClass_ABaseMonster_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "HideCategories", "Navigation" },
		{ "IncludePath", "Monster/BaseMonster.h" },
		{ "ModuleRelativePath", "Public/Monster/BaseMonster.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ABaseMonster>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_ABaseMonster_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_ACharacter,
	(UObject* (*)())Z_Construct_UPackage__Script_EdmundPrj,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ABaseMonster_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_ABaseMonster_Statics::ClassParams = {
	&ABaseMonster::StaticClass,
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
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ABaseMonster_Statics::Class_MetaDataParams), Z_Construct_UClass_ABaseMonster_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_ABaseMonster()
{
	if (!Z_Registration_Info_UClass_ABaseMonster.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ABaseMonster.OuterSingleton, Z_Construct_UClass_ABaseMonster_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_ABaseMonster.OuterSingleton;
}
template<> EDMUNDPRJ_API UClass* StaticClass<ABaseMonster>()
{
	return ABaseMonster::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(ABaseMonster);
ABaseMonster::~ABaseMonster() {}
// End Class ABaseMonster

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_jucho_Desktop_UnrealPrj_2nd_Team6_CH3_Project_EdmundPrj_Source_EdmundPrj_Public_Monster_BaseMonster_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_ABaseMonster, ABaseMonster::StaticClass, TEXT("ABaseMonster"), &Z_Registration_Info_UClass_ABaseMonster, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ABaseMonster), 3859042200U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jucho_Desktop_UnrealPrj_2nd_Team6_CH3_Project_EdmundPrj_Source_EdmundPrj_Public_Monster_BaseMonster_h_1381066158(TEXT("/Script/EdmundPrj"),
	Z_CompiledInDeferFile_FID_Users_jucho_Desktop_UnrealPrj_2nd_Team6_CH3_Project_EdmundPrj_Source_EdmundPrj_Public_Monster_BaseMonster_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jucho_Desktop_UnrealPrj_2nd_Team6_CH3_Project_EdmundPrj_Source_EdmundPrj_Public_Monster_BaseMonster_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
