// Copyright Miroslav Valach

#include "Lobby/UserInterfaces/UDLobbyHostViewModel.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Core/Simulation/Actions/UDSettingActionAICountChange.h"
#include "Core/Simulation/Actions/UDSettingActionMapSeedChange.h"
#include "Core/Simulation/Actions/UDSettingActionMapWidthChange.h"
#include "Core/Simulation/Actions/UDSettingActionMapHeightChange.h"
#include "Core/Simulation/Actions/UDSettingActionStratagemPointsChange.h"
#include "Core/UDSessionSubsystem.h"
#include "Core/Simulation/UDActionData.h"

#define LOCTEXT_NAMESPACE "LobbyHost"

void UUDLobbyHostViewModel::Setup()
{
	FText hostSettingsTitle = FText(LOCTEXT("LobbyHost", "Game Settings"));
	SetHostSettingsTitleText(hostSettingsTitle);
	FText AICount = FText(LOCTEXT("LobbyHost", "AI Count"));
	SetAICountText(AICount);
	FText mapSeed = FText(LOCTEXT("LobbyHost", "Map Seed"));
	SetMapSeedText(mapSeed);
	FText mapWidth = FText(LOCTEXT("LobbyHost", "Map Width"));
	SetMapWidthText(mapWidth);
	FText mapHeight = FText(LOCTEXT("LobbyHost", "Map Height"));
	SetMapHeightText(mapHeight);
	FText stratagemPoints = FText(LOCTEXT("LobbyHost", "Stratagem Points"));
	SetStratagemPointsText(stratagemPoints);
}

void UUDLobbyHostViewModel::Refresh()
{
	TObjectPtr<UUDSessionSubsystem> sessions = UUDSessionSubsystem::Get(GetWorld());
	SetIsHostValue(sessions->IsLocalPlayerHost(sessions->GetSessionName()));

	if (!Model->IsOverseeingStatePresent())
		return;
	// Following updates require model.
	if (!IsValueEditing || !IsHostValue)
	{
		int32 aiCountValue = Model->GetSettingsAICount();
		SetAICountValue(aiCountValue);
		int32 mapSeedValue = Model->GetSettingsMapSeed();
		SetMapSeedValue(mapSeedValue);
		int32 mapWidthValue = Model->GetSettingsMapWidth();
		SetMapWidthValue(mapWidthValue);
		int32 mapHeightValue = Model->GetSettingsMapHeight();
		SetMapHeightValue(mapHeightValue);
		int32 stratagemPointsValue = Model->GetSettingsStratagemPoints();
		SetStratagemPointsValue(stratagemPointsValue);
	}
}

#undef LOCTEXT_NAMESPACE

void UUDLobbyHostViewModel::StartValueEditation(float InValue)
{
	IsValueEditing = true;
}

void UUDLobbyHostViewModel::StopAICountValueEditation(float InValue, ETextCommit::Type CommitMethod)
{
	IsValueEditing = false;
	// Update only if value was changed.
	int32 newValue = InValue;
	int32 oldValue = Model->GetSettingsStratagemPoints();
	//// TODO find source of the bug that causes the binding for SpinBox to not update automatically.
	if (oldValue != newValue)
	{
		Model->RequestAction(Model->GetAction(UUDSettingActionAICountChange::ActionTypeId, { newValue }));
	}
}
void UUDLobbyHostViewModel::StopMapSeedValueEditation(float InValue, ETextCommit::Type CommitMethod)
{
	IsValueEditing = false;
	// Update only if value was changed.
	int32 newValue = InValue;
	int32 oldValue = Model->GetSettingsStratagemPoints();
	//// TODO find source of the bug that causes the binding for SpinBox to not update automatically.
	if (oldValue != newValue)
	{
		Model->RequestAction(Model->GetAction(UUDSettingActionMapSeedChange::ActionTypeId, { newValue }));
	}
}
void UUDLobbyHostViewModel::StopMapWidthValueEditation(float InValue, ETextCommit::Type CommitMethod)
{
	IsValueEditing = false;
	// Update only if value was changed.
	int32 newValue = InValue;
	int32 oldValue = Model->GetSettingsStratagemPoints();
	//// TODO find source of the bug that causes the binding for SpinBox to not update automatically.
	if (oldValue != newValue)
	{
		Model->RequestAction(Model->GetAction(UUDSettingActionMapWidthChange::ActionTypeId, { newValue }));
	}
}
void UUDLobbyHostViewModel::StopMapHeightValueEditation(float InValue, ETextCommit::Type CommitMethod)
{
	IsValueEditing = false;
	// Update only if value was changed.
	int32 newValue = InValue;
	int32 oldValue = Model->GetSettingsStratagemPoints();
	//// TODO find source of the bug that causes the binding for SpinBox to not update automatically.
	if (oldValue != newValue)
	{
		Model->RequestAction(Model->GetAction(UUDSettingActionMapHeightChange::ActionTypeId, { newValue }));
	}
}
void UUDLobbyHostViewModel::StopStratagemPointsValueEditation(float InValue, ETextCommit::Type CommitMethod)
{
	IsValueEditing = false;
	// Update only if value was changed.
	int32 newValue = InValue;
	int32 oldValue = Model->GetSettingsStratagemPoints();
	//// TODO find source of the bug that causes the binding for SpinBox to not update automatically.
	if (oldValue != newValue)
	{
		Model->RequestAction(Model->GetAction(UUDSettingActionStratagemPointsChange::ActionTypeId, { newValue }));
	}
}

