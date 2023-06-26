// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDDealEditationTabViewModel.h"
#include "Skirmish/UserInterfaces/Deals/UDPointItemViewModel.h"
#include "Skirmish/UDSkirmishHUD.h"
#include "Skirmish/UDSkirmishPlayerController.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Core/Simulation/UDModelStructs.h"

#define LOCTEXT_NAMESPACE "DealEditationTab"

FUDDealPointMinimalInfo GetInvalidPrimaryPoint(int32 dealId)
{
	FUDDealPointMinimalInfo info = FUDDealPointMinimalInfo();
	info.DealId = dealId;
	info.PointId = UUDGlobalData::InvalidDealPointId;
	return info;
}

void UUDDealEditationTabViewModel::Initialize()
{
	Update();
}

void UUDDealEditationTabViewModel::Reload()
{
	Update();
}

void UUDDealEditationTabViewModel::Update()
{
	if (!Model->IsOverseeingStatePresent())
		return;
	if (!Model->IsGamePlayed())
		return;
	if (Content.DealId == UUDGlobalData::InvalidDealId)
		return;
	// Following updates require model.
	UpdatePointList();
}

#undef LOCTEXT_NAMESPACE

void UUDDealEditationTabViewModel::InvalidateContent(FUDDealMinimalInfo content)
{
	UE_LOG(LogTemp, Log, TEXT("UUDDealEditationTabViewModel: InvalidateContent."));
	Content = content;
}

void UUDDealEditationTabViewModel::SetContent(FUDDealMinimalInfo content)
{
	UE_LOG(LogTemp, Log, TEXT("UUDDealEditationTabViewModel: SetContent."));
	Content = content;
}

void UUDDealEditationTabViewModel::UpdatePointList()
{
	UE_LOG(LogTemp, Log, TEXT("UUDDealEditationTabViewModel: UpdatePointList."));
	// Retrieve factions
	TArray<FUDDealPointMinimalInfo> points = Model->GetDealPrimaryPointList(Content.DealId);
	// We will use one additional model for new primary point node.
	int32 desiredPointCount = points.Num() + 1;
	int32 desiredPointIndex = points.Num();
	// Retrieve enough models
	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	TArray<TObjectPtr<UUDViewModel>>& viewModels = hud->GetViewModelCollection(PointItemViewModelCollectionName,
		PointItemViewModelType, desiredPointCount);
	// Get rid of all models
	PointItemViewModelCollection.Empty();
	for (int32 i = 0; i < points.Num(); i++)
	{
		TObjectPtr<UUDPointItemViewModel> newViewModel = Cast<UUDPointItemViewModel>(viewModels[i]);
		newViewModel->SetContent(points[i]);
		newViewModel->FullUpdate();
		PointItemViewModelCollection.Add(newViewModel);
	}
	// Finally add invalid node.
	TObjectPtr<UUDPointItemViewModel> newViewModel = Cast<UUDPointItemViewModel>(viewModels[desiredPointIndex]);
	newViewModel->SetContent(GetInvalidPrimaryPoint(Content.DealId));
	newViewModel->FullUpdate();
	PointItemViewModelCollection.Add(newViewModel);

	PointItemSourceUpdatedEvent.Broadcast(PointItemViewModelCollection);
}