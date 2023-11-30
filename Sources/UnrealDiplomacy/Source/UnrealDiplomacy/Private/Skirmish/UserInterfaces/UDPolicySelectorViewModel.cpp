// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDPolicySelectorViewModel.h"
#include "Skirmish/UserInterfaces/UDPolicySelectItemViewModel.h"
#include "Skirmish/UDSkirmishHUD.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Core/Simulation/UDModelStructs.h"

#define LOCTEXT_NAMESPACE "PolicySelector"

void UUDPolicySelectorViewModel::Setup()
{
	PolicySelectItemViewModelType = UUDPolicySelectItemViewModel::StaticClass();

	FText policyTitle = FText(LOCTEXT("PolicySelector", "Demand Policy"));
	SetPolicyTitleText(policyTitle);

	// TODO This is not undependent, check and remove the commented part later.
	//Model->OnDataReloadedEvent.AddUniqueDynamic(this, &UUDPolicySelectorViewModel::Reload);
	//Model->OnDataChangedEvent.AddUniqueDynamic(this, &UUDPolicySelectorViewModel::Update);
	//
	//Update();
}

void UUDPolicySelectorViewModel::Refresh()
{
	if (!Model->IsOverseeingStatePresent())
		return;
	if (!Model->IsGamePlayed())
		return;
	// Following updates require model.

	UpdatePolicyItemList();
}

#undef LOCTEXT_NAMESPACE

void UUDPolicySelectorViewModel::UpdatePolicyItemList()
{
	// Retrieve factions
	TArray<FUDPolicySelectItemInfo> policies = Model->GetConsequencePolicyList();
	// Retrieve enough models
	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	TArray<TObjectPtr<UUDViewModel>>& viewModels = hud->GetViewModelCollection(PolicySelectItemViewModelCollectionName, PolicySelectItemViewModelType, policies.Num());
	// Get rid of all models
	PolicySelectItemViewModelCollection.Empty();
	for (int32 i = 0; i < policies.Num(); i++)
	{
		TObjectPtr<UUDPolicySelectItemViewModel> newViewModel = Cast<UUDPolicySelectItemViewModel>(viewModels[i]);
		newViewModel->SetContent(policies[i]);
		newViewModel->Refresh();
		PolicySelectItemViewModelCollection.Add(newViewModel);
	}

	SetPolicyItemList(FUDViewModelList(viewModels));
}

void UUDPolicySelectorViewModel::SetPolicyTitleText(FText newPolicyTitleText)
{
	UE_MVVM_SET_PROPERTY_VALUE(PolicyTitleText, newPolicyTitleText);
}

FText UUDPolicySelectorViewModel::GetPolicyTitleText() const
{
	return PolicyTitleText;
}

void UUDPolicySelectorViewModel::SetPolicyItemList(FUDViewModelList newPolicyItemList)
{
	UE_MVVM_SET_PROPERTY_VALUE(PolicyItemList, newPolicyItemList);
}

FUDViewModelList UUDPolicySelectorViewModel::GetPolicyItemList() const
{
	return PolicyItemList;
}