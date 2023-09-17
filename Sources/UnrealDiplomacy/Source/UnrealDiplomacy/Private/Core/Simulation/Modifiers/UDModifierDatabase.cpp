// Copyright Miroslav Valach

#include "Core/Simulation/Modifiers/UDModifierDatabase.h"
#include "Core/Simulation/UDModifierInterface.h"
#include "Core/Simulation/Modifiers/UDDefaultModifier.h"
#include "Core/Simulation/Modifiers/UDDefaultModifierInvalid.h"
#include "Core/Simulation/Modifiers/UDTileModifier.h"
#include "Core/Simulation/Modifiers/UDTileModifierPermitExploit.h"
#include "Core/Simulation/Modifiers/UDFactionModifier.h"
#include "Core/Simulation/Modifiers/UDFactionModifierThroneSupport.h"

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
	};
}

TArray<TScriptInterface<IUDModifierInterface>> UUDModifierDatabase::GetFactionModifiers(UObject* parent)
{
	return {
		//NewObject<UUDFactionModifier>(parent),
		NewObject<UUDFactionModifierThroneSupport>(parent),
	};
}