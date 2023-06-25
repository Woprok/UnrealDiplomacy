// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDDealExecutionTabViewModel.h"
#include "Skirmish/UDSkirmishHUD.h"
#include "Skirmish/UDSkirmishPlayerController.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Core/Simulation/UDModelStructs.h"

#define LOCTEXT_NAMESPACE "DealExecutionTab"

void UUDDealExecutionTabViewModel::Initialize()
{
	FText dealName = FText(LOCTEXT("DealExecutionTab", "Deal"));
	SetDealNameText(dealName);

	Update();
}

void UUDDealExecutionTabViewModel::Reload()
{
	Update();
}

void UUDDealExecutionTabViewModel::Update()
{
	SetDealNameText(FText::FromString(Content.Name));

	if (!Model->IsOverseeingStatePresent())
		return;
	if (!Model->IsGamePlayed())
		return;
	// Following updates require model.
}

#undef LOCTEXT_NAMESPACE

void UUDDealExecutionTabViewModel::InvalidateContent(FUDDealMinimalInfo content)
{
	UE_LOG(LogTemp, Log, TEXT("UUDDealExecutionTabViewModel: InvalidateContent."));
	Content = content;
}

void UUDDealExecutionTabViewModel::SetContent(FUDDealMinimalInfo content)
{
	UE_LOG(LogTemp, Log, TEXT("UUDDealExecutionTabViewModel: SetContent."));
	Content = content;
}

void UUDDealExecutionTabViewModel::SetDealNameText(FText newDealNameText)
{
	UE_MVVM_SET_PROPERTY_VALUE(DealNameText, newDealNameText);
}

FText UUDDealExecutionTabViewModel::GetDealNameText() const
{
	return DealNameText;
}