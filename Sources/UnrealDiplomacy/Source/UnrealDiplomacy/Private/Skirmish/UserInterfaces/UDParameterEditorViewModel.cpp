// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDParameterEditorViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Skirmish/UDSkirmishHUD.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Skirmish/UserInterfaces/Parameters/UDFactionParameterViewModel.h"
#include "Skirmish/UserInterfaces/Parameters/UDActionParameterViewModel.h"
#include "Skirmish/UserInterfaces/Parameters/UDTileParameterViewModel.h"
#include "Skirmish/UserInterfaces/Parameters/UDResourceParameterViewModel.h"
#include "Skirmish/UserInterfaces/Parameters/UDValueParameterViewModel.h"
#include "Skirmish/UserInterfaces/Parameters/UDTextParameterViewModel.h"

#define LOCTEXT_NAMESPACE "ParameterEditor"

int32 UUDParameterEditorViewModel::UniqueNameDefinition = 0;

void UUDParameterEditorViewModel::Initialize()
{
	if (!IsUniqueNameDefined) 
	{
		DefineCollections();
		IsUniqueNameDefined = true;
	}
}

void UUDParameterEditorViewModel::Update()
{
	if (!Model->IsOverseeingStatePresent())
		return;
	// Following updates require model.
	UpdateParameterLists();
}

#undef LOCTEXT_NAMESPACE

void UUDParameterEditorViewModel::SetContent(FUDParameterListInfo content)
{
	Content = content;
}

TArray<int32> UUDParameterEditorViewModel::GetValueParameters()
{
	TArray<int32> parameters = { };

	for (int32 i = 0; i < Content.OrderedData.Num(); i++)
	{
		// This is using type, but it should probably use direct check.
		// TODO Decide if it's worth keeping enum or it should be removed.
		switch (Content.OrderedType[i])
		{
		case EUDParameterType::Value:
			for (const auto& param : ValueParameterCollection)
			{
				parameters.Add(param->GetAsValue());
			}
			break;
		case EUDParameterType::Tile:
			for (const auto& param : TileParameterCollection)
			{
				parameters.Append(param->GetAsValueRange());
			}
			break;
		case EUDParameterType::Faction:
			for (const auto& param : FactionParameterCollection)
			{
				parameters.Add(param->GetAsValue());
			}
			break;
		case EUDParameterType::Action:
			for (const auto& param : ActionParameterCollection)
			{
				parameters.Add(param->GetAsValue());
			}
			break;
		case EUDParameterType::Resource:
			for (const auto& param : ResourceParameterCollection)
			{
				parameters.Add(param->GetAsValue());
			}
			break;
		case EUDParameterType::Text:
			// Skip
			break;
		default:
			UE_LOG(LogTemp, Warning, TEXT("UUDParameterEditorViewModel: Parameter is not supported by value editor."));
			break;
		}
	}

	return parameters;
}

TArray<FString> UUDParameterEditorViewModel::GetTextParameters()
{
	TArray<FString> parameters = { };
	for (int32 i = 0; i < Content.OrderedData.Num(); i++)
	{
		// This is using type, but it should probably use direct check.
		// TODO Decide if it's worth keeping enum or it should be removed.
		switch (Content.OrderedType[i])
		{
		case EUDParameterType::Text:
			for (const auto& param : TextParameterCollection)
			{
				parameters.Add(param->GetAsText());
			}
			break;
		case EUDParameterType::Value:
		case EUDParameterType::Tile:
		case EUDParameterType::Faction:
		case EUDParameterType::Action:
		case EUDParameterType::Resource:
			// Skip
			break;
		default:
			UE_LOG(LogTemp, Warning, TEXT("UUDParameterEditorViewModel: Parameter is not supported by text editor."));
			break;
		}
	}

	return parameters;
}

