// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/UDActionInterface.h"
#include "UDDealAction.generated.h"

UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDDealAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual int32 GetId() const override { return ActionTypeId; };
	virtual int32 GetParameterCount() const override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 10000;
	static const int32 RequiredParametersCount = 0;
};