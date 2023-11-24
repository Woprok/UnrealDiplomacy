// Copyright Miroslav Valach

#include "Core/Simulation/Resources/UDResourceDatabase.h"
#include "Core/Simulation/Resources/UDDefaultResource.h"
#include "Core/Simulation/Resources/UDDefaultResourceInvalid.h"
#include "Core/Simulation/Resources/UDGameResource.h"
#include "Core/Simulation/Resources/UDGameResourceReputation.h"
#include "Core/Simulation/Resources/UDGameResourceFood.h"
#include "Core/Simulation/Resources/UDGameResourceMaterials.h"
#include "Core/Simulation/Resources/UDGameResourceGold.h"
#include "Core/Simulation/Resources/UDGameResourceLuxuries.h"
#include "Core/Simulation/Resources/UDGameResourceManpower.h"

TArray<TScriptInterface<IUDResourceInterface>> UUDResourceDatabase::GetDefaultResources(UObject* parent)
{
	return {
		NewObject<UUDDefaultResource>(parent),
		NewObject<UUDDefaultResourceInvalid>(parent)
	};
}

TArray<TScriptInterface<IUDResourceInterface>> UUDResourceDatabase::GetGameResources(UObject* parent)
{
	return {
		//NewObject<UUDGameResource>(parent),
		NewObject<UUDGameResourceReputation>(parent),
		NewObject<UUDGameResourceFood>(parent),
		NewObject<UUDGameResourceGold>(parent),
		NewObject<UUDGameResourceMaterials>(parent),
		NewObject<UUDGameResourceManpower>(parent),
		NewObject<UUDGameResourceLuxuries>(parent),
	};
}