void UUDParameterEditorViewModel::UpdateParameterLists()
{
	UE_LOG(LogTemp, Log, TEXT("UUDParameterEditorViewModel: UpdateParameterLists."));

	TArray<FUDFactionParameter> factionParameters = { };
	TArray<FUDTileParameter> tileParameters = { };
	TArray<FUDActionParameter> actionParameters = { };
	TArray<FUDResourceParameter> resourceParameters = { };
	TArray<FUDTextParameter> textParameters = { };
	TArray<FUDValueParameter> valueParameters = { };

	for (int32 i = 0; i < Content.OrderedData.Num(); i++)
	{
		// This is using type, but it should probably use direct check.
		// TODO Decide if it's worth keeping enum or it should be removed.
		switch (Content.OrderedType[i])
		{
		case EUDParameterType::Value:
			valueParameters.Add(Content.OrderedData[i].Get<FUDValueParameter>());
			break;
		case EUDParameterType::Text:
			textParameters.Add(Content.OrderedData[i].Get<FUDTextParameter>());
			break;
		case EUDParameterType::Tile:
			tileParameters.Add(Content.OrderedData[i].Get<FUDTileParameter>());
			break;
		case EUDParameterType::Faction:
			factionParameters.Add(Content.OrderedData[i].Get<FUDFactionParameter>());
			break;
		case EUDParameterType::Action:
			actionParameters.Add(Content.OrderedData[i].Get<FUDActionParameter>());
			break;
		case EUDParameterType::Resource:
			resourceParameters.Add(Content.OrderedData[i].Get<FUDResourceParameter>());
			break;
		default:
			UE_LOG(LogTemp, Warning, TEXT("UUDParameterEditorViewModel: New parameters must be supported by UI."));
			break;
		}
	}

	UpdateFactionParameters(factionParameters);
	UpdateTextParameters(textParameters);
	UpdateValueParameters(valueParameters);
	UpdateTileParameters(tileParameters);
	UpdateActionParameters(actionParameters);
	UpdateResourceParameters(resourceParameters);
}

void UUDParameterEditorViewModel::DefineCollections()
{
	FactionParameterType = UUDFactionParameterViewModel::StaticClass();
	TileParameterType = UUDTileParameterViewModel::StaticClass();
	ActionParameterType = UUDActionParameterViewModel::StaticClass();
	ResourceParameterType = UUDResourceParameterViewModel::StaticClass();
	ValueParameterType = UUDValueParameterViewModel::StaticClass();
	TextParameterType = UUDTextParameterViewModel::StaticClass();
	int32 uniqueId = UUDParameterEditorViewModel::UniqueNameDefinition++;
	FactionParameterCollectionName = FName(FactionParameterCollectionName.ToString() + FString::FromInt(uniqueId));
	TileParameterCollectionName = FName(TileParameterCollectionName.ToString() + FString::FromInt(uniqueId));
	ActionParameterCollectionName = FName(ActionParameterCollectionName.ToString() + FString::FromInt(uniqueId));
	ResourceParameterCollectionName = FName(ResourceParameterCollectionName.ToString() + FString::FromInt(uniqueId));
	TextParameterCollectionName = FName(TextParameterCollectionName.ToString() + FString::FromInt(uniqueId));
	ValueParameterCollectionName = FName(ValueParameterCollectionName.ToString() + FString::FromInt(uniqueId));
	UE_LOG(LogTemp, Log, TEXT("UUDParameterEditorViewModel: Defined lists with subname [%d]."), uniqueId);
}

void UUDParameterEditorViewModel::UpdateFactionParameters(const TArray<FUDFactionParameter>& parameters)
{
	// Retrieve enough models
	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	TArray<TObjectPtr<UUDViewModel>>& viewModels = hud->GetViewModelCollection(FactionParameterCollectionName, FactionParameterType, parameters.Num());
	// Get rid of all models
	FactionParameterCollection.Empty();
	for (int32 i = 0; i < parameters.Num(); i++)
	{
		TObjectPtr<UUDFactionParameterViewModel> newViewModel = Cast<UUDFactionParameterViewModel>(viewModels[i]);
		newViewModel->SetContent(parameters[i]);
		newViewModel->FullUpdate();
		FactionParameterCollection.Add(newViewModel);
	}

	FactionParameterUpdatedEvent.Broadcast(FactionParameterCollection);
}

