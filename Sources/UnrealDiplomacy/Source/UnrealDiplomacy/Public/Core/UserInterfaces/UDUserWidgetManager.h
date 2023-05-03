// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDUserWidgetManager.generated.h"

// Forward Declarations

class UUDUserWidget;

/**
 * Manager for maintaining instances of views.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDUserWidgetManager : public UObject
{
	GENERATED_BODY()
public:
	/**
	 * Retrieve global view based on name.
	 */
	TObjectPtr<UUDUserWidget> Get(const FName& name);
	/**
	 * Add new view to list of all global views.
	 */
	void Register(FName name, TSubclassOf<UUDUserWidget> viewType);
	/**
	 * Adds selected widget to viewport.
	 */
	virtual void ShowWidget(const FName& name);
	/**
	 * Removes selected widget from viewport.
	 */
	virtual void HideWidget(const FName& name);
protected:
	/**
	 * Initializes new instance of specified view model class.
	 */
	TObjectPtr<UUDUserWidget> Create(TSubclassOf<UUDUserWidget> viewType);
	/**
	 * Current collection of available global views.
	 * Each is instanced exactly once.
	 */
	UPROPERTY()
	TMap<FName, TObjectPtr<UUDUserWidget>> Views;
};