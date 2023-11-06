// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDPrimaryPointItemViewModel.h"
#include "Skirmish/UserInterfaces/Deals/UDSecondaryPointItemViewModel.h"
#include "Skirmish/UserInterfaces/Deals/UDPointContentViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Skirmish/UDSkirmishHUD.h"
#include "Core/Simulation/Actions/UDDealActionPointAdd.h"
#include "Core/Simulation/UDActionData.h"

#define LOCTEXT_NAMESPACE "PrimaryPointItem"

int32 UUDPrimaryPointItemViewModel::UniqueNameDefinition = 0;

// This is valid data carrier...
FUDDealPointMinimalInfo GetInvalidSecondaryPoint(int32 dealId, int32 pointId)
{
	FUDDealPointMinimalInfo info = FUDDealPointMinimalInfo();
	info.DealId = dealId;
	info.PointId = pointId;
	return info;
}

void UUDPrimaryPointItemViewModel::Initialize()
{
	if (!IsUniqueNameDefined)
	{
		int32 uniqueId = UUDPrimaryPointItemViewModel::UniqueNameDefinition++;
		PointItemViewModelType = UUDSecondaryPointItemViewModel::StaticClass();
		PointItemViewModelCollectionName = FName(PointItemViewModelCollectionName.ToString() + FString::FromInt(uniqueId));

		PointContentViewModelType = UUDPointContentViewModel::StaticClass();
		PointContentViewModelInstanceName = FName(PointContentViewModelInstanceName.ToString() + FString::FromInt(uniqueId));

		UE_LOG(LogTemp, Log, TEXT("UUDPrimaryPointItemViewModel: Defined points [%d]."), uniqueId);
		IsUniqueNameDefined = true;
	}
	FText createPoint = FText(LOCTEXT("PrimaryPointItem", "+ Point"));
	SetCreatePointText(createPoint);
}

void UUDPrimaryPointItemViewModel::Update()
{
	if (GetIsValidContentValue())
	{
		// Valid can already have childs
		UpdatePointList();
		UpdatePointContent();
	}
	else
	{
		PointItemViewModelCollection.Empty();
		SetPointItemList(FUDViewModelList(TArray<TObjectPtr<UUDViewModel>>()));
	}
}

#undef LOCTEXT_NAMESPACE

void UUDPrimaryPointItemViewModel::SetContent(FUDDealPointMinimalInfo content, bool isValid)
{
	SetIsValidContentValue(isValid);
	Content = content;
}

void UUDPrimaryPointItemViewModel::UpdatePointContent()
{
	UE_LOG(LogTemp, Log, TEXT("UUDPrimaryPointItemViewModel: UpdatePointContent."));
	// Retrieve model
	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	TObjectPtr<UUDViewModel> viewModel = hud->GetViewModelCollection(PointContentViewModelInstanceName, PointContentViewModelType);
	PointContentViewModelInstance = Cast<UUDPointContentViewModel>(viewModel);
	PointContentViewModelInstance->SetContent(Content);
	PointContentViewModelInstance->FullUpdate();
	PointContentSourceUpdatedEvent.Broadcast(PointContentViewModelInstance);
}

void UUDPrimaryPointItemViewModel::UpdatePointList()
{
	UE_LOG(LogTemp, Log, TEXT("UUDPrimaryPointItemViewModel: UpdatePointList."));
	// Retrieve points
	TArray<FUDDealPointMinimalInfo> points = Model->GetDealSecondaryPointList(Content.DealId, Content.PointId);
	// We will use one additional model for new point node.
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
		TObjectPtr<UUDSecondaryPointItemViewModel> newViewModel = Cast<UUDSecondaryPointItemViewModel>(viewModels[i]);
		newViewModel->SetContent(points[i], true);
		newViewModel->FullUpdate();
		PointItemViewModelCollection.Add(newViewModel);
	}
	// Finally add invalid node.
	TObjectPtr<UUDSecondaryPointItemViewModel> newViewModel = Cast<UUDSecondaryPointItemViewModel>(viewModels[desiredPointIndex]);
	newViewModel->SetContent(GetInvalidSecondaryPoint(Content.DealId, Content.PointId), false);
	newViewModel->FullUpdate();
	PointItemViewModelCollection.Add(newViewModel);

	SetPointItemList(FUDViewModelList(viewModels));
}

void UUDPrimaryPointItemViewModel::CreatePoint()
{
	UE_LOG(LogTemp, Log, TEXT("UUDPrimaryPointItemViewModel: CreatePoint."));
	Model->RequestAction(Model->GetAction(UUDDealActionPointAdd::ActionTypeId, { Content.DealId }));
}

void UUDPrimaryPointItemViewModel::SetCreatePointText(FText newCreatePointText)
{
	UE_MVVM_SET_PROPERTY_VALUE(CreatePointText, newCreatePointText);
}

FText UUDPrimaryPointItemViewModel::GetCreatePointText() const
{
	return CreatePointText;
}

void UUDPrimaryPointItemViewModel::SetIsValidContentValue(bool newIsValidContentValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(IsValidContentValue, newIsValidContentValue);
}

bool UUDPrimaryPointItemViewModel::GetIsValidContentValue() const
{
	return IsValidContentValue;
}

void UUDPrimaryPointItemViewModel::SetPointItemList(FUDViewModelList newPointItemList)
{
	UE_MVVM_SET_PROPERTY_VALUE(PointItemList, newPointItemList);
}

FUDViewModelList UUDPrimaryPointItemViewModel::GetPointItemList() const
{
	return PointItemList;
}