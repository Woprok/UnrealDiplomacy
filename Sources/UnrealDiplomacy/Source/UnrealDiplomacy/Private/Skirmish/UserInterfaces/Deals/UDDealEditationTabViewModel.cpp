// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDDealEditationTabViewModel.h"
#include "Skirmish/UDSkirmishHUD.h"
#include "Skirmish/UDSkirmishPlayerController.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Core/Simulation/UDModelStructs.h"

#define LOCTEXT_NAMESPACE "DealEditationTab"

void UUDDealEditationTabViewModel::Initialize()
{
	FText dealName = FText(LOCTEXT("DealEditationTab", "Deal"));
	SetDealNameText(dealName);

	Update();
}

void UUDDealEditationTabViewModel::Reload()
{
	Update();
}

void UUDDealEditationTabViewModel::Update()
{
	SetDealNameText(FText::FromString(Content.Name));

	if (!Model->IsOverseeingStatePresent())
		return;
	if (!Model->IsGamePlayed())
		return;
	if (Content.DealId == UUDGlobalData::InvalidDealId)
		return;
	// Following updates require model.
}

#undef LOCTEXT_NAMESPACE

void UUDDealEditationTabViewModel::InvalidateContent(FUDDealMinimalInfo content)
{
	UE_LOG(LogTemp, Log, TEXT("UUDDealEditationTabViewModel: InvalidateContent."));
	Content = content;
}

void UUDDealEditationTabViewModel::SetContent(FUDDealMinimalInfo content)
{
	UE_LOG(LogTemp, Log, TEXT("UUDDealEditationTabViewModel: SetContent."));
	Content = content;
}

void UUDDealEditationTabViewModel::SetDealNameText(FText newDealNameText)
{
	UE_MVVM_SET_PROPERTY_VALUE(DealNameText, newDealNameText);
}

FText UUDDealEditationTabViewModel::GetDealNameText() const
{
	return DealNameText;
}