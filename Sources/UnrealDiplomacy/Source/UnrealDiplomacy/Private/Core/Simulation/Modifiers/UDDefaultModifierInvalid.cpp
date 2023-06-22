// Copyright Miroslav Valach

#include "Core/Simulation/Modifiers/UDDefaultModifierInvalid.h"
#include "Core/UDGlobalData.h"

int32 UUDDefaultModifierInvalid::GetId() const
{
	return UUDGlobalData::InvalidModifierId;
}