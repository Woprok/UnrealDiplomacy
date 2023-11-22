// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDDealItemViewModel.h"
#include "Skirmish/UserInterfaces/Deals/UDDealGeneralTabViewModel.h"
#include "Skirmish/UserInterfaces/Deals/UDDealEditationTabViewModel.h"
#include "Skirmish/UserInterfaces/Deals/UDDealExecutionTabViewModel.h"
#include "Skirmish/UDSkirmishHUD.h"
#include "Skirmish/UDSkirmishPlayerController.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/UDGlobalData.h"

#define LOCTEXT_NAMESPACE "DealItem"

void UUDDealItemViewModel::Setup()
{
	GeneralTabViewModelType = UUDDealGeneralTabViewModel::StaticClass();
	EditationTabViewModelType = UUDDealEditationTabViewModel::StaticClass();
	ExecutionTabViewModelType = UUDDealExecutionTabViewModel::StaticClass();

	FText dealName = FText(LOCTEXT("DealItem", "Deal"));
	SetDealNameText(dealName);
	FText general = FText(LOCTEXT("DealItem", "Overview"));
	SetGeneralText(general);
	FText points = FText(LOCTEXT("DealItem", "Edit Points"));
	SetEditationText(points);
	FText resolution = FText(LOCTEXT("DealItem", "Resolution"));
	SetExecutionText(resolution);

	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	// Retrieve view models for sub content controls
	TObjectPtr<UUDViewModel> generalModel = hud->GetViewModelCollection(GeneralTabViewModelInstanceName, GeneralTabViewModelType);
	TObjectPtr<UUDViewModel> editationModel = hud->GetViewModelCollection(EditationTabViewModelInstanceName, EditationTabViewModelType);
	TObjectPtr<UUDViewModel> executionModel = hud->GetViewModelCollection(ExecutionTabViewModelInstanceName, ExecutionTabViewModelType);
	GeneralTabViewModelInstance = Cast<UUDDealGeneralTabViewModel>(generalModel);
	EditationTabViewModelInstance = Cast<UUDDealEditationTabViewModel>(editationModel);
	ExecutionTabViewModelInstance = Cast<UUDDealExecutionTabViewModel>(executionModel);
	// Announce them to widget for additional binding.
	SetGeneralTabContent(FUDViewModelContent(GeneralTabViewModelInstance));
	SetEditationTabContent(FUDViewModelContent(EditationTabViewModelInstance));
	SetExecutionTabContent(FUDViewModelContent(ExecutionTabViewModelInstance));
	// Call initialize so each Instance is ready to use, once it receives data in runtime.
	// TODO remove this commented code, if it works properly
	//GeneralTabViewModelInstance->Refresh();
	//EditationTabViewModelInstance->Refresh();
	//ExecutionTabViewModelInstance->Refresh();
	//Update();
}

void UUDDealItemViewModel::Refresh()
{
	SetDealNameText(FText::FromString(Content.Name));

	if (!Model->IsOverseeingStatePresent())
		return;
	if (!Model->IsGamePlayed())
		return;
	if (Content.DealId == UUDGlobalData::InvalidDealId)
		return;
	// Following updates require model.
	GeneralTabViewModelInstance->SetContent(Content);
	EditationTabViewModelInstance->SetContent(Content);
	ExecutionTabViewModelInstance->SetContent(Content);
	GeneralTabViewModelInstance->Refresh();
	EditationTabViewModelInstance->Refresh();
	ExecutionTabViewModelInstance->Refresh();
}

#undef LOCTEXT_NAMESPACE

void UUDDealItemViewModel::General()
{
	UE_LOG(LogTemp, Log, TEXT("UUDDealItemViewModel: General."));
	SetSelectedTabValue(GeneralTabValue);
}

void UUDDealItemViewModel::Editation()
{
	UE_LOG(LogTemp, Log, TEXT("UUDDealItemViewModel: Editation."));
	SetSelectedTabValue(EditationTabValue);
}

void UUDDealItemViewModel::Execution()
{
	UE_LOG(LogTemp, Log, TEXT("UUDDealItemViewModel: Execution."));
	SetSelectedTabValue(ExecutionTabValue);
}

void UUDDealItemViewModel::InvalidateContent(FUDDealMinimalInfo content)
{
	UE_LOG(LogTemp, Log, TEXT("UUDDealItemViewModel: InvalidateContent."));
	Content = content;
	SetIsValidContentValue(false);
}

void UUDDealItemViewModel::SetContent(FUDDealMinimalInfo content)
{
	UE_LOG(LogTemp, Log, TEXT("UUDDealItemViewModel: SetContent."));
	Content = content;
	SetIsValidContentValue(true);
}

void UUDDealItemViewModel::SetDealNameText(FText newDealNameText)
{
	UE_MVVM_SET_PROPERTY_VALUE(DealNameText, newDealNameText);
}

FText UUDDealItemViewModel::GetDealNameText() const
{
	return DealNameText;
}

void UUDDealItemViewModel::SetGeneralText(FText newGeneralText)
{
	UE_MVVM_SET_PROPERTY_VALUE(GeneralText, newGeneralText);
}

FText UUDDealItemViewModel::GetGeneralText() const
{
	return GeneralText;
}

void UUDDealItemViewModel::SetEditationText(FText newEditationText)
{
	UE_MVVM_SET_PROPERTY_VALUE(EditationText, newEditationText);
}

FText UUDDealItemViewModel::GetEditationText() const
{
	return EditationText;
}

void UUDDealItemViewModel::SetExecutionText(FText newExecutionText)
{
	UE_MVVM_SET_PROPERTY_VALUE(ExecutionText, newExecutionText);
}

FText UUDDealItemViewModel::GetExecutionText() const
{
	return ExecutionText;
}

void UUDDealItemViewModel::SetSelectedTabValue(int32 newSelectedTabValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(SelectedTabValue, newSelectedTabValue);
}

int32 UUDDealItemViewModel::GetSelectedTabValue() const
{
	return SelectedTabValue;
}

int32 UUDDealItemViewModel::GetGeneralTabValue() const
{
	return GeneralTabValue;
}

int32 UUDDealItemViewModel::GetEditationTabValue() const
{
	return EditationTabValue;
}

int32 UUDDealItemViewModel::GetExecutionTabValue() const
{
	return ExecutionTabValue;
}

void UUDDealItemViewModel::SetIsValidContentValue(bool newIsValidContentValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(IsValidContentValue, newIsValidContentValue);
}

bool UUDDealItemViewModel::GetIsValidContentValue() const
{
	return IsValidContentValue;
}

void UUDDealItemViewModel::SetGeneralTabContent(FUDViewModelContent newGeneralTabContent)
{
	UE_MVVM_SET_PROPERTY_VALUE(GeneralTabContent, newGeneralTabContent);

}
FUDViewModelContent UUDDealItemViewModel::GetGeneralTabContent() const
{
	return GeneralTabContent;

}
void UUDDealItemViewModel::SetEditationTabContent(FUDViewModelContent newEditationTabContent)
{
	UE_MVVM_SET_PROPERTY_VALUE(EditationTabContent, newEditationTabContent);

}
FUDViewModelContent UUDDealItemViewModel::GetEditationTabContent() const
{
	return EditationTabContent;
}
void UUDDealItemViewModel::SetExecutionTabContent(FUDViewModelContent newExecutionTabContent)
{
	UE_MVVM_SET_PROPERTY_VALUE(ExecutionTabContent, newExecutionTabContent);

}
FUDViewModelContent UUDDealItemViewModel::GetExecutionTabContent() const
{
	return ExecutionTabContent;
}