// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "UDParameterEditorViewModel.generated.h"

// Forward Declarations

struct FUDParameterListInfo;
struct FUDFactionParameter;
struct FUDTileParameter;
struct FUDResourceParameter;
struct FUDActionParameter;
struct FUDValueParameter;
struct FUDTextParameter;

class UUDTextParameterViewModel;
class UUDFactionParameterViewModel;
class UUDActionParameterViewModel;
class UUDTileParameterViewModel;
class UUDValueParameterViewModel;
class UUDResourceParameterViewModel;

DECLARE_MULTICAST_DELEGATE_OneParam(FactionParameterUpdated, const TArray<TObjectPtr<UUDFactionParameterViewModel>>& parameterViewModels);
DECLARE_MULTICAST_DELEGATE_OneParam(ActionParameterUpdated, const TArray<TObjectPtr<UUDActionParameterViewModel>>& parameterViewModels);
DECLARE_MULTICAST_DELEGATE_OneParam(TileParameterUpdated, const TArray<TObjectPtr<UUDTileParameterViewModel>>& parameterViewModels);
DECLARE_MULTICAST_DELEGATE_OneParam(ValueParameterUpdated, const TArray<TObjectPtr<UUDValueParameterViewModel>>& parameterViewModels);
DECLARE_MULTICAST_DELEGATE_OneParam(TextParameterUpdated, const TArray<TObjectPtr<UUDTextParameterViewModel>>& parameterViewModels);
DECLARE_MULTICAST_DELEGATE_OneParam(ResourceParameterUpdated, const TArray<TObjectPtr<UUDResourceParameterViewModel>>& parameterViewModels);

/**
 * Editor for parameters that belong to single action.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDParameterEditorViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	/**
	 * Set content of the parameter editor.
	 */
	void SetContent(FUDParameterListInfo content);
	/**
	 * Retrieves all value parameters from the editor items.
	 */
	TArray<int32> GetValueParameters();
	/**
	 * Retrieves all value parameters from the editor items.
	 */
	TArray<FString> GetTextParameters();
public:
	// Button Functions
	// MVVM Fields
	// Events
	FactionParameterUpdated FactionParameterUpdatedEvent;
	ActionParameterUpdated ActionParameterUpdatedEvent;
	TileParameterUpdated TileParameterUpdatedEvent;
	ValueParameterUpdated ValueParameterUpdatedEvent;
	TextParameterUpdated TextParameterUpdatedEvent;
	ResourceParameterUpdated ResourceParameterUpdatedEvent;
protected:
	virtual void Initialize() override;
	virtual void Update() override;
private:
	/** Updates all parameter lists. */
	void UpdateParameterLists();
	/** Defines all parameter collections. */
	void DefineCollections();
	/** Updates specific parameter list. */
	void UpdateFactionParameters(const TArray<FUDFactionParameter>& parameters);
	/** Updates specific parameter list. */
	void UpdateTextParameters(const TArray<FUDTextParameter>& parameters);
	/** Updates specific parameter list. */
	void UpdateValueParameters(const TArray<FUDValueParameter>& parameters);
	/** Updates specific parameter list. */
	void UpdateTileParameters(const TArray<FUDTileParameter>& parameters);
	/** Updates specific parameter list. */
	void UpdateActionParameters(const TArray<FUDActionParameter>& parameters);
	/** Updates specific parameter list. */
	void UpdateResourceParameters(const TArray<FUDResourceParameter>& parameters);
private:
	// MVVM Setters & Getters
private:
	// Fields
	FUDParameterListInfo Content;

	// Current Collection in use...
	FName FactionParameterCollectionName = TEXT("FactionParameterCollection");
	TSubclassOf<UUDViewModel> FactionParameterType;
	TArray<TObjectPtr<UUDFactionParameterViewModel>> FactionParameterCollection;
	FName TileParameterCollectionName = TEXT("TileParameterCollection");
	TSubclassOf<UUDViewModel> TileParameterType;
	TArray<TObjectPtr<UUDTileParameterViewModel>> TileParameterCollection;
	FName ActionParameterCollectionName = TEXT("ActionParameterCollection");
	TSubclassOf<UUDViewModel> ActionParameterType;
	TArray<TObjectPtr<UUDActionParameterViewModel>> ActionParameterCollection;
	FName ResourceParameterCollectionName = TEXT("ResourceParameterCollection");
	TSubclassOf<UUDViewModel> ResourceParameterType;
	TArray<TObjectPtr<UUDResourceParameterViewModel>> ResourceParameterCollection;
	FName ValueParameterCollectionName = TEXT("ValueParameterCollection");
	TSubclassOf<UUDViewModel> ValueParameterType;
	TArray<TObjectPtr<UUDValueParameterViewModel>> ValueParameterCollection;
	FName TextParameterCollectionName = TEXT("TextParameterCollection");
	TSubclassOf<UUDViewModel> TextParameterType;
	TArray<TObjectPtr<UUDTextParameterViewModel>> TextParameterCollection;
private:
	// Ensure uniqueness of all collections used...
	bool IsUniqueNameDefined = false;
	// Static trick to always have unique names for each instance.
	static int32 UniqueNameDefinition;
};