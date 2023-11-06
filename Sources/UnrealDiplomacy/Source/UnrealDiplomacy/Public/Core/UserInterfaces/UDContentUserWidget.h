// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDUserWidget.h"
#include "UDContentUserWidget.generated.h"

// Forward Declarations

struct FUDViewModelContent;

/**
 * Wrapper around list view to enable binding collection.
 * View needs to implement IUserObjectListEntry.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDContentUserWidget : public UUDUserWidget
{
	GENERATED_BODY()

public:
	/** Enables defining view models for list by binding. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, Category = "Bindings")
	FUDViewModelContent ViewModelContent;
public:
	/** Gets the view models of the widget. */
	UFUNCTION(BlueprintCallable, Category = "Widget")
	FUDViewModelContent GetViewModelContent() const;

	/** Sets the view models of the widget. */
	UFUNCTION(BlueprintCallable, Category = "Widget")
	virtual void SetViewModelContent(const FUDViewModelContent& content);
};