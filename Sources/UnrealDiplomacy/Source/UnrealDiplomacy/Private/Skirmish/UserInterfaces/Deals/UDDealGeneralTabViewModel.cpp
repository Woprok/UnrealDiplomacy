// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDDealItemViewModel.h"
#include "Skirmish/UDSkirmishHUD.h"
#include "Skirmish/UDSkirmishPlayerController.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Core/Simulation/UDModelStructs.h"

#define LOCTEXT_NAMESPACE "DealItem"

void UUDDealItemViewModel::Initialize()
{
	FText dealName = FText(LOCTEXT("DealItem", "Deal"));
	SetDealNameText(dealName);

	Update();
}

void UUDDealItemViewModel::Reload()
{
	Update();
}

void UUDDealItemViewModel::Update()
{
	SetDealNameText(FText::FromString(Content.Name));

	if (!Model->IsOverseeingStatePresent())
		return;
	if (!Model->IsGamePlayed())
		return;
	// Following updates require model.
}

#undef LOCTEXT_NAMESPACE

void UUDDealItemViewModel::InvalidateContent(FUDDealMinimalInfo content)
{
	UE_LOG(LogTemp, Log, TEXT("UUDDealItemViewModel: InvalidateContent."));
	Content = content;
	Update();
}

void UUDDealItemViewModel::SetContent(FUDDealMinimalInfo content)
{
	UE_LOG(LogTemp, Log, TEXT("UUDDealItemViewModel: SetContent."));
	Content = content;
	Update();
}

void UUDDealItemViewModel::SetDealNameText(FText newDealNameText)
{
	UE_MVVM_SET_PROPERTY_VALUE(DealNameText, newDealNameText);
}

FText UUDDealItemViewModel::GetDealNameText() const
{
	return DealNameText;
}