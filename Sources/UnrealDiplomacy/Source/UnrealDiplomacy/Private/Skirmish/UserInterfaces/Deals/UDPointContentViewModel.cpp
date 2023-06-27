// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDPointContentViewModel.h"
#include "Skirmish/UserInterfaces/UDParameterEditorViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Skirmish/UDSkirmishHUD.h"
#include "Core/Simulation/Actions/UDDealActionPointAdd.h"

#define LOCTEXT_NAMESPACE "PointContent"

int32 UUDPointContentViewModel::UniqueNameDefinition = 0;

void UUDPointContentViewModel::Initialize()
{
	if (!IsUniqueNameDefined)
	{
		DefineInstances();
		IsUniqueNameDefined = true;
	}
	//FText pointTitle = FText(LOCTEXT("PointContent", "Point Title"));
	//SetPointTitleText(pointTitle);
}

void UUDPointContentViewModel::Update()
{
	FText pointTitle = FText::Format(LOCTEXT("PointContent", "Deal {0} Point {1}"), Content.DealId, Content.PointId);
	SetPointTitleText(pointTitle);
}

#undef LOCTEXT_NAMESPACE

void UUDPointContentViewModel::SetContent(FUDDealPointMinimalInfo content)
{
	Content = content;
	//FUDDealInfo fullContent = Model->GetDealInfo(Content.DealId, Content.PointId);
}

//void UUDPointContentViewModel::CreatePoint()
//{
//	UE_LOG(LogTemp, Log, TEXT("UUDPointContentViewModel: CreatePoint."));
//	Model->RequestAction(Model->GetAction(UUDDealActionPointAdd::ActionTypeId, { Content.DealId }));
//}

void UUDPointContentViewModel::UpdateEditor()
{
	UE_LOG(LogTemp, Log, TEXT("UUDPointContentViewModel: UpdateEditor."));
	ParameterEditorInstance->SetContent(Content.Parameters);
}

void UUDPointContentViewModel::DefineInstances()
{
	int32 uniqueId = UUDPointContentViewModel::UniqueNameDefinition++;
	ParameterEditorType = UUDParameterEditorViewModel::StaticClass();
	ParameterEditorInstanceName = FName(ParameterEditorInstanceName.ToString() + FString::FromInt(uniqueId));
	UE_LOG(LogTemp, Log, TEXT("UUDPointContentViewModel: Defined editor [%d]."), uniqueId);
}

void UUDPointContentViewModel::SetPointTitleText(FText newPointTitleText)
{
	UE_MVVM_SET_PROPERTY_VALUE(PointTitleText, newPointTitleText);
}

FText UUDPointContentViewModel::GetPointTitleText() const
{
	return PointTitleText;
}

void UUDPointContentViewModel::SetIsValidContentValue(bool newIsValidContentValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(IsValidContentValue, newIsValidContentValue);
}

bool UUDPointContentViewModel::GetIsValidContentValue() const
{
	return IsValidContentValue;
}