// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDDealGeneralTabViewModel.h"
#include "Skirmish/UDSkirmishHUD.h"
#include "Skirmish/UDSkirmishPlayerController.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Core/Simulation/UDModelStructs.h"

#define LOCTEXT_NAMESPACE "DealGeneralTab"

void UUDDealGeneralTabViewModel::Initialize()
{
	FText dealName = FText(LOCTEXT("DealGeneralTab", "Deal"));
	SetDealNameText(dealName);

	Update();
}

void UUDDealGeneralTabViewModel::Reload()
{
	Update();
}

void UUDDealGeneralTabViewModel::Update()
{
	SetDealNameText(FText::FromString(Content.Name));

	if (!Model->IsOverseeingStatePresent())
		return;
	if (!Model->IsGamePlayed())
		return;
	// Following updates require model.
}

#undef LOCTEXT_NAMESPACE

void UUDDealGeneralTabViewModel::InvalidateContent(FUDDealMinimalInfo content)
{
	UE_LOG(LogTemp, Log, TEXT("UUDDealGeneralTabViewModel: InvalidateContent."));
	Content = content;
	Update();
}

void UUDDealGeneralTabViewModel::SetContent(FUDDealMinimalInfo content)
{
	UE_LOG(LogTemp, Log, TEXT("UUDDealGeneralTabViewModel: SetContent."));
	Content = content;
	Update();
}

void UUDDealGeneralTabViewModel::SetDealNameText(FText newDealNameText)
{
	UE_MVVM_SET_PROPERTY_VALUE(DealNameText, newDealNameText);
}

FText UUDDealGeneralTabViewModel::GetDealNameText() const
{
	return DealNameText;
}