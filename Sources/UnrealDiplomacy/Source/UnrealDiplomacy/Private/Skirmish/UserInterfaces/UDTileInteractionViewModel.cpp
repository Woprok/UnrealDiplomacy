// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDTileInteractionViewModel.h"
#include "Skirmish/UserInterfaces/UDParameterEditorViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Skirmish/UDSkirmishHUD.h"
#include "Core/Simulation/UDActionData.h"

#define LOCTEXT_NAMESPACE "TileInteraction"

int32 UUDTileInteractionViewModel::UniqueNameDefinition = 0;

void UUDTileInteractionViewModel::Initialize()
{
	if (!IsUniqueNameDefined) 
	{
		DefineInstances();
		IsUniqueNameDefined = true;
	}
	FText interact = FText(LOCTEXT("TileInteraction", "Interact"));
	SetInteractText(interact);

	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	// Retrieve view models for sub content controls
	TObjectPtr<UUDViewModel> editorModel = hud->GetViewModelCollection(ParameterEditorInstanceName, ParameterEditorType);
	ParameterEditorInstance = Cast<UUDParameterEditorViewModel>(editorModel);
	// Announce them to widget for additional binding.
	SetParameterEditorContent(FUDViewModelContent(ParameterEditorInstance));
	// Call initialize so each Instance is ready to use, once it receives data in runtime.
	ParameterEditorInstance->FullUpdate();
}

void UUDTileInteractionViewModel::Update()
{
	if (!Model->IsOverseeingStatePresent())
		return;
	FText interact = FText::FromString(Content.Name);
	SetInteractText(interact);
	UpdateEditor();
}

#undef LOCTEXT_NAMESPACE

void UUDTileInteractionViewModel::SetContent(FIntPoint selectedTile, FUDTileInteractionInfo content)
{
	SelectedTile = selectedTile;
	Content = content;
}

void UUDTileInteractionViewModel::Interact()
{
	UE_LOG(LogTemp, Log, TEXT("UUDTileInteractionViewModel: Interact."));
	// Start with the guaranteed parameter.
	TArray<int32> valueParameters = { };
	valueParameters.Add(SelectedTile.X);
	valueParameters.Add(SelectedTile.Y);
	valueParameters.Append(ParameterEditorInstance->GetValueParameters());
	FString textParameter = ParameterEditorInstance->GetTextParameter();

	if (valueParameters.Num() > 0 && textParameter.Len() == 0)
	{
		Model->RequestAction(Model->GetAction(Content.ActionTypeId, valueParameters));
	}
	else if (valueParameters.Num() > 0 && textParameter.Len() > 0)
	{
		Model->RequestAction(Model->GetAction(Content.ActionTypeId, valueParameters, textParameter));
	}
}

void UUDTileInteractionViewModel::UpdateEditor()
{
	UE_LOG(LogTemp, Log, TEXT("UUDTileInteractionViewModel: UpdateEditor."));
	ParameterEditorInstance->SetContent(Content.Parameters);
}

void UUDTileInteractionViewModel::DefineInstances()
{
	ParameterEditorType = UUDParameterEditorViewModel::StaticClass();
	int32 uniqueId = UUDTileInteractionViewModel::UniqueNameDefinition++;
	ParameterEditorInstanceName = FName(ParameterEditorInstanceName.ToString() + FString::FromInt(uniqueId));
	UE_LOG(LogTemp, Log, TEXT("UUDTileInteractionViewModel: Defined editor [%d]."), uniqueId);
}

void UUDTileInteractionViewModel::SetInteractText(FText newInteractText)
{
	UE_MVVM_SET_PROPERTY_VALUE(InteractText, newInteractText);
}

FText UUDTileInteractionViewModel::GetInteractText() const
{
	return InteractText;
}

void UUDTileInteractionViewModel::SetParameterEditorContent(FUDViewModelContent newParameterEditorContent)
{
	UE_MVVM_SET_PROPERTY_VALUE(ParameterEditorContent, newParameterEditorContent);
}

FUDViewModelContent UUDTileInteractionViewModel::GetParameterEditorContent() const
{
	return ParameterEditorContent;
}