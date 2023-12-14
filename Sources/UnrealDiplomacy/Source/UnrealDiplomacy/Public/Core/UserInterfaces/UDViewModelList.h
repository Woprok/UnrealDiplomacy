// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDViewModelList.generated.h"

// Forward Declarations

class UUDViewModel;

/**
 * Wrapper around pointer to list of view models.
 */
USTRUCT(BlueprintType)
struct FUDViewModelList
{
	GENERATED_BODY()
public:
	FUDViewModelList();
	FUDViewModelList(TArray<TObjectPtr<UUDViewModel>> viewModels);
	TArray<TObjectPtr<UUDViewModel>> ViewModels;
public:
	/** Equality over UniqueId field. */
	inline bool operator!=(const FUDViewModelList& rhs) const
	{
		return !(*this == rhs);
	}
	/** Equality over UniqueId field. */
	inline bool operator==(const FUDViewModelList& rhs) const
	{
		return ViewModels == rhs.ViewModels;
	}
};