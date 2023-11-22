// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDTileManagementViewModel.h"
#include "Skirmish/UserInterfaces/UDTileInteractionViewModel.h"
#include "Skirmish/UserInterfaces/UDModifierItemViewModel.h"
#include "Skirmish/UDSkirmishHUD.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Core/Simulation/UDModelStructs.h"

#define LOCTEXT_NAMESPACE "TileManagement"

void UUDTileManagementViewModel::Setup()
{
	TileInteractionViewModelType = UUDTileInteractionViewModel::StaticClass();
	ModifierItemViewModelType = UUDModifierItemViewModel::StaticClass();

	FText managementTitle = FText(LOCTEXT("TilePanel", "Tile Management"));
	SetTileManagementTitleText(managementTitle);
	FText tileName = FText(LOCTEXT("TilePanel", "Tile"));
	SetTileNameText(tileName);
	FText owner = FText(LOCTEXT("TilePanel", "Current Owner"));
	SetOwnerText(owner);
	FText factionName = FText(LOCTEXT("TilePanel", "Faction"));
	SetFactionNameText(factionName);
	FText resource = FText(LOCTEXT("TilePanel", "Resource Left"));
	SetResourceText(resource);
	FText resourceValue = FText::Format(LOCTEXT("TilePanel", "{0}"), 0);
	SetResourceValueText(resourceValue);
	FText resourceType = FText(LOCTEXT("TilePanel", "Type"));
	SetResourceTypeText(resourceType);
	FText close = FText(LOCTEXT("TilePanel", "X"));
	SetCloseText(close);


	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	hud->OnTileSelectedEvent.AddUniqueDynamic(this, &UUDTileManagementViewModel::OnTileSelected);

	Model->OnDataReloadedEvent.AddUniqueDynamic(this, &UUDTileManagementViewModel::Refresh);
	Model->OnDataChangedEvent.AddUniqueDynamic(this, &UUDTileManagementViewModel::Refresh);

	// TODO delete this comment if it works as expected...
	//Update();
}

void UUDTileManagementViewModel::Refresh()
{
	if (!Model->IsOverseeingStatePresent())
		return;
	if (!Model->IsGamePlayed())
		return;
	// Following updates require model.
	// TODO check that tile was selected...
	FUDTileInfo tile = Model->GetTileInfo(SelectedTile);
	SetTileNameText(FText::FromString(tile.Name));
	SetFactionNameText(FText::FromString(tile.FactionName));
	FText resourceValue = FText::Format(LOCTEXT("TilePanel", "{0}"), tile.ResourceAmount);
	SetResourceValueText(resourceValue);
	SetResourceTypeText(FText::FromString(tile.ResourceTypeName));

	UpdateTileInteractionList();
	UpdateModifierItemList();
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
	// This has to update the content, the if might be possible to uncomment if other bugs are fixed
	// As this is change by other interaction, this has to get immediate update to update current content
	// TODO verify validity of this comment.
	{
		// This is basically SetContent by user
		SelectedTile = selectedTile;
		// Thus it is followed by Refresh
		Refresh();
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
		newViewModel->Refresh();
		TileInteractionViewModelCollection.Add(newViewModel);
	}

	SetTileInteractionList(FUDViewModelList(viewModels));
}

void UUDTileManagementViewModel::UpdateModifierItemList()
{
	UE_LOG(LogTemp, Log, TEXT("UUDTilePanelViewModel: UpdateModifierItemList."));
	// Retrieve tiles
	TArray<FUDModifierInfo> modifiers = Model->GetTileModifierList(SelectedTile);
	// Retrieve enough models
	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	TArray<TObjectPtr<UUDViewModel>>& viewModels = hud->GetViewModelCollection(ModifierItemViewModelCollectionName, ModifierItemViewModelType, modifiers.Num());
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

void UUDTileManagementViewModel::SetOwnerText(FText newOwnerText)
{
	UE_MVVM_SET_PROPERTY_VALUE(OwnerText, newOwnerText);
}

FText UUDTileManagementViewModel::GetOwnerText() const
{
	return OwnerText;
}

void UUDTileManagementViewModel::SetFactionNameText(FText newFactionNameText)
{
	UE_MVVM_SET_PROPERTY_VALUE(FactionNameText, newFactionNameText);
}

FText UUDTileManagementViewModel::GetFactionNameText() const
{
	return FactionNameText;
}

void UUDTileManagementViewModel::SetResourceText(FText newResourceText)
{
	UE_MVVM_SET_PROPERTY_VALUE(ResourceText, newResourceText);
}

FText UUDTileManagementViewModel::GetResourceText() const
{
	return ResourceText;
}

void UUDTileManagementViewModel::SetResourceTypeText(FText newResourceTypeText)
{
	UE_MVVM_SET_PROPERTY_VALUE(ResourceTypeText, newResourceTypeText);
}

FText UUDTileManagementViewModel::GetResourceTypeText() const
{
	return ResourceTypeText;
}

void UUDTileManagementViewModel::SetResourceValueText(FText newResourceValueText)
{
	UE_MVVM_SET_PROPERTY_VALUE(ResourceValueText, newResourceValueText);
}

FText UUDTileManagementViewModel::GetResourceValueText() const
{
	return ResourceValueText;
}

void UUDTileManagementViewModel::SetCloseText(FText newCloseText)
{
	UE_MVVM_SET_PROPERTY_VALUE(CloseText, newCloseText);
}

FText UUDTileManagementViewModel::GetCloseText() const
{
	return CloseText;
}

void UUDTileManagementViewModel::SetTileInteractionList(FUDViewModelList newTileInteractionList)
{
	UE_MVVM_SET_PROPERTY_VALUE(TileInteractionList, newTileInteractionList);
}

FUDViewModelList UUDTileManagementViewModel::GetTileInteractionList() const
{
	return TileInteractionList;
}

void UUDTileManagementViewModel::SetModifierItemList(FUDViewModelList newModifierItemList)
{
	UE_MVVM_SET_PROPERTY_VALUE(ModifierItemList, newModifierItemList);
}

FUDViewModelList UUDTileManagementViewModel::GetModifierItemList() const
{
	return ModifierItemList;
}