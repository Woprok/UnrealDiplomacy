// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/UDResourceInterface.h"
#include "UDDefaultResource.generated.h"

/**
 * Default resource that serves as fallback.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDDefaultResource : public UObject, public IUDResourceInterface
{
	GENERATED_BODY()
public:
	virtual int32 GetId() const override { return ResourceTypeId; };
public:
	static const int32 ResourceTypeId = 0;
};