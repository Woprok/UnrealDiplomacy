// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Components/TileView.h"
#include "UDTileView.generated.h"


// Forward Declarations

struct FUDViewModelList;

/**
 * Wrapper around tile view to enable binding collection.
 * Note: this is same as in UUDListView, but this does not inherit from it to keep it simple.
 * View needs to implement IUserObjectListEntry.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDTileView : public UTileView
{
	GENERATED_BODY()
public:
	/** Enables defining view models for list by binding. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, Category = "Bindings")
	FUDViewModelList ViewModelList;
public:
	/** Gets the view models of the widget. */
	UFUNCTION(BlueprintCallable, Category = "Widget")
	FUDViewModelList GetViewModelList() const;

	/** Sets the view models of the widget. */
	UFUNCTION(BlueprintCallable, Category = "Widget")
	virtual void SetViewModelList(const FUDViewModelList& collection);
};