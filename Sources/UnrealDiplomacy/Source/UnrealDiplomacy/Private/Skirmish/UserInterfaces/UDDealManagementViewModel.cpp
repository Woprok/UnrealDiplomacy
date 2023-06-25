// Copyright Miroslav Valach
// TODO slight refactoring to reduce duplication caused by separate history and active ?

#include "Skirmish/UserInterfaces/UDDealManagementViewModel.h"
#include "Skirmish/UserInterfaces/Deals/UDDealItemViewModel.h"
#include "Skirmish/UDSkirmishHUD.h"
#include "Skirmish/UDSkirmishPlayerController.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/Simulation/Actions/UDDealActionCreate.h"

#define LOCTEXT_NAMESPACE "DealManagement"

FUDDealMinimalInfo GetInvalidDeal(bool getActive)
{
	FUDDealMinimalInfo info;
	info.DealId = UUDGlobalData::InvalidDealId;
	if (getActive)
		info.Name = FText(LOCTEXT("DealItem", "No Active Deals")).ToString();
	else
		info.Name = FText(LOCTEXT("DealItem", "No Historic Deals")).ToString();
	return info;
}

void UUDDealManagementViewModel::Initialize()
{
	DealItemType = UUDDealItemViewModel::StaticClass();

	FText dealTitle = FText(LOCTEXT("DealManagement", "Deal Management"));
	SetDealManagementTitleText(dealTitle);
	FText dealCount = FText::Format(LOCTEXT("DealManagement", "{0}/{1}"), 0, 0);
	SetDealCountText(dealCount);

	FText close = FText(LOCTEXT("DealManagement", "X"));
	SetCloseText(close);
	FText first = FText(LOCTEXT("DealManagement", "|<<"));
	SetFirstText(first);
	FText previous = FText(LOCTEXT("DealManagement", "<"));
	SetPreviousText(previous);
	FText next = FText(LOCTEXT("DealManagement", ">"));
	SetNextText(next);
	FText last = FText(LOCTEXT("DealManagement", ">>|"));
	SetLastText(last);
	FText create = FText(LOCTEXT("DealManagement", "Create"));
	SetCreateText(create);
	FText createToolTip = FText(LOCTEXT("DealManagement", "Regent can create one deal per regency."));
	SetCreateToolTipText(createToolTip);
	FText active = FText(LOCTEXT("DealManagement", "Active"));
	SetActiveText(active);
	FText history = FText(LOCTEXT("DealManagement", "History"));
	SetHistoryText(history);

	SelectedActiveIndex = UUDGlobalData::InvalidArrayIndex;
	SelectedHistoryIndex = UUDGlobalData::InvalidArrayIndex;
	SelectedActiveDealItem = GetInvalidDeal(true);
	SelectedHistoryDealItem = GetInvalidDeal(false);

	Model->OnDataReloadedEvent.AddUniqueDynamic(this, &UUDDealManagementViewModel::Reload);
	Model->OnDataChangedEvent.AddUniqueDynamic(this, &UUDDealManagementViewModel::Update);

	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	// Retrieve view model for sub content control
	TObjectPtr<UUDViewModel> activeDealItemModel = hud->GetViewModelCollection(ActiveDealItemInstanceName, DealItemType);
	ActiveDealItemInstance = Cast<UUDDealItemViewModel>(activeDealItemModel);
	TObjectPtr<UUDViewModel> historyDealItemModel = hud->GetViewModelCollection(ActiveDealItemInstanceName, DealItemType);
	HistoryDealItemInstance = Cast<UUDDealItemViewModel>(historyDealItemModel);
	// Announce them to widget for additional binding.
	ActiveDealItemChangedEvent.Broadcast(ActiveDealItemInstance);
	HistoryDealItemChangedEvent.Broadcast(HistoryDealItemInstance);
	// Call initialize so instance is ready to use, once it receives data in runtime.
	ActiveDealItemInstance->FullUpdate();
	HistoryDealItemInstance->FullUpdate();

	Update();
}

void UUDDealManagementViewModel::Reload()
{
	Update();
}

void UUDDealManagementViewModel::Update()
{
	if (!Model->IsOverseeingStatePresent())
		return;
	if (!Model->IsGamePlayed())
		return;
	// Following updates require model.
	UpdateDealItems();
}

void UUDDealManagementViewModel::UpdateSelectedDealItem()
{
	if (IsActive())
	{
		FText dealCount = FText::Format(LOCTEXT("DealManagement", "{0}/{1}"), SelectedActiveIndex + 1, Content.Active.Num());
		SetDealCountText(dealCount);
		if (SelectedActiveIndex == UUDGlobalData::InvalidArrayIndex)
		{
			ActiveDealItemInstance->InvalidateContent(SelectedActiveDealItem);
		}
		else
		{
			ActiveDealItemInstance->SetContent(SelectedActiveDealItem);
		}
	}
	if (IsHistory())
	{
		FText dealCount = FText::Format(LOCTEXT("DealManagement", "{0}/{1}"), SelectedHistoryIndex + 1, Content.History.Num());
		SetDealCountText(dealCount);
		if (SelectedHistoryIndex == UUDGlobalData::InvalidArrayIndex)
		{
			HistoryDealItemInstance->InvalidateContent(SelectedHistoryDealItem);
		}
		else
		{
			HistoryDealItemInstance->SetContent(SelectedHistoryDealItem);
		}
	}
}

