// Copyright Miroslav Valach

#include "Lobby/UserInterfaces/UDLobbyHostViewModel.h"

#define LOCTEXT_NAMESPACE "LobbyHost"

void UUDLobbyHostViewModel::Initialize()
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
}

void UUDLobbyHostViewModel::Update()
{

}

#undef LOCTEXT_NAMESPACE

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