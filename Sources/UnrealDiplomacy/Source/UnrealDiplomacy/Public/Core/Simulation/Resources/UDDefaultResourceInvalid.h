// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/UDResourceInterface.h"
#include "UDDefaultResourceInvalid.generated.h"

/**
 * Default resource that serves as fallback.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDDefaultResourceInvalid : public UObject, public IUDResourceInterface
{
	GENERATED_BODY()
public:
	virtual int32 GetId() const override;
};