#undef LOCTEXT_NAMESPACE

void UUDDealManagementViewModel::CreateDeal()
{
	UE_LOG(LogTemp, Log, TEXT("UUDDealManagementViewModel: Create."));
	Model->RequestAction(Model->GetAction(UUDDealActionCreate::ActionTypeId));
}

void UUDDealManagementViewModel::SwitchActiveDeals()
{
	UE_LOG(LogTemp, Log, TEXT("UUDDealManagementViewModel: SwitchActiveDeals."));
	SetSelectedTabValue(ActiveTabValue);
	UpdateSelectedDealItem();
}

void UUDDealManagementViewModel::SwitchHistoryDeals()
{
	UE_LOG(LogTemp, Log, TEXT("UUDDealManagementViewModel: SwitchHistoryDeals."));
	SetSelectedTabValue(HistoryTabValue);
	UpdateSelectedDealItem();
}

void UUDDealManagementViewModel::Close()
{
	UE_LOG(LogTemp, Log, TEXT("UUDDealManagementViewModel: Close."));
	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	hud->HideWidget(hud->DealManagementWidget);
}

void UUDDealManagementViewModel::UpdateDealItems()
{
	UE_LOG(LogTemp, Log, TEXT("UUDDealManagementViewModel: UpdateDealItems."));
	Content = Model->GetAllLocalDeals();
	SelectedActiveDealItem = GetSelectedOrDefaultActive(SelectedActiveDealItem.DealId);
	SelectedHistoryDealItem = GetSelectedOrDefaultHistory(SelectedHistoryDealItem.DealId);
	UpdateSelectedDealItem();
}

#pragma region Navigation

FUDDealMinimalInfo UUDDealManagementViewModel::GetSelectedOrDefaultActive(int32 desiredSelectedItem)
{
	FUDDealMinimalInfo selected = GetInvalidDeal(true);
	SelectedActiveIndex = UUDGlobalData::InvalidArrayIndex;
	// Find
	const auto& found = Content.Active.FindByPredicate(
		[&desiredSelectedItem](const FUDDealMinimalInfo& item) { return item.DealId == desiredSelectedItem; }
	);
	if (found)
	{
		selected = *found;
		SelectedActiveIndex = Content.Active.Find(selected);
	}
	else if (Content.Active.Num() > 0)
	{
		selected = Content.Active[0];
		SelectedActiveIndex = 0;
	}

	return selected;
}

FUDDealMinimalInfo UUDDealManagementViewModel::GetSelectedOrDefaultHistory(int32 desiredSelectedItem)
{
	FUDDealMinimalInfo selected = GetInvalidDeal(false);
	SelectedHistoryIndex = UUDGlobalData::InvalidArrayIndex;
	// Find
	const auto& found = Content.History.FindByPredicate(
		[&desiredSelectedItem](const FUDDealMinimalInfo& item) { return item.DealId == desiredSelectedItem; }
	);
	if (found)
	{
		selected = *found;
		SelectedHistoryIndex = Content.History.Find(selected);
	}
	else if (Content.History.Num() > 0)
	{
		selected = Content.History[0];
		SelectedHistoryIndex = 0;
	}

	return selected;
}

void UUDDealManagementViewModel::First()
{
	if (IsActive() && Content.Active.Num() > 0)
	{
		SelectedActiveIndex = 0;
		SelectedActiveDealItem = Content.Active[SelectedActiveIndex];
		UpdateSelectedDealItem();
	}	
	if (IsHistory() && Content.History.Num() > 0)
	{
		SelectedHistoryIndex = 0;
		SelectedHistoryDealItem = Content.History[SelectedHistoryIndex];
		UpdateSelectedDealItem();
	}
}

void UUDDealManagementViewModel::Previous()
{
	int32 distance = AUDSkirmishPlayerController::Get(GetWorld())->GetButtonKeyDistance();
	if (IsActive() && SelectedActiveIndex - distance >= 0)
	{
		SelectedActiveIndex -= distance;
		SelectedActiveDealItem = Content.Active[SelectedActiveIndex];
		UpdateSelectedDealItem();
	}	
	if (IsHistory() && SelectedHistoryIndex - distance >= 0)
	{
		SelectedHistoryIndex -= distance;
		SelectedHistoryDealItem = Content.History[SelectedHistoryIndex];
		UpdateSelectedDealItem();
	}
}

