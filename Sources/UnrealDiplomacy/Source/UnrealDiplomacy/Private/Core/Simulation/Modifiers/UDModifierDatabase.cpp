// Copyright Miroslav Valach

#include "Core/Simulation/Modifiers/UDModifierDatabase.h"
#include "Core/Simulation/UDModifierInterface.h"
#include "Core/Simulation/Modifiers/UDDefaultModifier.h"
#include "Core/Simulation/Modifiers/UDDefaultModifierInvalid.h"
#include "Core/Simulation/Modifiers/UDTileModifier.h"
#include "Core/Simulation/Modifiers/UDTileModifierPermitExploit.h"
#include "Core/Simulation/Modifiers/UDTileModifierStratagemActivated.h"
#include "Core/Simulation/Modifiers/UDTileModifierBuildingFarm.h"
#include "Core/Simulation/Modifiers/UDTileModifierBuildingFortress.h"
#include "Core/Simulation/Modifiers/UDTileModifierBuildingManufactury.h"
#include "Core/Simulation/Modifiers/UDTileModifierBuildingPalace.h"
#include "Core/Simulation/Modifiers/UDTileModifierBuildingQuarry.h"
#include "Core/Simulation/Modifiers/UDTileModifierBuildingTradeGuild.h"
#include "Core/Simulation/Modifiers/UDFactionModifier.h"
#include "Core/Simulation/Modifiers/UDFactionModifierThroneSupport.h"
#include "Core/Simulation/Modifiers/UDFactionModifierStratagemShare.h"
#include "Core/Simulation/Modifiers/UDFactionModifierMilitarySupport.h"
#include "Core/Simulation/Modifiers/UDFactionModifierStratagemActivated.h"
#include "Core/Simulation/Modifiers/UDFactionModifierThroneUsurper.h"

TArray<TScriptInterface<IUDModifierInterface>> UUDModifierDatabase::GetDefaultModifiers(UObject* parent)
{
	return {
		NewObject<UUDDefaultModifier>(parent),
		NewObject<UUDDefaultModifierInvalid>(parent),
	};
}

TArray<TScriptInterface<IUDModifierInterface>> UUDModifierDatabase::GetTileModifiers(UObject* parent)
{
	return {
		//NewObject<UUDTileModifier>(parent),
		NewObject<UUDTileModifierPermitExploit>(parent),
		NewObject<UUDTileModifierBuildingFarm>(parent),
		NewObject<UUDTileModifierBuildingFortress>(parent),
		NewObject<UUDTileModifierBuildingManufactury>(parent),
		NewObject<UUDTileModifierBuildingPalace>(parent),
		NewObject<UUDTileModifierBuildingQuarry>(parent),
		NewObject<UUDTileModifierBuildingTradeGuild>(parent),
		NewObject<UUDTileModifierStratagemActivated>(parent),
	};
}

TArray<TScriptInterface<IUDModifierInterface>> UUDModifierDatabase::GetFactionModifiers(UObject* parent)
{
	return {
		//NewObject<UUDFactionModifier>(parent),
		NewObject<UUDFactionModifierThroneSupport>(parent),
		NewObject<UUDFactionModifierMilitarySupport>(parent),
		NewObject<UUDFactionModifierStratagemShare>(parent),
		NewObject<UUDFactionModifierStratagemActivated>(parent),
		NewObject<UUDFactionModifierThroneUsurper>(parent),
	};
}