// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDTileManagementViewModel.h"
#include "Skirmish/UserInterfaces/UDTileInteractionViewModel.h"
#include "Skirmish/UDSkirmishHUD.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Core/Simulation/UDModelStructs.h"

#define LOCTEXT_NAMESPACE "TileManagement"

void UUDTileManagementViewModel::Initialize()
{
	TileInteractionViewModelType = UUDTileInteractionViewModel::StaticClass();

	FText managementTitle = FText(LOCTEXT("TilePanel", "Tile Management"));
	SetTileManagementTitleText(managementTitle);
	FText tileName = FText(LOCTEXT("TilePanel", "Tile"));
	SetTileNameText(tileName);
	FText factionName = FText(LOCTEXT("TilePanel", "Faction"));
	SetFactionNameText(factionName);
	FText close = FText(LOCTEXT("TilePanel", "X"));
	SetCloseText(close);


	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	hud->OnTileSelectedEvent.AddUniqueDynamic(this, &UUDTileManagementViewModel::OnTileSelected);

	Model->OnDataReloadedEvent.AddUniqueDynamic(this, &UUDTileManagementViewModel::Reload);
	Model->OnDataChangedEvent.AddUniqueDynamic(this, &UUDTileManagementViewModel::Update);

	Update();
}

void UUDTileManagementViewModel::Reload()
{
	Update();
}

void UUDTileManagementViewModel::Update()
{
	if (!Model->IsOverseeingStatePresent())
		return;
	if (!Model->IsGamePlayed())
		return;
	// Following updates require model.
	FUDTileInfo tile = Model->GetTileInfo(SelectedTile);
	SetTileNameText(FText::FromString(tile.Name));
	SetFactionNameText(FText::FromString(tile.FactionName));

	UpdateTileInteractionList();
}

#undef LOCTEXT_NAMESPACE

void UUDTileManagementViewModel::Close()
{
	UE_LOG(LogTemp, Log, TEXT("UUDTileManagementViewModel: Close."));
	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	hud->HideWidget(hud->TileManagementWidget);
}

void UUDTileManagementViewModel::OnTileSelected(FIntPoint selectedTile)
{
	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	hud->ShowWidget(hud->TileManagementWidget);

	//if (SelectedTile != selectedTile)
	{
		SelectedTile = selectedTile;
		Update();
	}
}

void UUDTileManagementViewModel::UpdateTileInteractionList()
{
	UE_LOG(LogTemp, Log, TEXT("UUDTilePanelViewModel: UpdateTileList."));
	// Retrieve tiles
	TArray<FUDTileInteractionInfo> interactions = Model->GetTileInteractionList();
	// Retrieve enough models
	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	TArray<TObjectPtr<UUDViewModel>>& viewModels = hud->GetViewModelCollection(TileInteractionViewModelCollectionName, TileInteractionViewModelType, interactions.Num());
	// Get rid of all models
	TileInteractionViewModelCollection.Empty();
	for (int32 i = 0; i < interactions.Num(); i++)
	{
		TObjectPtr<UUDTileInteractionViewModel> newViewModel = Cast<UUDTileInteractionViewModel>(viewModels[i]);
		newViewModel->SetContent(SelectedTile, interactions[i]);
		newViewModel->FullUpdate();
		TileInteractionViewModelCollection.Add(newViewModel);
	}

	TileInteractionSourceUpdatedEvent.Broadcast(TileInteractionViewModelCollection);
}

void UUDTileManagementViewModel::SetTileManagementTitleText(FText newTileManagementTitleText)
{
	UE_MVVM_SET_PROPERTY_VALUE(TileManagementTitleText, newTileManagementTitleText);
}

FText UUDTileManagementViewModel::GetTileManagementTitleText() const
{
	return TileManagementTitleText;
}

void UUDTileManagementViewModel::SetTileNameText(FText newTileNameText)
{
	UE_MVVM_SET_PROPERTY_VALUE(TileNameText, newTileNameText);
}

FText UUDTileManagementViewModel::GetTileNameText() const
{
	return TileNameText;
}

void UUDTileManagementViewModel::SetFactionNameText(FText newFactionNameText)
{
	UE_MVVM_SET_PROPERTY_VALUE(FactionNameText, newFactionNameText);
}

FText UUDTileManagementViewModel::GetFactionNameText() const
{
	return FactionNameText;
}

void UUDTileManagementViewModel::SetCloseText(FText newCloseText)
{
	UE_MVVM_SET_PROPERTY_VALUE(CloseText, newCloseText);
}

FText UUDTileManagementViewModel::GetCloseText() const
{
	return CloseText;
}