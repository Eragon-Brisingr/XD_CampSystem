// Fill out your copyright notice in the Description page of Project Settings.

#include "XD_CampConfig_Customization.h"
#include "XD_CampSystemType.h"
#include <StringTable.h>
#include "XD_CampSystemSetting.h"
#include <DetailWidgetRow.h>
#include "XD_CampSystem_EditorUtility.h"
#include <STextComboBox.h>
#include <StringTableCore.h>
#include "XD_CampGraph.h"
#include "XD_CampInfo.h"

void FXD_CampConfig_Customization::CustomizeHeader(TSharedRef<class IPropertyHandle> StructPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	TSharedPtr<IPropertyHandle> CampName_PropertyHandle = FPropertyCustomizeHelper::GetPropertyHandleByName(StructPropertyHandle, GET_MEMBER_NAME_CHECKED(FXD_CampConfig, CampName));

	if (!GetDefault<UXD_CampSystemSetting>()->GlobalCampGraph.IsNull())
	{
		CampNameList = GetDefault<UXD_CampSystemSetting>()->GlobalCampGraph.LoadSynchronous()->GetAllCampNames();
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
					UXD_CampGraph* CampGraph = GetDefault<UXD_CampSystemSetting>()->GlobalCampGraph.LoadSynchronous();
					if (UXD_CampInfo** P_CampInfo = CampGraph->CampList.FindByPredicate([&](const UXD_CampInfo* E) {return E->CampName.ToString() == *Selection.Get(); }))
					{
						if (UXD_CampInfo* CampInfo = *P_CampInfo)
						{
							CampName_PropertyHandle->SetValue(CampInfo->CampName);
							FPropertyCustomizeHelper::SetValue(StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FXD_CampConfig, CampGuid)), CampInfo->CampGuid);
						}
					}
				}
			})
			.InitiallySelectedItem(InitSelectedText)
			.IsEnabled(!StructPropertyHandle->IsEditConst())
		];
}
