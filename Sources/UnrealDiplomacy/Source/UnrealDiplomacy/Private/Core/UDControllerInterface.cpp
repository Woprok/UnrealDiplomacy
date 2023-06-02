// Copyright Miroslav Valach

#include "Core/UDControllerInterface.h"
#include "Core/UDGlobalData.h"

void IUDControllerInterface::SetControllerUniqueId(int32 uniqueControllerId)
{
	return;
}

int32 IUDControllerInterface::GetControllerUniqueId()
{
	return UUDGlobalData::InvalidControllerId;
}

void IUDControllerInterface::SetControlledFactionId(int32 factionId)
{
	return;
}

int32 IUDControllerInterface::GetControlledFactionId()
{
	return UUDGlobalData::InvalidFactionId;
}

void IUDControllerInterface::SetControllerType(EUDControllerType type)
{
	return;
}

EUDControllerType IUDControllerInterface::GetControllerType()
{
	return EUDControllerType::Menu;
}