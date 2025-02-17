// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "EdmundPrj/Public/UI/TitleWidget.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeTitleWidget() {}

// Begin Cross Module References
EDMUNDPRJ_API UClass* Z_Construct_UClass_UBaseWidget();
EDMUNDPRJ_API UClass* Z_Construct_UClass_UTitleWidget();
EDMUNDPRJ_API UClass* Z_Construct_UClass_UTitleWidget_NoRegister();
UPackage* Z_Construct_UPackage__Script_EdmundPrj();
// End Cross Module References

// Begin Class UTitleWidget
void UTitleWidget::StaticRegisterNativesUTitleWidget()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UTitleWidget);
UClass* Z_Construct_UClass_UTitleWidget_NoRegister()
{
	return UTitleWidget::StaticClass();
}
struct Z_Construct_UClass_UTitleWidget_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "IncludePath", "UI/TitleWidget.h" },
		{ "ModuleRelativePath", "Public/UI/TitleWidget.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UTitleWidget>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UTitleWidget_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UBaseWidget,
	(UObject* (*)())Z_Construct_UPackage__Script_EdmundPrj,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UTitleWidget_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UTitleWidget_Statics::ClassParams = {
	&UTitleWidget::StaticClass,
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
	0x00B010A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UTitleWidget_Statics::Class_MetaDataParams), Z_Construct_UClass_UTitleWidget_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UTitleWidget()
{
	if (!Z_Registration_Info_UClass_UTitleWidget.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UTitleWidget.OuterSingleton, Z_Construct_UClass_UTitleWidget_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UTitleWidget.OuterSingleton;
}
template<> EDMUNDPRJ_API UClass* StaticClass<UTitleWidget>()
{
	return UTitleWidget::StaticClass();
}
UTitleWidget::UTitleWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UTitleWidget);
UTitleWidget::~UTitleWidget() {}
// End Class UTitleWidget

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_jucho_Desktop_UnrealPrj_2nd_Team6_CH3_Project_EdmundPrj_Source_EdmundPrj_Public_UI_TitleWidget_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UTitleWidget, UTitleWidget::StaticClass, TEXT("UTitleWidget"), &Z_Registration_Info_UClass_UTitleWidget, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UTitleWidget), 561521128U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jucho_Desktop_UnrealPrj_2nd_Team6_CH3_Project_EdmundPrj_Source_EdmundPrj_Public_UI_TitleWidget_h_4113032545(TEXT("/Script/EdmundPrj"),
	Z_CompiledInDeferFile_FID_Users_jucho_Desktop_UnrealPrj_2nd_Team6_CH3_Project_EdmundPrj_Source_EdmundPrj_Public_UI_TitleWidget_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jucho_Desktop_UnrealPrj_2nd_Team6_CH3_Project_EdmundPrj_Source_EdmundPrj_Public_UI_TitleWidget_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
