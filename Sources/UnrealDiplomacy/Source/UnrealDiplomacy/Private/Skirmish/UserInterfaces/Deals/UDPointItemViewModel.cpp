// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDPointItemViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Skirmish/UDSkirmishHUD.h"
#include "Core/Simulation/Actions/UDDealActionPointAdd.h"

#define LOCTEXT_NAMESPACE "PointItem"

void UUDPointItemViewModel::Initialize()
{
	FText createPoint = FText(LOCTEXT("DealGeneralTab", "+ Point"));
	SetCreatePointText(createPoint);
}

void UUDPointItemViewModel::Update()
{
	if (Content.PointId == UUDGlobalData::InvalidDealPointId)
	{
		SetIsValidContentValue(false);
	}
	else
	{
		SetIsValidContentValue(true);
	}
}

#undef LOCTEXT_NAMESPACE

void UUDPointItemViewModel::SetContent(FUDDealPointMinimalInfo content)
{
	Content = content;
}

void UUDPointItemViewModel::CreatePoint()
{
	UE_LOG(LogTemp, Log, TEXT("UUDPointItemViewModel: CreatePoint."));
	Model->RequestAction(Model->GetAction(UUDDealActionPointAdd::ActionTypeId, { Content.DealId }));
}

void UUDPointItemViewModel::SetCreatePointText(FText newCreatePointText)
{
	UE_MVVM_SET_PROPERTY_VALUE(CreatePointText, newCreatePointText);
}

FText UUDPointItemViewModel::GetCreatePointText() const
{
	return CreatePointText;
}

void UUDPointItemViewModel::SetIsValidContentValue(bool newIsValidContentValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(IsValidContentValue, newIsValidContentValue);
}

bool UUDPointItemViewModel::GetIsValidContentValue() const
{
	return IsValidContentValue;
}