// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "EdmundPrj/Public/System/EdmundGameMode.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeEdmundGameMode() {}

// Begin Cross Module References
EDMUNDPRJ_API UClass* Z_Construct_UClass_AEdmundGameMode();
EDMUNDPRJ_API UClass* Z_Construct_UClass_AEdmundGameMode_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_AGameMode();
UPackage* Z_Construct_UPackage__Script_EdmundPrj();
// End Cross Module References

// Begin Class AEdmundGameMode
void AEdmundGameMode::StaticRegisterNativesAEdmundGameMode()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(AEdmundGameMode);
UClass* Z_Construct_UClass_AEdmundGameMode_NoRegister()
{
	return AEdmundGameMode::StaticClass();
}
struct Z_Construct_UClass_AEdmundGameMode_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering HLOD WorldPartition DataLayers Transformation" },
		{ "IncludePath", "System/EdmundGameMode.h" },
		{ "ModuleRelativePath", "Public/System/EdmundGameMode.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AEdmundGameMode>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_AEdmundGameMode_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_AGameMode,
	(UObject* (*)())Z_Construct_UPackage__Script_EdmundPrj,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AEdmundGameMode_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_AEdmundGameMode_Statics::ClassParams = {
	&AEdmundGameMode::StaticClass,
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
	0x009002ACu,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AEdmundGameMode_Statics::Class_MetaDataParams), Z_Construct_UClass_AEdmundGameMode_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_AEdmundGameMode()
{
	if (!Z_Registration_Info_UClass_AEdmundGameMode.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AEdmundGameMode.OuterSingleton, Z_Construct_UClass_AEdmundGameMode_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_AEdmundGameMode.OuterSingleton;
}
template<> EDMUNDPRJ_API UClass* StaticClass<AEdmundGameMode>()
{
	return AEdmundGameMode::StaticClass();
}
AEdmundGameMode::AEdmundGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(AEdmundGameMode);
AEdmundGameMode::~AEdmundGameMode() {}
// End Class AEdmundGameMode

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_jucho_Desktop_UnrealPrj_2nd_Team6_CH3_Project_EdmundPrj_Source_EdmundPrj_Public_System_EdmundGameMode_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_AEdmundGameMode, AEdmundGameMode::StaticClass, TEXT("AEdmundGameMode"), &Z_Registration_Info_UClass_AEdmundGameMode, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AEdmundGameMode), 3372313964U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jucho_Desktop_UnrealPrj_2nd_Team6_CH3_Project_EdmundPrj_Source_EdmundPrj_Public_System_EdmundGameMode_h_1104135626(TEXT("/Script/EdmundPrj"),
	Z_CompiledInDeferFile_FID_Users_jucho_Desktop_UnrealPrj_2nd_Team6_CH3_Project_EdmundPrj_Source_EdmundPrj_Public_System_EdmundGameMode_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jucho_Desktop_UnrealPrj_2nd_Team6_CH3_Project_EdmundPrj_Source_EdmundPrj_Public_System_EdmundGameMode_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
