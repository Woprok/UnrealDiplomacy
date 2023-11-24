// Copyright Miroslav Valach

#include "Core/Simulation/Resources/UDDefaultResourceInvalid.h"
#include "Core/UDGlobalData.h"

int32 UUDDefaultResourceInvalid::GetId() const
{
	return UUDGlobalData::InvalidResourceId;
}