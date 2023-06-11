// Copyright Miroslav Valach

#include "Core/Simulation/UDModelStructs.h"

FUDFactionMinimalInfo::FUDFactionMinimalInfo()
{

}

FUDFactionMinimalInfo::FUDFactionMinimalInfo(int32 id, FString name) : Id(id), Name(name) 
{

}

FUDStratagemPickableInfo::FUDStratagemPickableInfo()
{

}

FUDStratagemPickableInfo::FUDStratagemPickableInfo(int32 id, FString name, int32 cost, bool isSelected) 
	: Id(id), Name(name), Cost(cost), IsSelected(isSelected)
{

}

FUDGameOverInfo::FUDGameOverInfo()
{

}

FUDFactionInfo::FUDFactionInfo()
{

}

FUDFactionInfo::FUDFactionInfo(int32 id, FString name, bool isPlayerControlled)
	: Id(id), Name(name), IsPlayerControlled(isPlayerControlled)
{

}