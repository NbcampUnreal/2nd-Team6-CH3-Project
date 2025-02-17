// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "EdmundPrj/Public/UI/BaseWidget.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBaseWidget() {}

// Begin Cross Module References
EDMUNDPRJ_API UClass* Z_Construct_UClass_UBaseWidget();
EDMUNDPRJ_API UClass* Z_Construct_UClass_UBaseWidget_NoRegister();
UMG_API UClass* Z_Construct_UClass_UUserWidget();
UPackage* Z_Construct_UPackage__Script_EdmundPrj();
// End Cross Module References

// Begin Class UBaseWidget
void UBaseWidget::StaticRegisterNativesUBaseWidget()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UBaseWidget);
UClass* Z_Construct_UClass_UBaseWidget_NoRegister()
{
	return UBaseWidget::StaticClass();
}
struct Z_Construct_UClass_UBaseWidget_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "IncludePath", "UI/BaseWidget.h" },
		{ "ModuleRelativePath", "Public/UI/BaseWidget.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UBaseWidget>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UBaseWidget_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UUserWidget,
	(UObject* (*)())Z_Construct_UPackage__Script_EdmundPrj,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UBaseWidget_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UBaseWidget_Statics::ClassParams = {
	&UBaseWidget::StaticClass,
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
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UBaseWidget_Statics::Class_MetaDataParams), Z_Construct_UClass_UBaseWidget_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UBaseWidget()
{
	if (!Z_Registration_Info_UClass_UBaseWidget.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UBaseWidget.OuterSingleton, Z_Construct_UClass_UBaseWidget_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UBaseWidget.OuterSingleton;
}
template<> EDMUNDPRJ_API UClass* StaticClass<UBaseWidget>()
{
	return UBaseWidget::StaticClass();
}
UBaseWidget::UBaseWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UBaseWidget);
UBaseWidget::~UBaseWidget() {}
// End Class UBaseWidget

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_jucho_Desktop_UnrealPrj_2nd_Team6_CH3_Project_EdmundPrj_Source_EdmundPrj_Public_UI_BaseWidget_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UBaseWidget, UBaseWidget::StaticClass, TEXT("UBaseWidget"), &Z_Registration_Info_UClass_UBaseWidget, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UBaseWidget), 579984157U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_jucho_Desktop_UnrealPrj_2nd_Team6_CH3_Project_EdmundPrj_Source_EdmundPrj_Public_UI_BaseWidget_h_1304970993(TEXT("/Script/EdmundPrj"),
	Z_CompiledInDeferFile_FID_Users_jucho_Desktop_UnrealPrj_2nd_Team6_CH3_Project_EdmundPrj_Source_EdmundPrj_Public_UI_BaseWidget_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_jucho_Desktop_UnrealPrj_2nd_Team6_CH3_Project_EdmundPrj_Source_EdmundPrj_Public_UI_BaseWidget_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
