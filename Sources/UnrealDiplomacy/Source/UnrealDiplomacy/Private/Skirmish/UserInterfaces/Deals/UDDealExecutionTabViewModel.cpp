// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDDealExecutionTabViewModel.h"
#include "Skirmish/UserInterfaces/Deals/UDActionItemViewModel.h"
#include "Skirmish/UDSkirmishHUD.h"
#include "Skirmish/UDSkirmishPlayerController.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/UDGlobalData.h"

#define LOCTEXT_NAMESPACE "DealExecutionTab"

void UUDDealExecutionTabViewModel::Initialize()
{
	ActionItemViewModelType = UUDActionItemViewModel::StaticClass();
	Update();
}

void UUDDealExecutionTabViewModel::Reload()
{
	Update();
}

void UUDDealExecutionTabViewModel::Update()
{
	if (!Model->IsOverseeingStatePresent())
		return;
	if (!Model->IsGamePlayed())
		return;
	if (Content.DealId == UUDGlobalData::InvalidDealId)
		return;
	// Following updates require model.
	UpdateActionList();
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

void UUDDealExecutionTabViewModel::UpdateActionList()
{
	UE_LOG(LogTemp, Log, TEXT("UUDDealExecutionTabViewModel: UpdateActionList."));
	// Retrieve points
	TArray<FUDDealActionMinimalInfo> actions = Model->GetDealActionList(Content.DealId);
	// Retrieve enough models
	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	TArray<TObjectPtr<UUDViewModel>>& viewModels = hud->GetViewModelCollection(ActionItemViewModelCollectionName,
		ActionItemViewModelType, actions.Num());
	// Get rid of all models
	ActionItemViewModelCollection.Empty();
	for (int32 i = 0; i < actions.Num(); i++)
	{
		TObjectPtr<UUDActionItemViewModel> newViewModel = Cast<UUDActionItemViewModel>(viewModels[i]);
		newViewModel->SetContent(actions[i]);
		newViewModel->Refresh();
		ActionItemViewModelCollection.Add(newViewModel);
	}
	SetActionItemList(FUDViewModelList(viewModels));
}

void UUDDealExecutionTabViewModel::SetActionItemList(FUDViewModelList newActionItemList)
{
	UE_MVVM_SET_PROPERTY_VALUE(ActionItemList, newActionItemList);
}

FUDViewModelList UUDDealExecutionTabViewModel::GetActionItemList() const
{
	return ActionItemList;
}