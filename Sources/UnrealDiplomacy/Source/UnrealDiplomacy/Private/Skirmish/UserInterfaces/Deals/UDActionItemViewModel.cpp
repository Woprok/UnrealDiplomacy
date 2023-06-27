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

#define LOCTEXT_NAMESPACE "ActionItem"

int32 UUDActionItemViewModel::UniqueNameDefinition = 0;

void UUDActionItemViewModel::Initialize()
{
	if (!IsUniqueNameDefined)
	{
		DefineInstances();
		IsUniqueNameDefined = true;
	}
	FText editor = FText(LOCTEXT("PointContent", "Change"));
	SetEditorText(editor);
}

void UUDActionItemViewModel::Update()
{
	// TODO set action text based on parameters same way as the message is done.
	// TODO allow editing all parameters of the actions other then invoker with the standard interaction editor ?.
}

#undef LOCTEXT_NAMESPACE

void UUDActionItemViewModel::SetContent(FUDDealActionInfo content)
{
	Content = content;
}

void UUDActionItemViewModel::Accept()
{
	UE_LOG(LogTemp, Log, TEXT("UUDActionItemViewModel: Accept."));
	Model->RequestAction(Model->GetAction(UUDDealActionContractPointAccept::ActionTypeId, { Content.DealId }));
}

void UUDActionItemViewModel::Change()
{
	UE_LOG(LogTemp, Log, TEXT("UUDActionItemViewModel: Change."));
	Model->RequestAction(Model->GetAction(UUDDealActionContractPointTamper::ActionTypeId, { Content.DealId }));
}

void UUDActionItemViewModel::Deny()
{
	UE_LOG(LogTemp, Log, TEXT("UUDActionItemViewModel: Deny."));
	Model->RequestAction(Model->GetAction(UUDDealActionContractPointReject::ActionTypeId, { Content.DealId }));
}

void UUDActionItemViewModel::Sabotage()
{
	UE_LOG(LogTemp, Log, TEXT("UUDActionItemViewModel: Sabotage."));
	Model->RequestAction(Model->GetAction(UUDDealActionContractPointSabotage::ActionTypeId, { Content.DealId }));
}

void UUDActionItemViewModel::UpdateEditor()
{
	UE_LOG(LogTemp, Log, TEXT("UUDActionItemViewModel: UpdateEditor."));
	//ParameterEditorInstance->SetContent(Content.Parameters);
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