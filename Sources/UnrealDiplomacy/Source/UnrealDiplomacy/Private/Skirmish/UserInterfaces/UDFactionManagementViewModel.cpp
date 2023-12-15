// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDFactionManagementViewModel.h"
#include "Skirmish/UserInterfaces/UDFactionInteractionViewModel.h"
#include "Skirmish/UserInterfaces/UDModifierItemViewModel.h"
#include "Skirmish/UserInterfaces/UDPolicySelectorViewModel.h"
#include "Skirmish/UDSkirmishHUD.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/Simulation/UDWorldState.h"

#define LOCTEXT_NAMESPACE "FactionManagement"

void UUDFactionManagementViewModel::Setup()
{
	FactionInteractionViewModelType = UUDFactionInteractionViewModel::StaticClass();
	ModifierItemViewModelType = UUDModifierItemViewModel::StaticClass();
	PolicySelectorType = UUDPolicySelectorViewModel::StaticClass();

	FText managementTitle = FText(LOCTEXT("FactionManagement", "Faction Management"));
	SetFactionManagementTitleText(managementTitle);
	FText factionName = FText(LOCTEXT("FactionManagement", "Faction"));
	SetFactionNameText(factionName);
	FText close = FText(LOCTEXT("FactionManagement", "X"));
	SetCloseText(close);
	FText interactionTitle = FText(LOCTEXT("FactionManagement", "Interactions"));
	SetInteractionTitleText(interactionTitle);
	FText offerTitle = FText(LOCTEXT("FactionManagement", "Offers"));
	SetOfferTitleText(offerTitle);
	FText requestTitle = FText(LOCTEXT("FactionManagement", "Requests"));
	SetRequestTitleText(requestTitle);
	FText demandTitle = FText(LOCTEXT("FactionManagement", "Demands"));
	SetDemandTitleText(demandTitle);


	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	hud->OnFactionSelectedEvent.AddUniqueDynamic(this, &UUDFactionManagementViewModel::OnFactionSelected);

	Model->OnDataReloadedEvent.AddUniqueDynamic(this, &UUDFactionManagementViewModel::Refresh);
	Model->OnDataChangedEvent.AddUniqueDynamic(this, &UUDFactionManagementViewModel::Refresh);


	// Retrieve view models for sub content controls
	TObjectPtr<UUDViewModel> policySelectorModel = hud->GetViewModelCollection(PolicySelectorInstanceName, PolicySelectorType);
	PolicySelectorInstance = Cast<UUDPolicySelectorViewModel>(policySelectorModel);
	// Announce them to widget for additional binding.
	SetPolicySelectorContent(FUDViewModelContent(PolicySelectorInstance));
}

void UUDFactionManagementViewModel::Refresh()
{
	if (!Model->IsOverseeingStatePresent())
		return;
	if (!Model->IsGamePlayed())
		return;
	// This should show only informations on players, ignoring observers and server faction.
	if (!Model->IsFactionPlayerControlled(SelectedFactionId))
		return;
	// Following updates require model.
	FUDFactionMinimalInfo faction = Model->GetFactionInfo(SelectedFactionId);
	SetFactionNameText(FText::FromString(faction.Name));

	// Call initialize so each Instance is ready to use, once it receives data in runtime.
	// Policy needs to be refreshed after update, in case the user change the selected option.
	PolicySelectorInstance->Refresh();

	UpdateFactionLists();
	UpdateModifierItemList();
}

#undef LOCTEXT_NAMESPACE

void UUDFactionManagementViewModel::Close()
{
	UE_LOG(LogTemp, Log, TEXT("UUDFactionManagementViewModel: Close."));
	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	hud->HideWidget(hud->FactionManagementWidget);
}

void UUDFactionManagementViewModel::OnFactionSelected(int32 selectedFactionId)
{
	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	hud->ShowWidget(hud->FactionManagementWidget);

	//if (SelectedFactionId != selectedFactionId)
	// This has to update the content, the if might be possible to uncomment if other bugs are fixed
	// As this is change by other interaction, this has to get immediate update to update current content
	// TODO verify validity of this comment.
	{
		// This is basically SetContent by user
		SelectedFactionId = selectedFactionId;
		// Thus it is followed by Refresh
		Refresh();
	}
}

void UUDFactionManagementViewModel::UpdateFactionLists()
{
	UE_LOG(LogTemp, Log, TEXT("UUDFactionPanelViewModel: UpdateFactionLists."));
	UpdateFactionInteractionList();
	UpdateFactionOfferList();
	UpdateFactionRequestList();
	UpdateFactionDemandList();
}

