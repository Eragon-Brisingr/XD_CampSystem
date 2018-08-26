// Fill out your copyright notice in the Description page of Project Settings.

#include "XD_CampConfig_Customization.h"
#include "XD_CampSystemType.h"
#include <StringTable.h>
#include "XD_CampSystemSetting.h"
#include <DetailWidgetRow.h>
#include "XD_CampSystem_EditorUtility.h"

void FXD_CampConfig_Customization::CustomizeHeader(TSharedRef<class IPropertyHandle> StructPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	TSharedPtr<IPropertyHandle> CampName_PropertyHandle = FPropertyCustomizeHelper::GetPropertyHandleByName(StructPropertyHandle, GET_MEMBER_NAME_STRING_CHECKED(FXD_CampConfig, CampName));
	CampName_PropertyHandle->SetOnPropertyValueChanged(FSimpleDelegate::CreateLambda([=]()
	{
		if (UStringTable* CampNameStringTable = GetDefault<UXD_CampSystemSetting>()->GetCampNameStringTable())
		{
			FText& CampName = FPropertyCustomizeHelper::Value<FText>(CampName_PropertyHandle);
			if (CampName.IsFromStringTable())
			{
				FName CurStringTableID;
				FString CurKey;
				FTextInspector::GetTableIdAndKey(CampName, CurStringTableID, CurKey);
				if (CurStringTableID != CampNameStringTable->GetStringTableId())
				{
					CampName = FText::FromStringTable(CampNameStringTable->GetStringTableId(), FString());
				}
			}
			else
			{
				CampName = FText::FromStringTable(CampNameStringTable->GetStringTableId(), FString());
			}
		}
	}));

	HeaderRow.NameContent()
		[
			StructPropertyHandle->CreatePropertyNameWidget()
		]
	.ValueContent()
		[
			CampName_PropertyHandle->CreatePropertyValueWidget(true)
		];
}
