// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDSecondaryPointItemViewModel.h"
#include "Skirmish/UserInterfaces/Deals/UDTertiaryPointItemViewModel.h"
#include "Skirmish/UserInterfaces/Deals/UDPointContentViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Skirmish/UDSkirmishHUD.h"
#include "Core/Simulation/Actions/UDDealActionPointChildAdd.h"

#define LOCTEXT_NAMESPACE "SecondaryPointItem"

int32 UUDSecondaryPointItemViewModel::UniqueNameDefinition = 0;

FUDDealPointMinimalInfo GetInvalidTertiaryPoint(int32 dealId, int32 pointId)
{
	FUDDealPointMinimalInfo info = FUDDealPointMinimalInfo();
	info.DealId = dealId;
	info.PointId = pointId;
	return info;
}

void UUDSecondaryPointItemViewModel::Initialize()
{
	if (!IsUniqueNameDefined)
	{
		int32 uniqueId = UUDSecondaryPointItemViewModel::UniqueNameDefinition++;
		PointItemViewModelType = UUDTertiaryPointItemViewModel::StaticClass();
		PointItemViewModelCollectionName = FName(PointItemViewModelCollectionName.ToString() + FString::FromInt(uniqueId));

		PointContentViewModelType = UUDPointContentViewModel::StaticClass();
		PointContentViewModelInstanceName = FName(PointContentViewModelInstanceName.ToString() + FString::FromInt(uniqueId));

		UE_LOG(LogTemp, Log, TEXT("UUDSecondaryPointItemViewModel: Defined points [%d]."), uniqueId);
		IsUniqueNameDefined = true;
	}
	FText createPoint = FText(LOCTEXT("SecondaryPointItem", "++ Point"));
	SetCreateSecondaryPointText(createPoint);
}

void UUDSecondaryPointItemViewModel::Update()
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

void UUDSecondaryPointItemViewModel::SetContent(FUDDealPointMinimalInfo content, bool isValid)
{
	SetIsValidContentValue(isValid);
	Content = content;
}

void UUDSecondaryPointItemViewModel::UpdatePointContent()
{
	UE_LOG(LogTemp, Log, TEXT("UUDSecondaryPointItemViewModel: UpdatePointContent."));
	// Retrieve model
	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	TObjectPtr<UUDViewModel> viewModel = hud->GetViewModelCollection(PointContentViewModelInstanceName, PointContentViewModelType);
	// Get rid of all models
	PointContentViewModelInstance = Cast<UUDPointContentViewModel>(viewModel);
	PointContentViewModelInstance->SetContent(Content);
	PointContentViewModelInstance->FullUpdate();
	PointContentSourceUpdatedEvent.Broadcast(PointContentViewModelInstance);
}

void UUDSecondaryPointItemViewModel::UpdatePointList()
{
	UE_LOG(LogTemp, Log, TEXT("UUDSecondaryPointItemViewModel: UpdatePointList."));
	// Retrieve points
	TArray<FUDDealPointMinimalInfo> points = Model->GetDealTertiaryPointList(Content.DealId, Content.PointId);
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
		TObjectPtr<UUDTertiaryPointItemViewModel> newViewModel = Cast<UUDTertiaryPointItemViewModel>(viewModels[i]);
		newViewModel->SetContent(points[i], true);
		newViewModel->FullUpdate();
		PointItemViewModelCollection.Add(newViewModel);
	}
	// Finally add invalid node.
	TObjectPtr<UUDTertiaryPointItemViewModel> newViewModel = Cast<UUDTertiaryPointItemViewModel>(viewModels[desiredPointIndex]);
	newViewModel->SetContent(GetInvalidTertiaryPoint(Content.DealId, Content.PointId), false);
	newViewModel->FullUpdate();
	PointItemViewModelCollection.Add(newViewModel);

	SetPointItemList(FUDViewModelList(viewModels));
}

void UUDSecondaryPointItemViewModel::CreateSecondaryPoint()
{
	UE_LOG(LogTemp, Log, TEXT("UUDSecondaryPointItemViewModel: CreatePoint."));
	Model->RequestAction(Model->GetAction(UUDDealActionPointChildAdd::ActionTypeId, { Content.DealId, Content.PointId } ));
}

void UUDSecondaryPointItemViewModel::SetCreateSecondaryPointText(FText newCreateSecondaryPointText)
{
	UE_MVVM_SET_PROPERTY_VALUE(CreateSecondaryPointText, newCreateSecondaryPointText);
}

FText UUDSecondaryPointItemViewModel::GetCreateSecondaryPointText() const
{
	return CreateSecondaryPointText;
}

void UUDSecondaryPointItemViewModel::SetIsValidContentValue(bool newIsValidContentValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(IsValidContentValue, newIsValidContentValue);
}

bool UUDSecondaryPointItemViewModel::GetIsValidContentValue() const
{
	return IsValidContentValue;
}

void UUDSecondaryPointItemViewModel::SetPointItemList(FUDViewModelList newPointItemList)
{
	UE_MVVM_SET_PROPERTY_VALUE(PointItemList, newPointItemList);
}

FUDViewModelList UUDSecondaryPointItemViewModel::GetPointItemList() const
{
	return PointItemList;
}