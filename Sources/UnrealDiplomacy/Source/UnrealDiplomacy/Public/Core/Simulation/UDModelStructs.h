// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDModelStructs.generated.h"

USTRUCT(BlueprintType)
struct FUDFactionMinimalInfo
{
	GENERATED_BODY()
public:
	FUDFactionMinimalInfo();
	FUDFactionMinimalInfo(int32 id, FString name);
	UPROPERTY(BlueprintReadOnly)
	int32 Id = 0;
	UPROPERTY(BlueprintReadOnly)
	FString Name;
};

USTRUCT(BlueprintType)
struct FUDStratagemPickableInfo
{
	GENERATED_BODY()
public:
	FUDStratagemPickableInfo();
	FUDStratagemPickableInfo(int32 id, FString name, int32 cost, bool isSelected);
	UPROPERTY(BlueprintReadOnly)
	int32 Id = 0;
	UPROPERTY(BlueprintReadOnly)
	FString Name;
	UPROPERTY(BlueprintReadOnly)
	int32 Cost;
	UPROPERTY(BlueprintReadOnly)
	bool IsSelected;
};