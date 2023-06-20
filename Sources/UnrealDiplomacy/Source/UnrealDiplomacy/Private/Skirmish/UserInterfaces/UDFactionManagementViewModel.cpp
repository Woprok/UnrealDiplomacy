// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDFactionManagementViewModel.h"
#include "Skirmish/UserInterfaces/UDFactionInteractionViewModel.h"
#include "Skirmish/UDSkirmishHUD.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Core/Simulation/UDModelStructs.h"

#define LOCTEXT_NAMESPACE "FactionManagement"

void UUDFactionManagementViewModel::Initialize()
{
	FactionInteractionViewModelType = UUDFactionInteractionViewModel::StaticClass();

	FText managementTitle = FText(LOCTEXT("FactionPanel", "Faction Management"));
	SetFactionManagementTitleText(managementTitle);
	FText factionName = FText(LOCTEXT("FactionPanel", "Faction"));
	SetFactionNameText(factionName);
	FText close = FText(LOCTEXT("FactionPanel", "X"));
	SetCloseText(close);


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

	UpdateFactionInteractionList();
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

void UUDFactionManagementViewModel::UpdateFactionInteractionList()
{
	UE_LOG(LogTemp, Log, TEXT("UUDFactionPanelViewModel: UpdateFactionList."));
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
		newViewModel->SetContent(SelectedFactionId, interactions[i]);
		newViewModel->FullUpdate();
		FactionInteractionViewModelCollection.Add(newViewModel);
	}

	FactionInteractionSourceUpdatedEvent.Broadcast(FactionInteractionViewModelCollection);
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