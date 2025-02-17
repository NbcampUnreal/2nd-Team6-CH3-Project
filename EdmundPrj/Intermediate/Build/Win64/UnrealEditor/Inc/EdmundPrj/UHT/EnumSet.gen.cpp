// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "EdmundPrj/Public/System/EnumSet.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeEnumSet() {}

// Begin Cross Module References
COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
EDMUNDPRJ_API UClass* Z_Construct_UClass_UEnumSet();
EDMUNDPRJ_API UClass* Z_Construct_UClass_UEnumSet_NoRegister();
UPackage* Z_Construct_UPackage__Script_EdmundPrj();
// End Cross Module References

// Begin Class UEnumSet
void UEnumSet::StaticRegisterNativesUEnumSet()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UEnumSet);
UClass* Z_Construct_UClass_UEnumSet_NoRegister()
{
	return UEnumSet::StaticClass();
}
struct Z_Construct_UClass_UEnumSet_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "IncludePath", "System/EnumSet.h" },
		{ "ModuleRelativePath", "Public/System/EnumSet.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UEnumSet>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UEnumSet_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UObject,
	(UObject* (*)())Z_Construct_UPackage__Script_EdmundPrj,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UEnumSet_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UEnumSet_Statics::ClassParams = {
	&UEnumSet::StaticClass,
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
	0x001000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UEnumSet_Statics::Class_MetaDataParams), Z_Construct_UClass_UEnumSet_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UEnumSet()
{
	if (!Z_Registration_Info_UClass_UEnumSet.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UEnumSet.OuterSingleton, Z_Construct_UClass_UEnumSet_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UEnumSet.OuterSingleton;
}
template<> EDMUNDPRJ_API UClass* StaticClass<UEnumSet>()
{
	return UEnumSet::StaticClass();
}
UEnumSet::UEnumSet(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UEnumSet);
UEnumSet::~UEnumSet() {}
// End Class UEnumSet

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_jucho_Desktop_UnrealPrj_2nd_Team6_CH3_Project_EdmundPrj_Source_EdmundPrj_Public_System_EnumSet_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UEnumSet, UEnumSet::StaticClass, TEXT("UEnumSet"), &Z_Registration_Info_UClass_UEnumSet, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UEnumSet), 2226486544U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jucho_Desktop_UnrealPrj_2nd_Team6_CH3_Project_EdmundPrj_Source_EdmundPrj_Public_System_EnumSet_h_3013554727(TEXT("/Script/EdmundPrj"),
	Z_CompiledInDeferFile_FID_Users_jucho_Desktop_UnrealPrj_2nd_Team6_CH3_Project_EdmundPrj_Source_EdmundPrj_Public_System_EnumSet_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jucho_Desktop_UnrealPrj_2nd_Team6_CH3_Project_EdmundPrj_Source_EdmundPrj_Public_System_EnumSet_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
