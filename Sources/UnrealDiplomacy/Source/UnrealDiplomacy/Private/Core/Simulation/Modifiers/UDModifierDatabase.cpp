// Copyright Miroslav Valach

#include "Core/Simulation/Modifiers/UDModifierDatabase.h"
#include "Core/Simulation/UDModifierInterface.h"
#include "Core/Simulation/Modifiers/UDDefaultModifierInvalid.h"
#include "Core/Simulation/Modifiers/UDTileModifierPermitExploit.h"
#include "Core/Simulation/Modifiers/UDFactionModifierThroneSupport.h"

TArray<TScriptInterface<IUDModifierInterface>> UUDModifierDatabase::GetDefaultModifiers(UObject* parent)
{
	return {
		NewObject<UUDDefaultModifierInvalid>(parent),
	};
}

TArray<TScriptInterface<IUDModifierInterface>> UUDModifierDatabase::GetTileModifiers(UObject* parent)
{
	return {
		NewObject<UUDTileModifierPermitExploit>(parent),
	};
}

TArray<TScriptInterface<IUDModifierInterface>> UUDModifierDatabase::GetFactionModifiers(UObject* parent)
{
	return {
		NewObject<UUDFactionModifierThroneSupport>(parent),
	};
}