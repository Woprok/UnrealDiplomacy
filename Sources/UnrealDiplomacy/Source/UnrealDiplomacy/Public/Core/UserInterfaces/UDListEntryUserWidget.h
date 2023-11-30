// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "UDListEntryUserWidget.generated.h"

/**
 * Ancestor for blueprint.
 * Supports IUserObjectListEntry for list view initializations.
 * Thus, it will call SetViewModel, whenever item is set.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDListEntryUserWidget : public UUDUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
public:
	/** Allows List View to define context. */
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
};