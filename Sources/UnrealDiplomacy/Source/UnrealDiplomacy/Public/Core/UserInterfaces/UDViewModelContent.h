// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDViewModelContent.generated.h"

// Forward Declarations

class UUDViewModel;

/**
 * Wrapper around pointer to single view model.
 * Note: this is required as UE macros can't be used with templates.
 */
USTRUCT(BlueprintType)
struct FUDViewModelContent
{
	GENERATED_BODY()
public:
	FUDViewModelContent();
	FUDViewModelContent(TObjectPtr<UUDViewModel> viewModel);
	TObjectPtr<UUDViewModel> ViewModel;
public:
	/** Equality over UniqueId field. */
	inline bool operator!=(const FUDViewModelContent& rhs) const
	{
		return !(*this == rhs);
	}
	/** Equality over UniqueId field. */
	inline bool operator==(const FUDViewModelContent& rhs) const
	{
		return ViewModel == rhs.ViewModel;
	}
};