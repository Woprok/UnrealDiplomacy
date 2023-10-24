// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDPolicySelectorViewModel.h"
#include "Skirmish/UserInterfaces/UDPolicySelectItemViewModel.h"
#include "Skirmish/UDSkirmishHUD.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Core/Simulation/UDModelStructs.h"


#define LOCTEXT_NAMESPACE "PolicySelector"

void UUDPolicySelectorViewModel::Initialize()
{
	PolicySelectItemViewModelType = UUDPolicySelectItemViewModel::StaticClass();

	FText policyTitle = FText(LOCTEXT("PolicySelector", "Demand Policy"));
	SetPolicyTitleText(policyTitle);

	Model->OnDataReloadedEvent.AddUniqueDynamic(this, &UUDPolicySelectorViewModel::Reload);
	Model->OnDataChangedEvent.AddUniqueDynamic(this, &UUDPolicySelectorViewModel::Update);

	Update();
}

void UUDPolicySelectorViewModel::Reload()
{
	Update();
}

void UUDPolicySelectorViewModel::Update()
{
	if (!Model->IsOverseeingStatePresent())
		return;
	if (!Model->IsGamePlayed())
		return;
	// Following updates require model.

	UpdatePolicyItemList();
}

#undef LOCTEXT_NAMESPACE

void UUDPolicySelectorViewModel::SetContent()
{
	// TODO create generic version (independent on collectionName and sourceCollection)
}

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
		newViewModel->FullUpdate();
		PolicySelectItemViewModelCollection.Add(newViewModel);
	}

	PolicySelectItemSourceUpdatedEvent.Broadcast(PolicySelectItemViewModelCollection);
}

void UUDPolicySelectorViewModel::SetPolicyTitleText(FText newPolicyTitleText)
{
	UE_MVVM_SET_PROPERTY_VALUE(PolicyTitleText, newPolicyTitleText);
}

FText UUDPolicySelectorViewModel::GetPolicyTitleText() const
{
	return PolicyTitleText;
}