void UUDParameterEditorViewModel::UpdateResourceParameters(const TArray<FUDResourceParameter>& parameters)
{
	// Retrieve enough models
	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	TArray<TObjectPtr<UUDViewModel>>& viewModels = hud->GetViewModelCollection(ResourceParameterCollectionName, ResourceParameterType, parameters.Num());
	// Get rid of all models
	ResourceParameterCollection.Empty();
	for (int32 i = 0; i < parameters.Num(); i++)
	{
		TObjectPtr<UUDResourceParameterViewModel> newViewModel = Cast<UUDResourceParameterViewModel>(viewModels[i]);
		newViewModel->SetContent(parameters[i]);
		newViewModel->FullUpdate();
		ResourceParameterCollection.Add(newViewModel);
	}

	ResourceParameterUpdatedEvent.Broadcast(ResourceParameterCollection);
}

void UUDParameterEditorViewModel::UpdateTextParameters(const TArray<FUDTextParameter>& parameters)
{
	// Retrieve enough models
	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	TArray<TObjectPtr<UUDViewModel>>& viewModels = hud->GetViewModelCollection(TextParameterCollectionName, TextParameterType, parameters.Num());
	// Get rid of all models
	TextParameterCollection.Empty();
	for (int32 i = 0; i < parameters.Num(); i++)
	{
		TObjectPtr<UUDTextParameterViewModel> newViewModel = Cast<UUDTextParameterViewModel>(viewModels[i]);
		newViewModel->SetContent(parameters[i]);
		newViewModel->FullUpdate();
		TextParameterCollection.Add(newViewModel);
	}

	TextParameterUpdatedEvent.Broadcast(TextParameterCollection);
}

void UUDParameterEditorViewModel::UpdateValueParameters(const TArray<FUDValueParameter>& parameters)
{
	// Retrieve enough models
	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	TArray<TObjectPtr<UUDViewModel>>& viewModels = hud->GetViewModelCollection(ValueParameterCollectionName, ValueParameterType, parameters.Num());
	// Get rid of all models
	ValueParameterCollection.Empty();
	for (int32 i = 0; i < parameters.Num(); i++)
	{
		TObjectPtr<UUDValueParameterViewModel> newViewModel = Cast<UUDValueParameterViewModel>(viewModels[i]);
		newViewModel->SetContent(parameters[i]);
		newViewModel->FullUpdate();
		ValueParameterCollection.Add(newViewModel);
	}

	ValueParameterUpdatedEvent.Broadcast(ValueParameterCollection);
}

void UUDParameterEditorViewModel::UpdateActionParameters(const TArray<FUDActionParameter>& parameters)
{
	// Retrieve enough models
	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	TArray<TObjectPtr<UUDViewModel>>& viewModels = hud->GetViewModelCollection(ActionParameterCollectionName, ActionParameterType, parameters.Num());
	// Get rid of all models
	ActionParameterCollection.Empty();
	for (int32 i = 0; i < parameters.Num(); i++)
	{
		TObjectPtr<UUDActionParameterViewModel> newViewModel = Cast<UUDActionParameterViewModel>(viewModels[i]);
		newViewModel->SetContent(parameters[i]);
		newViewModel->FullUpdate();
		ActionParameterCollection.Add(newViewModel);
	}

	ActionParameterUpdatedEvent.Broadcast(ActionParameterCollection);
}

void UUDParameterEditorViewModel::UpdateTileParameters(const TArray<FUDTileParameter>& parameters)
{
	// Retrieve enough models
	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	TArray<TObjectPtr<UUDViewModel>>& viewModels = hud->GetViewModelCollection(TileParameterCollectionName, TileParameterType, parameters.Num());
	// Get rid of all models
	TileParameterCollection.Empty();
	for (int32 i = 0; i < parameters.Num(); i++)
	{
		TObjectPtr<UUDTileParameterViewModel> newViewModel = Cast<UUDTileParameterViewModel>(viewModels[i]);
		newViewModel->SetContent(parameters[i]);
		newViewModel->FullUpdate();
		TileParameterCollection.Add(newViewModel);
	}

	TileParameterUpdatedEvent.Broadcast(TileParameterCollection);
}