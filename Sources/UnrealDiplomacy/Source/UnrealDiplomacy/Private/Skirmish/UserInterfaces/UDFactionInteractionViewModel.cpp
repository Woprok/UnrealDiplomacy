// Copyright Miroslav Valach
// TODO if we added continuation for executing direct actions such as gift. Then this should be updated.
// TODO add text parameter in indirect actions.

#include "Skirmish/UserInterfaces/UDFactionInteractionViewModel.h"
#include "Skirmish/UserInterfaces/UDParameterEditorViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Skirmish/UDSkirmishHUD.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

#define LOCTEXT_NAMESPACE "FactionInteraction"

int32 UUDFactionInteractionViewModel::UniqueNameDefinition = 0;

void UUDFactionInteractionViewModel::Initialize()
{
	if (!IsUniqueNameDefined) 
	{
		DefineInstances();
		IsUniqueNameDefined = true;
	}
	FText interact = FText(LOCTEXT("FactionInteraction", "Interact"));
	SetInteractText(interact);

	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	// Retrieve view models for sub content controls
	TObjectPtr<UUDViewModel> editorModel = hud->GetViewModelCollection(ParameterEditorInstanceName, ParameterEditorType);
	ParameterEditorInstance = Cast<UUDParameterEditorViewModel>(editorModel);
	// Announce them to widget for additional binding.
	ParameterEditorChangedEvent.Broadcast(ParameterEditorInstance);
	// Call initialize so each Instance is ready to use, once it receives data in runtime.
	ParameterEditorInstance->FullUpdate();
}

void UUDFactionInteractionViewModel::Update()
{
	if (!Model->IsOverseeingStatePresent())
		return;
	FText interact = FText::FromString(Content.Name);
	SetInteractText(interact);
	UpdateEditor();
}

#undef LOCTEXT_NAMESPACE

void UUDFactionInteractionViewModel::SetContent(int32 selectedFaction, EUDDecisionType interactionType, FUDFactionInteractionInfo content)
{
	SelectedFaction = selectedFaction;
	InteractionType = interactionType;
	Content = content;
}

void UUDFactionInteractionViewModel::Interact()
{
	// TODO execution of the action with all current parameters.
	UE_LOG(LogTemp, Log, TEXT("UUDFactionInteractionViewModel: Interact."));
	// Start with the guaranteed parameter.
	TArray<int32> valueParameters = { };
	valueParameters.Add(SelectedFaction);
	valueParameters.Append(ParameterEditorInstance->GetValueParameters());
	FString textParameter = ParameterEditorInstance->GetTextParameter();

	if (valueParameters.Num() > 0 && textParameter.Len() == 0)
	{
		FUDActionData data = Model->GetAction(Content.ActionTypeId, valueParameters);
		DecisionRequest(data);
	}
	else if (valueParameters.Num() > 0 && textParameter.Len() > 0)
	{
		FUDActionData data = Model->GetAction(Content.ActionTypeId, valueParameters, textParameter);
		DecisionRequest(data);
	}
}

void UUDFactionInteractionViewModel::DecisionRequest(FUDActionData data)
{
	switch (InteractionType)
	{
	case EUDDecisionType::Gift:
		// Direct requests should not be propagated to decision level ?
		Model->RequestAction(data);
		break;
	case EUDDecisionType::Offer:
	case EUDDecisionType::Request:
	case EUDDecisionType::Demand:
		Model->RequestAction(Model->GetDecisionAction(SelectedFaction, InteractionType, data));
		break;
	case EUDDecisionType::Error:
	default:
		UE_LOG(LogTemp, Log, TEXT("UUDFactionInteractionViewModel: Failed to interact."));
		break;
	}
}

void UUDFactionInteractionViewModel::UpdateEditor()
{
	UE_LOG(LogTemp, Log, TEXT("UUDFactionInteractionViewModel: UpdateEditor."));
	ParameterEditorInstance->SetContent(Content.Parameters);
}

void UUDFactionInteractionViewModel::DefineInstances()
{
	int32 uniqueId = UUDFactionInteractionViewModel::UniqueNameDefinition++;
	ParameterEditorType = UUDParameterEditorViewModel::StaticClass();
	ParameterEditorInstanceName = FName(ParameterEditorInstanceName.ToString() + FString::FromInt(uniqueId));
	UE_LOG(LogTemp, Log, TEXT("UUDFactionInteractionViewModel: Defined editor [%d]."), uniqueId);
}

void UUDFactionInteractionViewModel::SetInteractText(FText newInteractText)
{
	UE_MVVM_SET_PROPERTY_VALUE(InteractText, newInteractText);
}

FText UUDFactionInteractionViewModel::GetInteractText() const
{
	return InteractText;
}