// Copyright Miroslav Valach

#include "Core/Simulation/UDModelStructs.h"
#include "Core/Simulation/UDWorldState.h"

FUDFactionMinimalInfo::FUDFactionMinimalInfo()
{

}

FUDFactionMinimalInfo::FUDFactionMinimalInfo(int32 id, FString name) : Id(id), Name(name) 
{

}

FUDActionMinimalInfo::FUDActionMinimalInfo()
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

FUDRegencyTurnInfo::FUDRegencyTurnInfo() 
{

}

FUDThroneInfo::FUDThroneInfo()
{

}

FUDAvailableDiplomacyInfo::FUDAvailableDiplomacyInfo()
{

}

#pragma region Resources
#define LOCTEXT_NAMESPACE "Resource"
FUDResourceInfo::FUDResourceInfo()
{

}

FUDResourceInfo::FUDResourceInfo(int32 resourceId, FString name, int32 amount) 
	: Id(resourceId), Name(name), Amount(amount)
{

}

FUDResourceMinimalInfo::FUDResourceMinimalInfo()
{

}
#undef LOCTEXT_NAMESPACE
#pragma endregion

FUDFactionInteractionInfo::FUDFactionInteractionInfo()
{

}

FUDTileInteractionInfo::FUDTileInteractionInfo()
{

}

FUDTileInfo::FUDTileInfo()
{

}

FUDTileMinimalInfo::FUDTileMinimalInfo()
{

}

#pragma region Parameter
FUDValueParameter::FUDValueParameter()
{

}

FUDTextParameter::FUDTextParameter()
{

}

FUDTileParameter::FUDTileParameter()
{

}

FUDDealParameter::FUDDealParameter()
{

}

FUDFactionParameter::FUDFactionParameter()
{

}

FUDActionParameter::FUDActionParameter()
{

}

FUDParameterListInfo::FUDParameterListInfo()
{

}

FUDResourceParameter::FUDResourceParameter()
{

}
#pragma endregion

FUDMessageContentInfo::FUDMessageContentInfo()
{

}

FUDMessageInfo::FUDMessageInfo()
{

}

FUDMessageInteractionInfo::FUDMessageInteractionInfo()
{

}

FUDModifierInfo::FUDModifierInfo()
{

}

#pragma region Deals

FUDDealMinimalInfo::FUDDealMinimalInfo()
{

}

FUDDealListInfo::FUDDealListInfo()
{

}
FUDChatMessageInfo::FUDChatMessageInfo()
{

}

FUDDealFactionInfo::FUDDealFactionInfo()
{

}

FUDDealInfo::FUDDealInfo()
{

}

FUDDealPointMinimalInfo::FUDDealPointMinimalInfo()
{

}

FUDPointInteractionInfo::FUDPointInteractionInfo()
{

}

FUDActionInteractionInfo::FUDActionInteractionInfo()
{

}

FUDDealActionMinimalInfo::FUDDealActionMinimalInfo()
{

}

FUDPolicySelectItemInfo::FUDPolicySelectItemInfo()
{

}

#pragma endregion