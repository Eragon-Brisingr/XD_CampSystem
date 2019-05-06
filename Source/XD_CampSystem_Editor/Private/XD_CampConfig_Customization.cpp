// Fill out your copyright notice in the Description page of Project Settings.

#include "XD_CampConfig_Customization.h"
#include "XD_CampSystemType.h"
#include <StringTable.h>
#include "XD_CampSystemSetting.h"
#include <DetailWidgetRow.h>
#include "XD_CampSystem_EditorUtility.h"
#include <STextComboBox.h>
#include <StringTableCore.h>
#include "XD_CampRelationshipGraph.h"

void FXD_CampConfig_Customization::CustomizeHeader(TSharedRef<class IPropertyHandle> StructPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	TSharedPtr<IPropertyHandle> CampName_PropertyHandle = FPropertyCustomizeHelper::GetPropertyHandleByName(StructPropertyHandle, GET_MEMBER_NAME_STRING_CHECKED(FXD_CampConfig, CampName));

	if (!GetDefault<UXD_CampSystemSetting>()->CampRelationshipGrap.IsNull())
	{
		ValidCampNames = GetDefault<UXD_CampSystemSetting>()->CampRelationshipGrap.LoadSynchronous()->GetAllCampNames();
		for (const FText& ValidCampName : ValidCampNames)
		{
			CampNameList.Add(MakeShareable(new FString(ValidCampName.ToString())));
		}
	}

	if (FText* CampName = FPropertyCustomizeHelper::Value<FText>(CampName_PropertyHandle))
	{
		int32 Index = CampNameList.IndexOfByPredicate([&](TSharedPtr<FString> e) {return *e.Get() == CampName->ToString(); });
		InitSelectedText = Index != INDEX_NONE ? CampNameList[Index] : MakeShareable(new FString(FString::Printf(TEXT("[%s]_无效的阵营名"), *CampName->ToString())));
	}

	HeaderRow.NameContent()
		[
			StructPropertyHandle->CreatePropertyNameWidget()
		]
	.ValueContent()
		[
			SNew(STextComboBox)
			.OptionsSource(&CampNameList)
			.OnSelectionChanged_Lambda([=](TSharedPtr<FString> Selection, ESelectInfo::Type SelectInfo)
			{
				if (SelectInfo == ESelectInfo::OnMouseClick)
				{
					CampName_PropertyHandle->SetValue(*ValidCampNames.FindByPredicate([&](const FText& CampName) {return CampName.ToString() == *Selection.Get(); }));
				}
			})
			.InitiallySelectedItem(InitSelectedText)
			.IsEnabled(!StructPropertyHandle->IsEditConst())
		];
}
