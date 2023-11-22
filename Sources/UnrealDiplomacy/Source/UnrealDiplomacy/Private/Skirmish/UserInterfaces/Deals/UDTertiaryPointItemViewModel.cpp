// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDTertiaryPointItemViewModel.h"
#include "Skirmish/UserInterfaces/Deals/UDPointContentViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Skirmish/UDSkirmishHUD.h"
#include "Core/Simulation/Actions/UDDealActionPointChildAdd.h"
#include "Core/Simulation/UDActionData.h"

#define LOCTEXT_NAMESPACE "TertiaryPointItem"

int32 UUDTertiaryPointItemViewModel::UniqueNameDefinition = 0;

void UUDTertiaryPointItemViewModel::Setup()
{
	if (!IsUniqueNameDefined)
	{
		int32 uniqueId = UUDTertiaryPointItemViewModel::UniqueNameDefinition++;

		PointContentViewModelType = UUDPointContentViewModel::StaticClass();
		PointContentViewModelInstanceName = FName(PointContentViewModelInstanceName.ToString() + FString::FromInt(uniqueId));

		UE_LOG(LogTemp, Log, TEXT("UUDSecondaryPointItemViewModel: Defined points [%d]."), uniqueId);
		IsUniqueNameDefined = true;
	}
	FText createPoint = FText(LOCTEXT("TertiaryPointItem", "+++ Point"));
	SetCreateTertiaryPointText(createPoint);

	// Retrieve model
	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	TObjectPtr<UUDViewModel> viewModel = hud->GetViewModelCollection(PointContentViewModelInstanceName, PointContentViewModelType);
	PointContentViewModelInstance = Cast<UUDPointContentViewModel>(viewModel);
	SetPointContent(FUDViewModelContent(PointContentViewModelInstance));
}

void UUDTertiaryPointItemViewModel::Refresh()
{
	if (GetIsValidContentValue())
	{
		// Valid can be updated.
		UpdatePointContent();
	}
}

#undef LOCTEXT_NAMESPACE

void UUDTertiaryPointItemViewModel::SetContent(FUDDealPointMinimalInfo content, bool isValid)
{
	SetIsValidContentValue(isValid);
	Content = content;
}

void UUDTertiaryPointItemViewModel::UpdatePointContent()
{
	UE_LOG(LogTemp, Log, TEXT("UUDTertiaryPointItemViewModel: UpdatePointContent."));
	// Get update on content
	PointContentViewModelInstance->SetContent(Content);
	PointContentViewModelInstance->Refresh();
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

void UUDTertiaryPointItemViewModel::SetPointContent(FUDViewModelContent newPointContent)
{
	UE_MVVM_SET_PROPERTY_VALUE(PointContent, newPointContent);
}

FUDViewModelContent UUDTertiaryPointItemViewModel::GetPointContent() const
{
	return PointContent;
}