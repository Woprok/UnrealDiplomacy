// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Components/ListView.h"
#include "Core/UserInterfaces/UDViewModelList.h"
#include "UDListView.generated.h"

// Forward Declarations

struct FUDViewModelList;

/**
 * Wrapper around list view to enable binding collection.
 * View needs to implement IUserObjectListEntry.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDListView : public UListView
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