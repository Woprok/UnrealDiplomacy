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

FUDRegencyTurnInfo::FUDRegencyTurnInfo() 
{

}

#pragma region Resources
#define LOCTEXT_NAMESPACE "Resource"
FUDResourceInfo::FUDResourceInfo()
{

}

FUDResourceInfo FUDResourceInfo::GetReputation(int32 amount)
{
	FUDResourceInfo resource = FUDResourceInfo();
	resource.Id = UD_RESOURCE_REPUTATION_ID;
	resource.Name = FText(LOCTEXT("Resource", "Reputation")).ToString();
	resource.Amount = amount;

	return resource;
}

FUDResourceInfo FUDResourceInfo::GetGold(int32 amount)
{
	FUDResourceInfo resource = FUDResourceInfo();
	resource.Id = UD_RESOURCE_GOLD_ID;
	resource.Name = FText(LOCTEXT("Resource", "Gold")).ToString();
	resource.Amount = amount;

	return resource;
}

FUDResourceInfo FUDResourceInfo::GetFood(int32 amount)
{
	FUDResourceInfo resource = FUDResourceInfo();
	resource.Id = UD_RESOURCE_FOOD_ID;
	resource.Name = FText(LOCTEXT("Resource", "Food")).ToString();
	resource.Amount = amount;

	return resource;
}

FUDResourceInfo FUDResourceInfo::GetMaterials(int32 amount)
{
	FUDResourceInfo resource = FUDResourceInfo();
	resource.Id = UD_RESOURCE_MATERIALS_ID;
	resource.Name = FText(LOCTEXT("Resource", "Materials")).ToString();
	resource.Amount = amount;

	return resource;
}

FUDResourceInfo FUDResourceInfo::GetLuxuries(int32 amount)
{
	FUDResourceInfo resource = FUDResourceInfo();
	resource.Id = UD_RESOURCE_LUXURIES_ID;
	resource.Name = FText(LOCTEXT("Resource", "Luxuries")).ToString();
	resource.Amount = amount;

	return resource;
}

#undef LOCTEXT_NAMESPACE
#pragma endregion