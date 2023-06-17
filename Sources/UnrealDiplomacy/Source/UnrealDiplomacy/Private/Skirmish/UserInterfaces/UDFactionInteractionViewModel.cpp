// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDFactionInteractionViewModel.h"
#include "Skirmish/UserInterfaces/UDParameterEditorViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Skirmish/UDSkirmishHUD.h"

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

void UUDFactionInteractionViewModel::SetContent(int32 selectedFaction, FUDFactionInteractionInfo content)
{
	SelectedFaction = selectedFaction;
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
	TArray<FString> textParameters = { };
	textParameters.Append(ParameterEditorInstance->GetTextParameters());

	if (valueParameters.Num() > 0 && textParameters.Num() == 0)
	{
		Model->RequestAction(Model->GetAction(Content.ActionTypeId, valueParameters));
	}
	else if (valueParameters.Num() > 0 && textParameters.Num() > 0)
	{
		// TODO stop ignoring array or get rid of array ?
		Model->RequestAction(Model->GetAction(Content.ActionTypeId, valueParameters, textParameters[0]));
	}
}

void UUDFactionInteractionViewModel::UpdateEditor()
{
	UE_LOG(LogTemp, Log, TEXT("UUDFactionInteractionViewModel: UpdateEditor."));
	ParameterEditorInstance->SetContent(Content.Parameters);
}

void UUDFactionInteractionViewModel::DefineInstances()
{
	ParameterEditorType = UUDParameterEditorViewModel::StaticClass();
	int32 uniqueId = UUDFactionInteractionViewModel::UniqueNameDefinition++;
	ParameterEditorInstanceName = FName(ParameterEditorInstanceName.ToString() + FString::FromInt(uniqueId));
	UE_LOG(LogTemp, Log, TEXT("UUDTileInteractionViewModel: Defined editor [%d]."), uniqueId);
}

void UUDFactionInteractionViewModel::SetInteractText(FText newInteractText)
{
	UE_MVVM_SET_PROPERTY_VALUE(InteractText, newInteractText);
}

FText UUDFactionInteractionViewModel::GetInteractText() const
{
	return InteractText;
}