// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDFactionPanelViewModel.h"
#include "Skirmish/UserInterfaces/UDFactionItemViewModel.h"
#include "Skirmish/UDSkirmishHUD.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Core/Simulation/UDModelStructs.h"

#define LOCTEXT_NAMESPACE "FactionPanel"

void UUDFactionPanelViewModel::Initialize()
{
	FactionViewModelType = UUDFactionItemViewModel::StaticClass();

	FText nameHeader = FText(LOCTEXT("FactionPanel", "Faction"));
	SetNameHeaderText(nameHeader);
	FText controllerHeader = FText(LOCTEXT("FactionPanel", "[PC|AI]"));
	SetControllerHeaderText(controllerHeader);

	Model->OnDataReloadedEvent.AddUniqueDynamic(this, &UUDFactionPanelViewModel::Reload);
	Model->OnDataChangedEvent.AddUniqueDynamic(this, &UUDFactionPanelViewModel::Update);

	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());

	Update();
}

void UUDFactionPanelViewModel::Reload()
{
	Update();
}

void UUDFactionPanelViewModel::Update()
{
	if (!Model->IsOverseeingStatePresent())
		return;
	if (!Model->IsGamePlayed())
		return;
	// Following updates require model.
	UpdateFactionList();
}

#undef LOCTEXT_NAMESPACE
void UUDFactionPanelViewModel::UpdateFactionList()
{
	UE_LOG(LogTemp, Log, TEXT("UUDFactionPanelViewModel: UpdateFactionList."));
	// Retrieve factions
	TArray<FUDFactionInfo> factions = Model->GetFactionInfoList();
	// Retrieve enough models
	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	TArray<TObjectPtr<UUDViewModel>>& viewModels = hud->GetViewModelCollection(FactionViewModelCollectionName, FactionViewModelType, factions.Num());
	// Get rid of all models
	FactionViewModelCollection.Empty();
	for (int32 i = 0; i < factions.Num(); i++)
	{
		TObjectPtr<UUDFactionItemViewModel> newViewModel = Cast<UUDFactionItemViewModel>(viewModels[i]);
		newViewModel->SetContent(factions[i]);
		newViewModel->FullUpdate();
		FactionViewModelCollection.Add(newViewModel);
	}

	SetFactionItemList(FUDViewModelList(viewModels));
}

void UUDFactionPanelViewModel::SetNameHeaderText(FText newNameHeaderText)
{
	UE_MVVM_SET_PROPERTY_VALUE(NameHeaderText, newNameHeaderText);
}

FText UUDFactionPanelViewModel::GetNameHeaderText() const
{
	return NameHeaderText;
}

void UUDFactionPanelViewModel::SetControllerHeaderText(FText newControllerHeaderText)
{
	UE_MVVM_SET_PROPERTY_VALUE(ControllerHeaderText, newControllerHeaderText);
}

FText UUDFactionPanelViewModel::GetControllerHeaderText() const
{
	return ControllerHeaderText;
}

void UUDFactionPanelViewModel::SetFactionItemList(FUDViewModelList newFactionItemList)
{
	UE_MVVM_SET_PROPERTY_VALUE(FactionItemList, newFactionItemList);
}

FUDViewModelList UUDFactionPanelViewModel::GetFactionItemList() const
{
	return FactionItemList;
}