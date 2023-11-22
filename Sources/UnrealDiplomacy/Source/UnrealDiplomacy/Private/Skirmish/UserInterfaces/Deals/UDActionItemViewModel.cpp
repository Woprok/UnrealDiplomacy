// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDActionItemViewModel.h"
#include "Skirmish/UserInterfaces/UDParameterEditorViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Skirmish/UDSkirmishHUD.h"
#include "Core/Simulation/Actions/UDDealActionContractPointAccept.h"
#include "Core/Simulation/Actions/UDDealActionContractPointReject.h"
#include "Core/Simulation/Actions/UDDealActionContractPointSabotage.h"
#include "Core/Simulation/Actions/UDDealActionContractPointTamper.h"
#include "Core/Simulation/UDActionData.h"

#define LOCTEXT_NAMESPACE "ActionItem"

int32 UUDActionItemViewModel::UniqueNameDefinition = 0;

void UUDActionItemViewModel::Setup()
{
	if (!IsUniqueNameDefined)
	{
		DefineInstances();
		IsUniqueNameDefined = true;
	}
	FText editor = FText(LOCTEXT("PointContent", "Change Editor"));
	SetEditorText(editor);
	FText accept = FText(LOCTEXT("PointContent", "Accept"));
	SetAcceptText(accept);
	FText deny = FText(LOCTEXT("PointContent", "Deny"));
	SetDenyText(deny);
	FText sabotage = FText(LOCTEXT("PointContent", "Sabotage"));
	SetSabotageText(sabotage);
	FText change = FText(LOCTEXT("PointContent", "Change"));
	SetChangeText(change);

	// Retrieve model
	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	TObjectPtr<UUDViewModel> editorModel = hud->GetViewModelCollection(ParameterEditorInstanceName, ParameterEditorType);
	ParameterEditorInstance = Cast<UUDParameterEditorViewModel>(editorModel);
	SetParameterEditorContent(FUDViewModelContent(ParameterEditorInstance));
	// TODO if this is equal to previous version that called it each update. If yes, remove the clear part.
	ParameterEditorInstance->ValuesUpdated.Clear();
	ParameterEditorInstance->TextUpdated.Clear();
	ParameterEditorInstance->ValuesUpdated.AddUObject(this, &UUDActionItemViewModel::SaveValuesChange);
	ParameterEditorInstance->TextUpdated.AddUObject(this, &UUDActionItemViewModel::SaveTextChange);
}

void UUDActionItemViewModel::Refresh()
{
	SetActionTitleText(FText::FromString(Content.ActionTitle));
	SetActionText(FText::FromString(Content.ActionContent));
	SetCanAcceptValue(Content.IsInteractable);
	SetCanDenyValue(Content.IsInteractable);
	SetCanChangeValue(Content.IsInteractable);
	SetCanSabotageValue(Content.IsSabotageable);
	UpdateEditor();
}

#undef LOCTEXT_NAMESPACE

void UUDActionItemViewModel::SaveValuesChange(TArray<int32> values)
{
	TArray<int32> valueParameters = { };
	valueParameters.Add(Content.DealId);
	valueParameters.Add(Content.PointIndex);
	valueParameters.Append(values);
	BufferedValueParameters = valueParameters;
}
void UUDActionItemViewModel::SaveTextChange(FString text)
{
	BufferedTextParameter = text;
}

void UUDActionItemViewModel::SetContent(FUDDealActionMinimalInfo content)
{
	Content = Model->GetActionInteraction(content.DealId, content.ActionIndex);
}

void UUDActionItemViewModel::Accept()
{
	UE_LOG(LogTemp, Log, TEXT("UUDActionItemViewModel: Accept."));
	Model->RequestAction(Model->GetAction(UUDDealActionContractPointAccept::ActionTypeId, { Content.DealId, Content.PointIndex }));
}

void UUDActionItemViewModel::Change()
{
	UE_LOG(LogTemp, Log, TEXT("UUDActionItemViewModel: Change."));
	Model->RequestAction(Model->GetAction(UUDDealActionContractPointTamper::ActionTypeId, 
		BufferedValueParameters,
		BufferedTextParameter
	));
}

void UUDActionItemViewModel::Deny()
{
	UE_LOG(LogTemp, Log, TEXT("UUDActionItemViewModel: Deny."));
	Model->RequestAction(Model->GetAction(UUDDealActionContractPointReject::ActionTypeId, { Content.DealId, Content.PointIndex }));
}

