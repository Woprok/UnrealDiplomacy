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
		DefineInstances();
		IsUniqueNameDefined = true;
	}

	Update();
}

void UUDParameterEditorViewModel::Update()
{
	HideParameters();

	if (!Model->IsOverseeingStatePresent())
		return;
	// Following updates require model.
	UpdateParameterInstances();
}

#undef LOCTEXT_NAMESPACE

void UUDParameterEditorViewModel::SetContent(FUDParameterListInfo content)
{
	Content = content;
}

void UUDParameterEditorViewModel::HideParameters()
{
	SetHasDealActionParameterValue(false);
	SetHasFactionInvokerParameterValue(false);
	SetHasFactionTargetParameterValue(false);
	SetHasTileParameterValue(false);
	SetHasActionParameterValue(false);
	SetHasResourceParameterValue(false);
	SetHasValueParameterValue(false);
	SetHasTextParameterValue(false);
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
		case EUDParameterType::DealAction:
			parameters.Add(DealActionParameterInstance->GetAsValue());
			break;
		case EUDParameterType::FactionInvoker:
			parameters.Add(FactionInvokerParameterInstance->GetAsValue());
			break;
		case EUDParameterType::FactionTarget:
			parameters.Add(FactionTargetParameterInstance->GetAsValue());
			break;
		case EUDParameterType::Tile:
			parameters.Append(TileParameterInstance->GetAsValueRange());
			break;
		case EUDParameterType::Action:
			parameters.Add(ActionParameterInstance->GetAsValue());
			break;
		case EUDParameterType::Resource:
			parameters.Add(ResourceParameterInstance->GetAsValue());
			break;
		case EUDParameterType::Value:
			parameters.Add(ValueParameterInstance->GetAsValue());
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

FString UUDParameterEditorViewModel::GetTextParameter()
{
	for (int32 i = 0; i < Content.OrderedData.Num(); i++)
	{
		// This is using type, but it should probably use direct check.
		// TODO Decide if it's worth keeping enum or it should be removed.
		switch (Content.OrderedType[i])
		{
		case EUDParameterType::Text:
			return TextParameterInstance->GetAsText();
			break;
		case EUDParameterType::DealAction:
		case EUDParameterType::FactionInvoker:
		case EUDParameterType::FactionTarget:
		case EUDParameterType::Tile:
		case EUDParameterType::Action:
		case EUDParameterType::Resource:
		case EUDParameterType::Value:
			// Skip
			break;
		default:
			UE_LOG(LogTemp, Warning, TEXT("UUDParameterEditorViewModel: Parameter is not supported by text editor."));
			break;
		}
	}

	return TEXT("");
}

void UUDParameterEditorViewModel::UpdateParameterInstances()
{
	UE_LOG(LogTemp, Log, TEXT("UUDParameterEditorViewModel: UpdateParameterInstances."));

	for (int32 i = 0; i < Content.OrderedData.Num(); i++)
	{
		// This is using type, but it should probably use direct check.
		// TODO Decide if it's worth keeping enum or it should be removed.
		switch (Content.OrderedType[i])
		{
		case EUDParameterType::DealAction:
			UpdateDealActionParameter(Content.OrderedData[i].Get<FUDActionParameter>());
			break;
		case EUDParameterType::FactionInvoker:
			UpdateFactionInvokerParameter(Content.OrderedData[i].Get<FUDFactionParameter>());
			break;
		case EUDParameterType::FactionTarget:
			UpdateFactionTargetParameter(Content.OrderedData[i].Get<FUDFactionParameter>());
			break;
		case EUDParameterType::Tile:
			UpdateTileParameter(Content.OrderedData[i].Get<FUDTileParameter>());
			break;
		case EUDParameterType::Action:
			UpdateActionParameter(Content.OrderedData[i].Get<FUDActionParameter>());
			break;
		case EUDParameterType::Resource:
			UpdateResourceParameter(Content.OrderedData[i].Get<FUDResourceParameter>());
			break;
		case EUDParameterType::Value:
			UpdateValueParameter(Content.OrderedData[i].Get<FUDValueParameter>());
			break;
		case EUDParameterType::Text:
			UpdateTextParameter(Content.OrderedData[i].Get<FUDTextParameter>());
			break;
		default:
			UE_LOG(LogTemp, Warning, TEXT("UUDParameterEditorViewModel: New parameters must be supported by UI."));
			break;
		}
	}
}

void UUDParameterEditorViewModel::OnDealActionParameterChanged()
{
	DealActionUpdated.Broadcast(DealActionParameterInstance->GetAsValue());
}

void UUDParameterEditorViewModel::OnFactionInvokerParameterChanged()
{
	InvokerUpdated.Broadcast(FactionInvokerParameterInstance->GetAsValue());
}

void UUDParameterEditorViewModel::ResolveValueParameterChange()
{
	TArray<int32> parameters = { };
	for (int32 i = 0; i < Content.OrderedData.Num(); i++)
	{
		switch (Content.OrderedType[i])
		{
		case EUDParameterType::FactionTarget:
			parameters.Add(FactionTargetParameterInstance->GetAsValue());
			break;
		case EUDParameterType::Tile:
			parameters.Append(TileParameterInstance->GetAsValueRange());
			break;
		case EUDParameterType::Action:
			parameters.Add(ActionParameterInstance->GetAsValue());
			break;
		case EUDParameterType::Resource:
			parameters.Add(ResourceParameterInstance->GetAsValue());
			break;
		case EUDParameterType::Value:
			parameters.Add(ValueParameterInstance->GetAsValue());
			break;
		}
	}

	ValuesUpdated.Broadcast(parameters);
}

void UUDParameterEditorViewModel::OnFactionTargetParameterChanged()
{
	ResolveValueParameterChange();
}

void UUDParameterEditorViewModel::OnTileParameterChanged()
{
	ResolveValueParameterChange();
}

void UUDParameterEditorViewModel::OnActionParameterChanged()
{
	ResolveValueParameterChange();
}

void UUDParameterEditorViewModel::OnResourceParameterChanged()
{
	ResolveValueParameterChange();
}

void UUDParameterEditorViewModel::OnValueParameterChanged()
{
	ResolveValueParameterChange();
}

void UUDParameterEditorViewModel::OnTextParameterChanged()
{
	TextUpdated.Broadcast(TextParameterInstance->GetAsText());
}

void UUDParameterEditorViewModel::DefineInstances()
{
	int32 uniqueId = UUDParameterEditorViewModel::UniqueNameDefinition++;
	DefineDealActionParameter(uniqueId);
	DefineFactionInvokerParameter(uniqueId);
	DefineFactionTargetParameter(uniqueId);
	DefineTileParameter(uniqueId);
	DefineActionParameter(uniqueId);
	DefineResourceParameter(uniqueId);
	DefineValueParameter(uniqueId);
	DefineTextParameter(uniqueId);
	UE_LOG(LogTemp, Log, TEXT("UUDParameterEditorViewModel: Defined isntances with subname [%d]."), uniqueId);
}

void UUDParameterEditorViewModel::DefineDealActionParameter(int32 id)
{
	DealActionParameterType = UUDActionParameterViewModel::StaticClass();
	DealActionParameterInstanceName = FName(DealActionParameterInstanceName.ToString() + FString::FromInt(id));

	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	TObjectPtr<UUDViewModel> viewModel = hud->GetViewModelCollection(DealActionParameterInstanceName, DealActionParameterType);
	DealActionParameterInstance = Cast<UUDActionParameterViewModel>(viewModel);
	DealActionParameterUpdatedEvent.Broadcast(DealActionParameterInstance);
	DealActionParameterInstance->OnChangeEvent.Clear();
	DealActionParameterInstance->OnChangeEvent.AddUObject(this, &UUDParameterEditorViewModel::OnDealActionParameterChanged);
	DealActionParameterInstance->FullUpdate();
}

void UUDParameterEditorViewModel::DefineFactionInvokerParameter(int32 id)
{
	FactionInvokerParameterType = UUDFactionParameterViewModel::StaticClass();
	FactionInvokerParameterInstanceName = FName(FactionInvokerParameterInstanceName.ToString() + FString::FromInt(id));

	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	TObjectPtr<UUDViewModel> viewModel = hud->GetViewModelCollection(FactionInvokerParameterInstanceName, FactionInvokerParameterType);
	FactionInvokerParameterInstance = Cast<UUDFactionParameterViewModel>(viewModel);
	FactionInvokerParameterUpdatedEvent.Broadcast(FactionInvokerParameterInstance);
	FactionInvokerParameterInstance->OnChangeEvent.Clear();
	FactionInvokerParameterInstance->OnChangeEvent.AddUObject(this, &UUDParameterEditorViewModel::OnFactionInvokerParameterChanged);
	FactionInvokerParameterInstance->FullUpdate();
}

void UUDParameterEditorViewModel::DefineFactionTargetParameter(int32 id)
{
	FactionTargetParameterType = UUDFactionParameterViewModel::StaticClass();
	FactionTargetParameterInstanceName = FName(FactionTargetParameterInstanceName.ToString() + FString::FromInt(id));

	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	TObjectPtr<UUDViewModel> viewModel = hud->GetViewModelCollection(FactionTargetParameterInstanceName, FactionTargetParameterType);
	FactionTargetParameterInstance = Cast<UUDFactionParameterViewModel>(viewModel);
	FactionTargetParameterUpdatedEvent.Broadcast(FactionTargetParameterInstance);
	FactionTargetParameterInstance->OnChangeEvent.Clear();
	FactionTargetParameterInstance->OnChangeEvent.AddUObject(this, &UUDParameterEditorViewModel::OnFactionTargetParameterChanged);
	FactionTargetParameterInstance->FullUpdate();
}

void UUDParameterEditorViewModel::DefineTileParameter(int32 id)
{
	TileParameterType = UUDTileParameterViewModel::StaticClass();
	TileParameterInstanceName = FName(TileParameterInstanceName.ToString() + FString::FromInt(id));

	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	TObjectPtr<UUDViewModel> viewModel = hud->GetViewModelCollection(TileParameterInstanceName, TileParameterType);
	TileParameterInstance = Cast<UUDTileParameterViewModel>(viewModel);
	TileParameterUpdatedEvent.Broadcast(TileParameterInstance);
	TileParameterInstance->OnChangeEvent.Clear();
	TileParameterInstance->OnChangeEvent.AddUObject(this, &UUDParameterEditorViewModel::OnTileParameterChanged);
	TileParameterInstance->FullUpdate();
}

void UUDParameterEditorViewModel::DefineActionParameter(int32 id)
{
	ActionParameterType = UUDActionParameterViewModel::StaticClass();
	ActionParameterInstanceName = FName(ActionParameterInstanceName.ToString() + FString::FromInt(id));

	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	TObjectPtr<UUDViewModel> viewModel = hud->GetViewModelCollection(ActionParameterInstanceName, ActionParameterType);
	ActionParameterInstance = Cast<UUDActionParameterViewModel>(viewModel);
	ActionParameterUpdatedEvent.Broadcast(ActionParameterInstance);
	ActionParameterInstance->OnChangeEvent.Clear();
	ActionParameterInstance->OnChangeEvent.AddUObject(this, &UUDParameterEditorViewModel::OnActionParameterChanged);
	ActionParameterInstance->FullUpdate();
}

void UUDParameterEditorViewModel::DefineResourceParameter(int32 id)
{
	ResourceParameterType = UUDResourceParameterViewModel::StaticClass();
	ResourceParameterInstanceName = FName(ResourceParameterInstanceName.ToString() + FString::FromInt(id));

	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	TObjectPtr<UUDViewModel> viewModel = hud->GetViewModelCollection(ResourceParameterInstanceName, ResourceParameterType);
	ResourceParameterInstance = Cast<UUDResourceParameterViewModel>(viewModel);
	ResourceParameterUpdatedEvent.Broadcast(ResourceParameterInstance);
	ResourceParameterInstance->OnChangeEvent.Clear();
	ResourceParameterInstance->OnChangeEvent.AddUObject(this, &UUDParameterEditorViewModel::OnResourceParameterChanged);
	ResourceParameterInstance->FullUpdate();
}

void UUDParameterEditorViewModel::DefineValueParameter(int32 id)
{
	ValueParameterType = UUDValueParameterViewModel::StaticClass();
	ValueParameterInstanceName = FName(ValueParameterInstanceName.ToString() + FString::FromInt(id));

	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	TObjectPtr<UUDViewModel> viewModel = hud->GetViewModelCollection(ValueParameterInstanceName, ValueParameterType);
	ValueParameterInstance = Cast<UUDValueParameterViewModel>(viewModel);
	ValueParameterUpdatedEvent.Broadcast(ValueParameterInstance);
	ValueParameterInstance->OnChangeEvent.Clear();
	ValueParameterInstance->OnChangeEvent.AddUObject(this, &UUDParameterEditorViewModel::OnValueParameterChanged);
	ValueParameterInstance->FullUpdate();
}

void UUDParameterEditorViewModel::DefineTextParameter(int32 id)
{
	TextParameterType = UUDTextParameterViewModel::StaticClass();
	TextParameterInstanceName = FName(TextParameterInstanceName.ToString() + FString::FromInt(id));

	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	TObjectPtr<UUDViewModel> viewModel = hud->GetViewModelCollection(TextParameterInstanceName, TextParameterType);
	TextParameterInstance = Cast<UUDTextParameterViewModel>(viewModel);
	TextParameterUpdatedEvent.Broadcast(TextParameterInstance);
	TextParameterInstance->OnChangeEvent.Clear();
	TextParameterInstance->OnChangeEvent.AddUObject(this, &UUDParameterEditorViewModel::OnTextParameterChanged);
	TextParameterInstance->FullUpdate();
}

void UUDParameterEditorViewModel::UpdateDealActionParameter(const FUDActionParameter& parameter)
{
	DealActionParameterInstance->SetContent(parameter);
	SetHasDealActionParameterValue(true);
	DealActionParameterUpdatedEvent.Broadcast(DealActionParameterInstance);
	DealActionParameterInstance->FullUpdate();
}

void UUDParameterEditorViewModel::UpdateFactionInvokerParameter(const FUDFactionParameter& parameter)
{
	FactionInvokerParameterInstance->SetContent(parameter);
	SetHasFactionInvokerParameterValue(true);
	FactionInvokerParameterUpdatedEvent.Broadcast(FactionInvokerParameterInstance);
	FactionInvokerParameterInstance->FullUpdate();
}

void UUDParameterEditorViewModel::UpdateFactionTargetParameter(const FUDFactionParameter& parameter)
{
	FactionTargetParameterInstance->SetContent(parameter);
	SetHasFactionTargetParameterValue(true);
	FactionTargetParameterUpdatedEvent.Broadcast(FactionTargetParameterInstance);
	FactionTargetParameterInstance->FullUpdate();
}

void UUDParameterEditorViewModel::UpdateTileParameter(const FUDTileParameter& parameter)
{
	TileParameterInstance->SetContent(parameter);
	SetHasTileParameterValue(true);
	TileParameterUpdatedEvent.Broadcast(TileParameterInstance);
	TileParameterInstance->FullUpdate();
}

void UUDParameterEditorViewModel::UpdateActionParameter(const FUDActionParameter& parameter)
{
	ActionParameterInstance->SetContent(parameter);
	SetHasActionParameterValue(true);
	ActionParameterUpdatedEvent.Broadcast(ActionParameterInstance);
	ActionParameterInstance->FullUpdate();
}

void UUDParameterEditorViewModel::UpdateResourceParameter(const FUDResourceParameter& parameter)
{
	ResourceParameterInstance->SetContent(parameter);
	SetHasResourceParameterValue(true);
	ResourceParameterUpdatedEvent.Broadcast(ResourceParameterInstance);
	ResourceParameterInstance->FullUpdate();
}

void UUDParameterEditorViewModel::UpdateValueParameter(const FUDValueParameter& parameter)
{
	ValueParameterInstance->SetContent(parameter);
	SetHasValueParameterValue(true);
	ValueParameterUpdatedEvent.Broadcast(ValueParameterInstance);
	ValueParameterInstance->FullUpdate();
}

void UUDParameterEditorViewModel::UpdateTextParameter(const FUDTextParameter& parameter)
{
	TextParameterInstance->SetContent(parameter);
	SetHasTextParameterValue(true);
	TextParameterUpdatedEvent.Broadcast(TextParameterInstance);
	TextParameterInstance->FullUpdate();
}

void UUDParameterEditorViewModel::SetHasDealActionParameterValue(bool newHasDealActionParameterValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(HasDealActionParameterValue, newHasDealActionParameterValue);
}

bool UUDParameterEditorViewModel::GetHasDealActionParameterValue() const
{
	return HasDealActionParameterValue;
}

void UUDParameterEditorViewModel::SetHasFactionInvokerParameterValue(bool newHasFactionInvokerParameterValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(HasFactionInvokerParameterValue, newHasFactionInvokerParameterValue);
}

bool UUDParameterEditorViewModel::GetHasFactionInvokerParameterValue() const
{
	return HasFactionInvokerParameterValue;
}


void UUDParameterEditorViewModel::SetHasFactionTargetParameterValue(bool newHasFactionTargetParameterValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(HasFactionTargetParameterValue, newHasFactionTargetParameterValue);
}

bool UUDParameterEditorViewModel::GetHasFactionTargetParameterValue() const
{
	return HasFactionTargetParameterValue;
}

void UUDParameterEditorViewModel::SetHasTileParameterValue(bool newHasTileParameterValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(HasTileParameterValue, newHasTileParameterValue);
}

bool UUDParameterEditorViewModel::GetHasTileParameterValue() const
{
	return HasTileParameterValue;
}

void UUDParameterEditorViewModel::SetHasActionParameterValue(bool newHasActionParameterValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(HasActionParameterValue, newHasActionParameterValue);
}

bool UUDParameterEditorViewModel::GetHasActionParameterValue() const
{
	return HasActionParameterValue;
}

void UUDParameterEditorViewModel::SetHasResourceParameterValue(bool newHasResourceParameterValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(HasResourceParameterValue, newHasResourceParameterValue);
}

bool UUDParameterEditorViewModel::GetHasResourceParameterValue() const
{
	return HasResourceParameterValue;
}

void UUDParameterEditorViewModel::SetHasValueParameterValue(bool newHasValueParameterValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(HasValueParameterValue, newHasValueParameterValue);
}

bool UUDParameterEditorViewModel::GetHasValueParameterValue() const
{
	return HasValueParameterValue;
}

void UUDParameterEditorViewModel::SetHasTextParameterValue(bool newHasTextParameterValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(HasTextParameterValue, newHasTextParameterValue);
}

bool UUDParameterEditorViewModel::GetHasTextParameterValue() const
{
	return HasTextParameterValue;
}