void UUDFactionManagementViewModel::UpdateFactionInteractionList()
{
	// Retrieve factions
	TArray<FUDFactionInteractionInfo> interactions = {};
	if (!Model->IsLocalPlayer(SelectedFactionId))
		interactions = Model->GetFactionInteractionList(Model->GetLocalPlayerFaction());

	// Retrieve enough models
	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	TArray<TObjectPtr<UUDViewModel>> viewModels = hud->GetViewModelCollection(FactionInteractionViewModelCollectionName, FactionInteractionViewModelType, interactions.Num());
	// Get rid of all models
	FactionInteractionViewModelCollection.Empty();
	for (int32 i = 0; i < interactions.Num(); i++)
	{
		TObjectPtr<UUDFactionInteractionViewModel> newViewModel = Cast<UUDFactionInteractionViewModel>(viewModels[i]);
		newViewModel->SetContent(SelectedFactionId, EUDDecisionType::Gift, interactions[i]);
		newViewModel->Refresh();
		FactionInteractionViewModelCollection.Add(newViewModel);
	}

	SetFactionInteractionList(FUDViewModelList(viewModels));
}

void UUDFactionManagementViewModel::UpdateFactionOfferList()
{
	// Retrieve factions
	TArray<FUDFactionInteractionInfo> interactions = {};
	if (!Model->IsLocalPlayer(SelectedFactionId))
		interactions = Model->GetFactionOfferList(Model->GetLocalPlayerFaction());

	// Retrieve enough models
	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	TArray<TObjectPtr<UUDViewModel>> viewModels = hud->GetViewModelCollection(FactionOfferViewModelCollectionName, FactionInteractionViewModelType, interactions.Num());
	// Get rid of all models
	FactionOfferViewModelCollection.Empty();
	for (int32 i = 0; i < interactions.Num(); i++)
	{
		TObjectPtr<UUDFactionInteractionViewModel> newViewModel = Cast<UUDFactionInteractionViewModel>(viewModels[i]);
		newViewModel->SetContent(SelectedFactionId, EUDDecisionType::Offer, interactions[i]);
		newViewModel->Refresh();
		FactionOfferViewModelCollection.Add(newViewModel);
	}

	SetFactionOfferList(FUDViewModelList(viewModels));
}

void UUDFactionManagementViewModel::UpdateFactionRequestList()
{
	// Retrieve factions
	TArray<FUDFactionInteractionInfo> interactions = {};
	if (!Model->IsLocalPlayer(SelectedFactionId))
		interactions = Model->GetFactionRequestList(SelectedFactionId);

	// Retrieve enough models
	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	TArray<TObjectPtr<UUDViewModel>> viewModels = hud->GetViewModelCollection(FactionRequestViewModelCollectionName, FactionInteractionViewModelType, interactions.Num());
	// Get rid of all models
	FactionRequestViewModelCollection.Empty();
	for (int32 i = 0; i < interactions.Num(); i++)
	{
		TObjectPtr<UUDFactionInteractionViewModel> newViewModel = Cast<UUDFactionInteractionViewModel>(viewModels[i]);
		newViewModel->SetContent(SelectedFactionId, EUDDecisionType::Request, interactions[i]);
		newViewModel->Refresh();
		FactionRequestViewModelCollection.Add(newViewModel);
	}

	SetFactionRequestList(FUDViewModelList(viewModels));
}

void UUDFactionManagementViewModel::UpdateFactionDemandList()
{
	// Retrieve factions
	TArray<FUDFactionInteractionInfo> interactions = {};
	if (!Model->IsLocalPlayer(SelectedFactionId))
		interactions = Model->GetFactionDemandList(SelectedFactionId);

	// Retrieve enough models
	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	TArray<TObjectPtr<UUDViewModel>> viewModels = hud->GetViewModelCollection(FactionDemandViewModelCollectionName, FactionInteractionViewModelType, interactions.Num());
	// Get rid of all models
	FactionDemandViewModelCollection.Empty();
	for (int32 i = 0; i < interactions.Num(); i++)
	{
		TObjectPtr<UUDFactionInteractionViewModel> newViewModel = Cast<UUDFactionInteractionViewModel>(viewModels[i]);
		newViewModel->SetContent(SelectedFactionId, EUDDecisionType::Demand, interactions[i]);
		newViewModel->Refresh();
		FactionDemandViewModelCollection.Add(newViewModel);
	}

	SetFactionDemandList(FUDViewModelList(viewModels));
}

void UUDFactionManagementViewModel::UpdateModifierItemList()
{
	UE_LOG(LogTemp, Log, TEXT("UUDFactionManagementViewModel: UpdateModifierItemList."));
	// Retrieve tiles
	TArray<FUDModifierInfo> modifiers = Model->GetFactionModifierList(SelectedFactionId);
	// Retrieve enough models
	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	TArray<TObjectPtr<UUDViewModel>> viewModels = hud->GetViewModelCollection(ModifierItemViewModelCollectionName, ModifierItemViewModelType, modifiers.Num());
	// Get rid of all models
	ModifierItemViewModelCollection.Empty();
	for (int32 i = 0; i < modifiers.Num(); i++)
	{
		TObjectPtr<UUDModifierItemViewModel> newViewModel = Cast<UUDModifierItemViewModel>(viewModels[i]);
		newViewModel->SetContent(modifiers[i]);
		newViewModel->Refresh();
		ModifierItemViewModelCollection.Add(newViewModel);
	}

	SetModifierItemList(FUDViewModelList(viewModels));
}

