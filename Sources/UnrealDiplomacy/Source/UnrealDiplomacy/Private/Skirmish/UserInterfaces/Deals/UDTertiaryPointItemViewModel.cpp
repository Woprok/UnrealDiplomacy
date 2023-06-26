// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDTertiaryPointItemViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Skirmish/UDSkirmishHUD.h"
#include "Core/Simulation/Actions/UDDealActionPointChildAdd.h"

#define LOCTEXT_NAMESPACE "TertiaryPointItem"

void UUDTertiaryPointItemViewModel::Initialize()
{
	FText createPoint = FText(LOCTEXT("TertiaryPointItem", "+++ Point"));
	SetCreateTertiaryPointText(createPoint);
}

void UUDTertiaryPointItemViewModel::Update()
{
}

#undef LOCTEXT_NAMESPACE

void UUDTertiaryPointItemViewModel::SetContent(FUDDealPointMinimalInfo content, bool isValid)
{
	SetIsValidContentValue(isValid);
	Content = content;
}

void UUDTertiaryPointItemViewModel::CreateTertiaryPoint()
{
	UE_LOG(LogTemp, Log, TEXT("UUDTertiaryPointItemViewModel: CreatePoint."));
	Model->RequestAction(Model->GetAction(UUDDealActionPointChildAdd::ActionTypeId, { Content.DealId, Content.PointId }));
}

void UUDTertiaryPointItemViewModel::SetCreateTertiaryPointText(FText newCreateTertiaryPointText)
{
	UE_MVVM_SET_PROPERTY_VALUE(CreateTertiaryPointText, newCreateTertiaryPointText);
}

FText UUDTertiaryPointItemViewModel::GetCreateTertiaryPointText() const
{
	return CreateTertiaryPointText;
}

void UUDTertiaryPointItemViewModel::SetIsValidContentValue(bool newIsValidContentValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(IsValidContentValue, newIsValidContentValue);
}

bool UUDTertiaryPointItemViewModel::GetIsValidContentValue() const
{
	return IsValidContentValue;
}