void UUDLobbyHostViewModel::SetHostSettingsTitleText(FText newHostSettingsTitleText)
{
	UE_MVVM_SET_PROPERTY_VALUE(HostSettingsTitleText, newHostSettingsTitleText);
}

FText UUDLobbyHostViewModel::GetHostSettingsTitleText() const
{
	return HostSettingsTitleText;
}

void UUDLobbyHostViewModel::SetAICountText(FText newAICountText)
{
	UE_MVVM_SET_PROPERTY_VALUE(AICountText, newAICountText);
}

FText UUDLobbyHostViewModel::GetAICountText() const
{
	return AICountText;
}

void UUDLobbyHostViewModel::SetMapSeedText(FText newMapSeedText)
{
	UE_MVVM_SET_PROPERTY_VALUE(MapSeedText, newMapSeedText);
}

FText UUDLobbyHostViewModel::GetMapSeedText() const
{
	return MapSeedText;
}

void UUDLobbyHostViewModel::SetMapWidthText(FText newMapWidthText)
{
	UE_MVVM_SET_PROPERTY_VALUE(MapWidthText, newMapWidthText);
}

FText UUDLobbyHostViewModel::GetMapWidthText() const
{
	return MapWidthText;
}

void UUDLobbyHostViewModel::SetMapHeightText(FText newMapHeightText)
{
	UE_MVVM_SET_PROPERTY_VALUE(MapHeightText, newMapHeightText);
}

FText UUDLobbyHostViewModel::GetMapHeightText() const
{
	return MapHeightText;
}

void UUDLobbyHostViewModel::SetStratagemPointsText(FText newStratagemPointsText)
{
	UE_MVVM_SET_PROPERTY_VALUE(StratagemPointsText, newStratagemPointsText);
}

FText UUDLobbyHostViewModel::GetStratagemPointsText() const
{
	return StratagemPointsText;
}

void UUDLobbyHostViewModel::SetAICountValue(float newAICountValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(AICountValue, newAICountValue);
}

float UUDLobbyHostViewModel::GetAICountValue() const
{
	return AICountValue;
}

void UUDLobbyHostViewModel::SetMapSeedValue(float newMapSeedValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(MapSeedValue, newMapSeedValue);
}

float UUDLobbyHostViewModel::GetMapSeedValue() const
{
	return MapSeedValue;
}

void UUDLobbyHostViewModel::SetMapWidthValue(float newMapWidthValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(MapWidthValue, newMapWidthValue);
}

float UUDLobbyHostViewModel::GetMapWidthValue() const
{
	return MapWidthValue;
}

void UUDLobbyHostViewModel::SetMapHeightValue(float newMapHeightValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(MapHeightValue, newMapHeightValue);
}

float UUDLobbyHostViewModel::GetMapHeightValue() const
{
	return MapHeightValue;
}

void UUDLobbyHostViewModel::SetStratagemPointsValue(float newStratagemPointsValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(StratagemPointsValue, newStratagemPointsValue);
}

float UUDLobbyHostViewModel::GetStratagemPointsValue() const
{
	return StratagemPointsValue;
}

void UUDLobbyHostViewModel::SetIsHostValue(bool newIsHostValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(IsHostValue, newIsHostValue);
}

bool UUDLobbyHostViewModel::GetIsHostValue() const
{
	return IsHostValue;
}