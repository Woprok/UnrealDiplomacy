// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDResourcePanelViewModel.h"
#include "Skirmish/UserInterfaces/UDResourceItemViewModel.h"
#include "Skirmish/UDSkirmishHUD.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Core/Simulation/UDModelStructs.h"

#define LOCTEXT_NAMESPACE "ResourcePanel"

void UUDResourcePanelViewModel::Initialize()
{
	ResourceViewModelType = UUDResourceItemViewModel::StaticClass();

	Model->OnDataReloadedEvent.AddUniqueDynamic(this, &UUDResourcePanelViewModel::Reload);
	Model->OnDataChangedEvent.AddUniqueDynamic(this, &UUDResourcePanelViewModel::Update);

	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());

	Update();
}

void UUDResourcePanelViewModel::Reload()
{
	Update();
}

void UUDResourcePanelViewModel::Update()
{
	if (!Model->IsOverseeingStatePresent())
		return;
	if (!Model->IsGamePlayed())
		return;
	// Following updates require model.
	UpdateResourceList();
}

#undef LOCTEXT_NAMESPACE
void UUDResourcePanelViewModel::UpdateResourceList()
{
	UE_LOG(LogTemp, Log, TEXT("UUDResourcePanelViewModel: UpdateResourceList."));
	// Retrieve resources
	TArray<FUDResourceInfo> resources = Model->GetLocalFactionResourceList();
	// Retrieve enough models
	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	TArray<TObjectPtr<UUDViewModel>>& viewModels = hud->GetViewModelCollection(ResourceViewModelCollectionName, ResourceViewModelType, resources.Num());
	// Get rid of all models
	ResourceViewModelCollection.Empty();
	for (int32 i = 0; i < resources.Num(); i++)
	{
		TObjectPtr<UUDResourceItemViewModel> newViewModel = Cast<UUDResourceItemViewModel>(viewModels[i]);
		newViewModel->SetContent(resources[i]);
		newViewModel->Refresh();
		ResourceViewModelCollection.Add(newViewModel);
	}

	SetResourceList(FUDViewModelList(viewModels));
}

void UUDResourcePanelViewModel::SetResourceList(FUDViewModelList newResourceList)
{
	UE_MVVM_SET_PROPERTY_VALUE(ResourceList, newResourceList);
}

FUDViewModelList UUDResourcePanelViewModel::GetResourceList() const
{
	return ResourceList;
}