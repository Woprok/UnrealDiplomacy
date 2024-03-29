// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UDHUD.generated.h"

// Forward Declarations

class UUDUserWidget;
class UUDViewModel;
class UUDViewModelManager;
class UUDUserWidgetManager;

/**
 * Single widget located inside of a screen.
 */
USTRUCT(Blueprintable, BlueprintType)
struct FUDWidgetInfo
{
	GENERATED_BODY()
public:
	FUDWidgetInfo() {}
	UPROPERTY(EditAnywhere)
	FName Name;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUDUserWidget> ViewType;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUDViewModel> ViewModelType;
};

/**
 * Switchable HUD screen.
 */
USTRUCT(Blueprintable, BlueprintType)
struct FUDScreenInfo
{
	GENERATED_BODY()
public:
	FUDScreenInfo() {}
	UPROPERTY(EditAnywhere)
	FName Name;
	UPROPERTY(EditAnywhere)
	TArray<FUDWidgetInfo> Widgets;
};

/**
 * Defines screen switching for all HUDs.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API AUDHUD : public AHUD
{
	GENERATED_BODY()
public:
	/**
	 * Shortcut to retrieve casted MenuHUD.
	 * Requries World pointer that can be retrieved by GetWorld().
	 */
	static TObjectPtr<AUDHUD> Get(TObjectPtr<UWorld> world);
	/**
	 * Switches from current screen to desired screen.
	 * Useful for reloading and static models.
	 */
	UFUNCTION(BlueprintCallable)
	void SwitchScreen(const FName& screenName);
	/**
	 * Safely initializes all dependencies.
	 */
	UFUNCTION(BlueprintCallable)
	virtual void Initialize();
	/**
	 * Show specified widget on the viewport.
	 * Returns view model that is associated with the widget.
	 * This expects view and view model to be registered under same name.
	 */
	TObjectPtr<UUDViewModel> ShowWidget(const FName& name);
	/**
	 * Hides specified widget from the viewport.
	 */
	void HideWidget(const FName& name);
	/**
	 * Obtains view model collection of desired size.
	 * If collection does not exists it will be created.
	 * If collection is not large enough it will be expanded.
	 * Use this for filling ListViews or similiar use-cases.
	 * Returns resized copy of the original collection.
	 * Original collection is never shrinked, only expanded.
	 */
	TArray<TObjectPtr<UUDViewModel>> GetViewModelCollection(const FName& name, TSubclassOf<UUDViewModel> viewModelType, int32 desiredTotalCount);
	/**
	 * Shortcut from retrieving single view model from the collection.
	 */
	TObjectPtr<UUDViewModel> GetViewModelCollection(const FName& name, TSubclassOf<UUDViewModel> viewModelType);
protected:
	/**
	 * Adds selected screen to viewport.
	 */
	bool ShowScreen(const FName& screenName);
	/**
	 * Removes selected screen from viewport.
	 */
	void HideScreen(const FName& screenName);
	/**
	 * Creates all screens based on their blueprint definitions.
	 */
	void PrepareAllScreens();
	/**
	 * Creates single screen from its definition.
	 */
	void PrepareScreen(const FUDScreenInfo& screen);
	/**
	 * Creates widget from the definition.
	 */
	void PrepareWidget(const FUDWidgetInfo& widget);
	/**
	 * All screens that can be displayed by this HUD.
	 */
	UPROPERTY(EditAnywhere)
	TMap<FName, FUDScreenInfo> Screens;
	/**
	 * All views associated with this HUD.
	 */
	UPROPERTY()
	TObjectPtr<UUDUserWidgetManager> ViewManager;
	/**
	 * All view models associated with this HUD.
	 */
	UPROPERTY()
	TObjectPtr<UUDViewModelManager> ViewModelManager;
	/**
	 * Right after initializing all core components, and before creating any screen.
	 * Enables to add custom logic, such as defining ViewModelManager model.
	 */
	virtual void OnComponentsInitialized();
private:
	/**
	 * Determines if this was initialized and can be used.
	 */
	UPROPERTY()
	bool IsInitialized = false;
	/**
	 * Last screen that was assigned to viewport.
	 */
	UPROPERTY()
	FName CurrentScreenName;
};