void UUDDealManagementViewModel::Next()
{
	int32 distance = AUDSkirmishPlayerController::Get(GetWorld())->GetButtonKeyDistance();
	if (IsActive() && SelectedActiveIndex + distance < Content.Active.Num())
	{
		SelectedActiveIndex += distance;
		SelectedActiveDealItem = Content.Active[SelectedActiveIndex];
		UpdateSelectedDealItem();
	}	
	if (IsHistory() && SelectedHistoryIndex + distance < Content.History.Num())
	{
		SelectedHistoryIndex += distance;
		SelectedHistoryDealItem = Content.History[SelectedHistoryIndex];
		UpdateSelectedDealItem();
	}
}

void UUDDealManagementViewModel::Last()
{
	if (IsActive() && Content.Active.Num() > 0)
	{
		SelectedActiveIndex = Content.Active.Num() - 1;
		SelectedActiveDealItem = Content.Active[SelectedActiveIndex];
		UpdateSelectedDealItem();
	}
	if (IsHistory() && Content.History.Num() > 0)
	{
		SelectedHistoryIndex = Content.History.Num() - 1;
		SelectedHistoryDealItem = Content.History[SelectedHistoryIndex];
		UpdateSelectedDealItem();
	}
}
#pragma endregion

void UUDDealManagementViewModel::SetDealManagementTitleText(FText newDealManagementTitleText)
{
	UE_MVVM_SET_PROPERTY_VALUE(DealManagementTitleText, newDealManagementTitleText);
}

FText UUDDealManagementViewModel::GetDealManagementTitleText() const
{
	return DealManagementTitleText;
}

void UUDDealManagementViewModel::SetDealCountText(FText newDealCountText)
{
	UE_MVVM_SET_PROPERTY_VALUE(DealCountText, newDealCountText);
}

FText UUDDealManagementViewModel::GetDealCountText() const
{
	return DealCountText;
}

void UUDDealManagementViewModel::SetCloseText(FText newCloseText)
{
	UE_MVVM_SET_PROPERTY_VALUE(CloseText, newCloseText);
}

FText UUDDealManagementViewModel::GetCloseText() const
{
	return CloseText;
}

void UUDDealManagementViewModel::SetFirstText(FText newFirstText)
{
	UE_MVVM_SET_PROPERTY_VALUE(FirstText, newFirstText);
}

FText UUDDealManagementViewModel::GetFirstText() const
{
	return FirstText;
}

void UUDDealManagementViewModel::SetPreviousText(FText newPreviousText)
{
	UE_MVVM_SET_PROPERTY_VALUE(PreviousText, newPreviousText);
}

FText UUDDealManagementViewModel::GetPreviousText() const
{
	return PreviousText;
}

void UUDDealManagementViewModel::SetNextText(FText newNextText)
{
	UE_MVVM_SET_PROPERTY_VALUE(NextText, newNextText);
}

FText UUDDealManagementViewModel::GetNextText() const
{
	return NextText;
}

void UUDDealManagementViewModel::SetLastText(FText newLastText)
{
	UE_MVVM_SET_PROPERTY_VALUE(LastText, newLastText);
}

FText UUDDealManagementViewModel::GetLastText() const
{
	return LastText;
}

void UUDDealManagementViewModel::SetCreateText(FText newCreateText)
{
	UE_MVVM_SET_PROPERTY_VALUE(CreateText, newCreateText);
}

FText UUDDealManagementViewModel::GetCreateText() const
{
	return CreateText;
}

void UUDDealManagementViewModel::SetCreateToolTipText(FText newCreateToolTipText)
{
	UE_MVVM_SET_PROPERTY_VALUE(CreateToolTipText, newCreateToolTipText);
}

FText UUDDealManagementViewModel::GetCreateToolTipText() const
{
	return CreateToolTipText;
}

void UUDDealManagementViewModel::SetActiveText(FText newActiveText)
{
	UE_MVVM_SET_PROPERTY_VALUE(ActiveText, newActiveText);
}

FText UUDDealManagementViewModel::GetActiveText() const
{
	return ActiveText;
}

void UUDDealManagementViewModel::SetHistoryText(FText newHistoryText)
{
	UE_MVVM_SET_PROPERTY_VALUE(HistoryText, newHistoryText);
}

FText UUDDealManagementViewModel::GetHistoryText() const
{
	return HistoryText;
}

void UUDDealManagementViewModel::SetSelectedTabValue(int32 newSelectedTabValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(SelectedTabValue, newSelectedTabValue);
}

int32 UUDDealManagementViewModel::GetSelectedTabValue() const
{
	return SelectedTabValue;
}

int32 UUDDealManagementViewModel::GetActiveTabValue() const
{
	return ActiveTabValue;
}

int32 UUDDealManagementViewModel::GetHistoryTabValue() const
{
	return HistoryTabValue;
}