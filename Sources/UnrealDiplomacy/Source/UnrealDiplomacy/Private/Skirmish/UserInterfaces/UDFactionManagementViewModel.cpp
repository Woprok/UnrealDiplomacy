// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDFactionManagementViewModel.h"
#include "Skirmish/UserInterfaces/UDFactionInteractionViewModel.h"
#include "Skirmish/UserInterfaces/UDModifierItemViewModel.h"
#include "Skirmish/UDSkirmishHUD.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/Simulation/UDWorldState.h"

#define LOCTEXT_NAMESPACE "FactionManagement"

void UUDFactionManagementViewModel::Initialize()
{
	FactionInteractionViewModelType = UUDFactionInteractionViewModel::StaticClass();
	ModifierItemViewModelType = UUDModifierItemViewModel::StaticClass();

	FText managementTitle = FText(LOCTEXT("FactionPanel", "Faction Management"));
	SetFactionManagementTitleText(managementTitle);
	FText factionName = FText(LOCTEXT("FactionPanel", "Faction"));
	SetFactionNameText(factionName);
	FText close = FText(LOCTEXT("FactionPanel", "X"));
	SetCloseText(close);
	FText interactionTitle = FText(LOCTEXT("FactionPanel", "Interactions"));
	SetInteractionTitleText(interactionTitle);
	FText offerTitle = FText(LOCTEXT("FactionPanel", "Offers"));
	SetOfferTitleText(offerTitle);
	FText requestTitle = FText(LOCTEXT("FactionPanel", "Requests"));
	SetRequestTitleText(requestTitle);
	FText demandTitle = FText(LOCTEXT("FactionPanel", "Demands"));
	SetDemandTitleText(demandTitle);


	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	hud->OnFactionSelectedEvent.AddUniqueDynamic(this, &UUDFactionManagementViewModel::OnFactionSelected);

	Model->OnDataReloadedEvent.AddUniqueDynamic(this, &UUDFactionManagementViewModel::Reload);
	Model->OnDataChangedEvent.AddUniqueDynamic(this, &UUDFactionManagementViewModel::Update);

	Update();
}

void UUDFactionManagementViewModel::Reload()
{
	Update();
}

void UUDFactionManagementViewModel::Update()
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
	{
		SelectedFactionId = selectedFactionId;
		Update();
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
	TArray<FUDFactionInteractionInfo> interactions = Model->GetFactionInteractionList();
	// Retrieve enough models
	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	TArray<TObjectPtr<UUDViewModel>>& viewModels = hud->GetViewModelCollection(FactionInteractionViewModelCollectionName, FactionInteractionViewModelType, interactions.Num());
	// Get rid of all models
	FactionInteractionViewModelCollection.Empty();
	for (int32 i = 0; i < interactions.Num(); i++)
	{
		TObjectPtr<UUDFactionInteractionViewModel> newViewModel = Cast<UUDFactionInteractionViewModel>(viewModels[i]);
		newViewModel->SetContent(SelectedFactionId, EUDDecisionType::Gift, interactions[i]);
		newViewModel->FullUpdate();
		FactionInteractionViewModelCollection.Add(newViewModel);
	}

	FactionInteractionSourceUpdatedEvent.Broadcast(FactionInteractionViewModelCollection);
}

void UUDFactionManagementViewModel::UpdateFactionOfferList()
{
	// Retrieve factions
	TArray<FUDFactionInteractionInfo> interactions = Model->GetFactionOfferList();
	// Retrieve enough models
	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	TArray<TObjectPtr<UUDViewModel>>& viewModels = hud->GetViewModelCollection(FactionOfferViewModelCollectionName, FactionInteractionViewModelType, interactions.Num());
	// Get rid of all models
	FactionOfferViewModelCollection.Empty();
	for (int32 i = 0; i < interactions.Num(); i++)
	{
		TObjectPtr<UUDFactionInteractionViewModel> newViewModel = Cast<UUDFactionInteractionViewModel>(viewModels[i]);
		newViewModel->SetContent(SelectedFactionId, EUDDecisionType::Offer, interactions[i]);
		newViewModel->FullUpdate();
		FactionOfferViewModelCollection.Add(newViewModel);
	}

	FactionOfferSourceUpdatedEvent.Broadcast(FactionOfferViewModelCollection);
}

void UUDFactionManagementViewModel::UpdateFactionRequestList()
{
	// Retrieve factions
	TArray<FUDFactionInteractionInfo> interactions = Model->GetFactionRequestList();
	// Retrieve enough models
	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	TArray<TObjectPtr<UUDViewModel>>& viewModels = hud->GetViewModelCollection(FactionRequestViewModelCollectionName, FactionInteractionViewModelType, interactions.Num());
	// Get rid of all models
	FactionRequestViewModelCollection.Empty();
	for (int32 i = 0; i < interactions.Num(); i++)
	{
		TObjectPtr<UUDFactionInteractionViewModel> newViewModel = Cast<UUDFactionInteractionViewModel>(viewModels[i]);
		newViewModel->SetContent(SelectedFactionId, EUDDecisionType::Request, interactions[i]);
		newViewModel->FullUpdate();
		FactionRequestViewModelCollection.Add(newViewModel);
	}

	FactionRequestSourceUpdatedEvent.Broadcast(FactionRequestViewModelCollection);
}

void UUDFactionManagementViewModel::UpdateFactionDemandList()
{
	// Retrieve factions
	TArray<FUDFactionInteractionInfo> interactions = Model->GetFactionDemandList();
	// Retrieve enough models
	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	TArray<TObjectPtr<UUDViewModel>>& viewModels = hud->GetViewModelCollection(FactionDemandViewModelCollectionName, FactionInteractionViewModelType, interactions.Num());
	// Get rid of all models
	FactionDemandViewModelCollection.Empty();
	for (int32 i = 0; i < interactions.Num(); i++)
	{
		TObjectPtr<UUDFactionInteractionViewModel> newViewModel = Cast<UUDFactionInteractionViewModel>(viewModels[i]);
		newViewModel->SetContent(SelectedFactionId, EUDDecisionType::Demand, interactions[i]);
		newViewModel->FullUpdate();
		FactionDemandViewModelCollection.Add(newViewModel);
	}

	FactionDemandSourceUpdatedEvent.Broadcast(FactionDemandViewModelCollection);
}

void UUDFactionManagementViewModel::UpdateModifierItemList()
{
	UE_LOG(LogTemp, Log, TEXT("UUDFactionManagementViewModel: UpdateModifierItemList."));
	// Retrieve tiles
	TArray<FUDModifierInfo> modifiers = Model->GetFactionModifierList(SelectedFactionId);
	// Retrieve enough models
	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	TArray<TObjectPtr<UUDViewModel>>& viewModels = hud->GetViewModelCollection(ModifierItemViewModelCollectionName, ModifierItemViewModelType, modifiers.Num());
	// Get rid of all models
	ModifierItemViewModelCollection.Empty();
	for (int32 i = 0; i < modifiers.Num(); i++)
	{
		TObjectPtr<UUDModifierItemViewModel> newViewModel = Cast<UUDModifierItemViewModel>(viewModels[i]);
		newViewModel->SetContent(modifiers[i]);
		newViewModel->FullUpdate();
		ModifierItemViewModelCollection.Add(newViewModel);
	}

	ModifierItemSourceUpdatedEvent.Broadcast(ModifierItemViewModelCollection);
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