void UUDActionItemViewModel::Sabotage()
{
	UE_LOG(LogTemp, Log, TEXT("UUDActionItemViewModel: Sabotage."));
	Model->RequestAction(Model->GetAction(UUDDealActionContractPointSabotage::ActionTypeId, { Content.DealId, Content.PointIndex }));
}

void UUDActionItemViewModel::UpdateEditor()
{
	UE_LOG(LogTemp, Log, TEXT("UUDActionItemViewModel: UpdateEditor."));
	ParameterEditorInstance->SetContent(Content.Parameters);
	ParameterEditorInstance->Refresh();
}

void UUDActionItemViewModel::DefineInstances()
{
	int32 uniqueId = UUDActionItemViewModel::UniqueNameDefinition++;
	ParameterEditorType = UUDParameterEditorViewModel::StaticClass();
	ParameterEditorInstanceName = FName(ParameterEditorInstanceName.ToString() + FString::FromInt(uniqueId));
	UE_LOG(LogTemp, Log, TEXT("UUDActionItemViewModel: Defined editor [%d]."), uniqueId);
}

void UUDActionItemViewModel::SetActionText(FText newActionText)
{
	UE_MVVM_SET_PROPERTY_VALUE(ActionText, newActionText);
}

FText UUDActionItemViewModel::GetActionText() const
{
	return ActionText;
}

void UUDActionItemViewModel::SetActionTitleText(FText newActionTitleText)
{
	UE_MVVM_SET_PROPERTY_VALUE(ActionTitleText, newActionTitleText);
}

FText UUDActionItemViewModel::GetActionTitleText() const
{
	return ActionTitleText;
}

void UUDActionItemViewModel::SetAcceptText(FText newAcceptText)
{
	UE_MVVM_SET_PROPERTY_VALUE(AcceptText, newAcceptText);
}

FText UUDActionItemViewModel::GetAcceptText() const
{
	return AcceptText;
}

void UUDActionItemViewModel::SetChangeText(FText newChangeText)
{
	UE_MVVM_SET_PROPERTY_VALUE(ChangeText, newChangeText);
}

FText UUDActionItemViewModel::GetChangeText() const
{
	return ChangeText;
}

void UUDActionItemViewModel::SetDenyText(FText newDenyText)
{
	UE_MVVM_SET_PROPERTY_VALUE(DenyText, newDenyText);
}

FText UUDActionItemViewModel::GetDenyText() const
{
	return DenyText;
}

void UUDActionItemViewModel::SetSabotageText(FText newSabotageText)
{
	UE_MVVM_SET_PROPERTY_VALUE(SabotageText, newSabotageText);
}

FText UUDActionItemViewModel::GetSabotageText() const
{
	return SabotageText;
}

void UUDActionItemViewModel::SetEditorText(FText newEditorText)
{
	UE_MVVM_SET_PROPERTY_VALUE(EditorText, newEditorText);
}

FText UUDActionItemViewModel::GetEditorText() const
{
	return EditorText;
}

void UUDActionItemViewModel::SetCanAcceptValue(bool newCanAcceptValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(CanAcceptValue, newCanAcceptValue);
}

bool UUDActionItemViewModel::GetCanAcceptValue() const
{
	return CanAcceptValue;
}

void UUDActionItemViewModel::SetCanChangeValue(bool newCanChangeValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(CanChangeValue, newCanChangeValue);
}

bool UUDActionItemViewModel::GetCanChangeValue() const
{
	return CanChangeValue;
}

void UUDActionItemViewModel::SetCanDenyValue(bool newCanDenyValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(CanDenyValue, newCanDenyValue);
}

bool UUDActionItemViewModel::GetCanDenyValue() const
{
	return CanDenyValue;
}

void UUDActionItemViewModel::SetCanSabotageValue(bool newCanSabotageValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(CanSabotageValue, newCanSabotageValue);
}

bool UUDActionItemViewModel::GetCanSabotageValue() const
{
	return CanSabotageValue;
}

void UUDActionItemViewModel::SetParameterEditorContent(FUDViewModelContent newParameterEditorContent)
{
	UE_MVVM_SET_PROPERTY_VALUE(ParameterEditorContent, newParameterEditorContent);
}

FUDViewModelContent UUDActionItemViewModel::GetParameterEditorContent() const
{
	return ParameterEditorContent;
}