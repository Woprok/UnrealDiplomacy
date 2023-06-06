// Copyright Miroslav Valach
// TODO Consider if we need to add update for all collection view models as well.
// TODO Rework Action Administrator into the model manager.

#pragma once

#include "CoreMinimal.h"
#include "UDViewModelManager.generated.h"

// Forward Declarations

class UUDActionAdministrator;
class UUDViewModel;

/**
 * Wrapper around items that will be in view.
 */
USTRUCT()
struct FUDViewModelCollection
{
	GENERATED_BODY()
public:
	FUDViewModelCollection() { }
	FUDViewModelCollection(FName name)
		: Name(name) { }
	UPROPERTY()
	FName Name;
	UPROPERTY()
	TArray<TObjectPtr<UUDViewModel>> Collection = { };
};

/**
 * Manager for maintaining instances of view models.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDViewModelManager : public UObject
{
	GENERATED_BODY()
public:
	/**
	 * Defines model manager that will be assigned to all viewmodels.
	 * If model is null then they will be left without manager.
	 */
	void SetModelManager(TObjectPtr<UUDActionAdministrator> model);
	/**
	 * Calls update on all global view models.
	 */
	void ForceUpdate();
	/**
	 * Retrieve global view model based on name.
	 */
	TObjectPtr<UUDViewModel> Get(const FName& name);
	/**
	 * Add new view model to list of all updated models.
	 */
	void Register(FName name, TSubclassOf<UUDViewModel> viewModelType);
	/**
	 * Retrieve global view model collection based on name.
	 */
	TArray<TObjectPtr<UUDViewModel>>& GetCollection(const FName& name);
	/**
	 * Add new view model to list of all models inside of a collection until they fit desired total count.
	 */
	void RegisterCollection(FName name, TSubclassOf<UUDViewModel> viewModelType, int32 desiredTotalCount);
protected:
	/**
	 * Initializes new instance of specified view model class.
	 */
	TObjectPtr<UUDViewModel> Create(TSubclassOf<UUDViewModel> viewModelType);
	/**
	 * Current collection of available global view models.
	 * Each is instanced exactly once.
	 */
	UPROPERTY()
	TMap<FName, TObjectPtr<UUDViewModel>> ViewModels;
	/**
	 * Current collection of available global view models.
	 * Each is instanced exactly once.
	 */
	UPROPERTY()
	TMap<FName, FUDViewModelCollection> ViewModelCollections;
	/**
	 * This model will be assigned to all view models, 
	 * allowing them to communicate with the game logic / mechanics.
	 */
	UPROPERTY()
	TObjectPtr<UUDActionAdministrator> Model = nullptr;
};