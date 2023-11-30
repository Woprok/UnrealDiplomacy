// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/UDResourceInterface.h"
#include "UDGameResource.generated.h"

/**
 * Generic ancestor for all game resources.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDGameResource : public UObject, public IUDResourceInterface
{
	GENERATED_BODY()
public:
	virtual int32 GetId() const override { return ResourceTypeId; };
public:
	static const int32 ResourceTypeId = 1000;
};