void UUDFactionManagementViewModel::SetFactionManagementTitleText(FText newFactionManagementTitleText)
{
	UE_MVVM_SET_PROPERTY_VALUE(FactionManagementTitleText, newFactionManagementTitleText);
}

FText UUDFactionManagementViewModel::GetFactionManagementTitleText() const
{
	return FactionManagementTitleText;
}

void UUDFactionManagementViewModel::SetFactionNameText(FText newFactionNameText)
{
	UE_MVVM_SET_PROPERTY_VALUE(FactionNameText, newFactionNameText);
}

FText UUDFactionManagementViewModel::GetFactionNameText() const
{
	return FactionNameText;
}

void UUDFactionManagementViewModel::SetCloseText(FText newCloseText)
{
	UE_MVVM_SET_PROPERTY_VALUE(CloseText, newCloseText);
}

FText UUDFactionManagementViewModel::GetCloseText() const
{
	return CloseText;
}

void UUDFactionManagementViewModel::SetInteractionTitleText(FText newInteractionTitleText)
{
	UE_MVVM_SET_PROPERTY_VALUE(InteractionTitleText, newInteractionTitleText);
}

FText UUDFactionManagementViewModel::GetInteractionTitleText() const
{
	return InteractionTitleText;
}

void UUDFactionManagementViewModel::SetOfferTitleText(FText newOfferTitleText)
{
	UE_MVVM_SET_PROPERTY_VALUE(OfferTitleText, newOfferTitleText);
}

FText UUDFactionManagementViewModel::GetOfferTitleText() const
{
	return OfferTitleText;
}

void UUDFactionManagementViewModel::SetRequestTitleText(FText newRequestTitleText)
{
	UE_MVVM_SET_PROPERTY_VALUE(RequestTitleText, newRequestTitleText);
}

FText UUDFactionManagementViewModel::GetRequestTitleText() const
{
	return RequestTitleText;
}

void UUDFactionManagementViewModel::SetDemandTitleText(FText newDemandTitleText)
{
	UE_MVVM_SET_PROPERTY_VALUE(DemandTitleText, newDemandTitleText);
}

FText UUDFactionManagementViewModel::GetDemandTitleText() const
{
	return DemandTitleText;
}

void UUDFactionManagementViewModel::SetFactionInteractionList(FUDViewModelList newFactionInteractionList)
{
	UE_MVVM_SET_PROPERTY_VALUE(FactionInteractionList, newFactionInteractionList);
}

FUDViewModelList UUDFactionManagementViewModel::GetFactionInteractionList() const
{
	return FactionInteractionList;
}

void UUDFactionManagementViewModel::SetFactionOfferList(FUDViewModelList newFactionOfferList)
{
	UE_MVVM_SET_PROPERTY_VALUE(FactionOfferList, newFactionOfferList);
}

FUDViewModelList UUDFactionManagementViewModel::GetFactionOfferList() const
{
	return FactionOfferList;
}

void UUDFactionManagementViewModel::SetFactionRequestList(FUDViewModelList newFactionRequestList)
{
	UE_MVVM_SET_PROPERTY_VALUE(FactionRequestList, newFactionRequestList);
}

FUDViewModelList UUDFactionManagementViewModel::GetFactionRequestList() const
{
	return FactionRequestList;
}

void UUDFactionManagementViewModel::SetFactionDemandList(FUDViewModelList newFactionDemandList)
{
	UE_MVVM_SET_PROPERTY_VALUE(FactionDemandList, newFactionDemandList);
}

FUDViewModelList UUDFactionManagementViewModel::GetFactionDemandList() const
{
	return FactionDemandList;
}

void UUDFactionManagementViewModel::SetModifierItemList(FUDViewModelList newModifierItemList)
{
	UE_MVVM_SET_PROPERTY_VALUE(ModifierItemList, newModifierItemList);
}

FUDViewModelList UUDFactionManagementViewModel::GetModifierItemList() const
{
	return ModifierItemList;
}

void UUDFactionManagementViewModel::SetPolicySelectorContent(FUDViewModelContent newPolicySelectorContent)
{
	UE_MVVM_SET_PROPERTY_VALUE(PolicySelectorContent, newPolicySelectorContent);
}

FUDViewModelContent UUDFactionManagementViewModel::GetPolicySelectorContent() const
{
	return PolicySelectorContent;
}