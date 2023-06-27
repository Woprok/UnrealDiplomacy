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

DECLARE_MULTICAST_DELEGATE_OneParam(FactionParameterUpdated, const TObjectPtr<UUDFactionParameterViewModel>& parameterViewModel);
DECLARE_MULTICAST_DELEGATE_OneParam(ActionParameterUpdated, const TObjectPtr<UUDActionParameterViewModel>& parameterViewModel);
DECLARE_MULTICAST_DELEGATE_OneParam(TileParameterUpdated, const TObjectPtr<UUDTileParameterViewModel>& parameterViewModel);
DECLARE_MULTICAST_DELEGATE_OneParam(ValueParameterUpdated, const TObjectPtr<UUDValueParameterViewModel>& parameterViewModel);
DECLARE_MULTICAST_DELEGATE_OneParam(TextParameterUpdated, const TObjectPtr<UUDTextParameterViewModel>& parameterViewModel);
DECLARE_MULTICAST_DELEGATE_OneParam(ResourceParameterUpdated, const TObjectPtr<UUDResourceParameterViewModel>& parameterViewModel);

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
	 * Retrieves text parameter from the editor items.
	 */
	FString GetTextParameter();
public:
	// Button Functions
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool HasFactionInvokerParameterValue;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool HasFactionTargetParameterValue;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool HasTileParameterValue;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool HasActionParameterValue;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool HasResourceParameterValue;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool HasValueParameterValue;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool HasTextParameterValue;
	// Events
	FactionParameterUpdated FactionInvokerParameterUpdatedEvent;
	FactionParameterUpdated FactionTargetParameterUpdatedEvent;
	ActionParameterUpdated ActionParameterUpdatedEvent;
	TileParameterUpdated TileParameterUpdatedEvent;
	ValueParameterUpdated ValueParameterUpdatedEvent;
	TextParameterUpdated TextParameterUpdatedEvent;
	ResourceParameterUpdated ResourceParameterUpdatedEvent;
protected:
	virtual void Initialize() override;
	virtual void Update() override;
private:
	/** Hide all parameter. */
	void HideParameters();
	/** Updates all parameters. */
	void UpdateParameterInstances();
	/** Defines all parameters instances. */
	void DefineInstances();
	/** Updates specific parameter. */
	void UpdateFactionInvokerParameter(const FUDFactionParameter& parameter);
	/** Updates specific parameter. */
	void UpdateFactionTargetParameter(const FUDFactionParameter& parameter);
	/** Updates specific parameter. */
	void UpdateTileParameter(const FUDTileParameter& parameter);
	/** Updates specific parameter. */
	void UpdateActionParameter(const FUDActionParameter& parameter);
	/** Updates specific parameter. */
	void UpdateResourceParameter(const FUDResourceParameter& parameter);
	/** Updates specific parameter. */
	void UpdateValueParameter(const FUDValueParameter& parameter);
	/** Updates specific parameter. */
	void UpdateTextParameter(const FUDTextParameter& parameter);
	/** Defines specific parameter. */
	void DefineFactionInvokerParameter(int32 id);
	/** Defines specific parameter. */
	void DefineFactionTargetParameter(int32 id);
	/** Defines specific parameter. */
	void DefineTileParameter(int32 id);
	/** Defines specific parameter. */
	void DefineActionParameter(int32 id);
	/** Defines specific parameter. */
	void DefineResourceParameter(int32 id);
	/** Defines specific parameter. */
	void DefineValueParameter(int32 id);
	/** Defines specific parameter. */
	void DefineTextParameter(int32 id);
	/** Recognizes specific parameter update. */
	void OnFactionInvokerParameterChanged();
	/** Recognizes specific parameter update. */
	void OnFactionTargetParameterChanged();
	/** Recognizes specific parameter update. */
	void OnTileParameterChanged();
	/** Recognizes specific parameter update. */
	void OnActionParameterChanged();
	/** Recognizes specific parameter update. */
	void OnResourceParameterChanged();
	/** Recognizes specific parameter update. */
	void OnValueParameterChanged();
	/** Recognizes specific parameter update. */
	void OnTextParameterChanged();
private:
	// MVVM Setters & Getters
	void SetHasFactionInvokerParameterValue(bool newHasFactionInvokerParameterValue);
	bool GetHasFactionInvokerParameterValue() const;
	void SetHasFactionTargetParameterValue(bool newHasFactionTargetParameterValue);
	bool GetHasFactionTargetParameterValue() const;
	void SetHasTileParameterValue(bool newHasTileParameterValue);
	bool GetHasTileParameterValue() const;
	void SetHasActionParameterValue(bool newHasActionParameterValue);
	bool GetHasActionParameterValue() const;
	void SetHasResourceParameterValue(bool newHasResourceParameterValue);
	bool GetHasResourceParameterValue() const;
	void SetHasValueParameterValue(bool newHasValueParameterValue);
	bool GetHasValueParameterValue() const;
	void SetHasTextParameterValue(bool newHasTextParameterValue);
	bool GetHasTextParameterValue() const;
private:
	// Fields
	FUDParameterListInfo Content;

	// Current Instances in use...
	FName FactionInvokerParameterInstanceName = TEXT("FactionInvokerParameterInstance");
	TSubclassOf<UUDViewModel> FactionInvokerParameterType;
	TObjectPtr<UUDFactionParameterViewModel> FactionInvokerParameterInstance;
	FName FactionTargetParameterInstanceName = TEXT("FactionTargetParameterInstance");
	TSubclassOf<UUDViewModel> FactionTargetParameterType;
	TObjectPtr<UUDFactionParameterViewModel> FactionTargetParameterInstance;
	FName TileParameterInstanceName = TEXT("TileParameterInstance");
	TSubclassOf<UUDViewModel> TileParameterType;
	TObjectPtr<UUDTileParameterViewModel> TileParameterInstance;
	FName ActionParameterInstanceName = TEXT("ActionParameterInstance");
	TSubclassOf<UUDViewModel> ActionParameterType;
	TObjectPtr<UUDActionParameterViewModel> ActionParameterInstance;
	FName ResourceParameterInstanceName = TEXT("ResourceParameterInstance");
	TSubclassOf<UUDViewModel> ResourceParameterType;
	TObjectPtr<UUDResourceParameterViewModel> ResourceParameterInstance;
	FName ValueParameterInstanceName = TEXT("ValueParameterInstance");
	TSubclassOf<UUDViewModel> ValueParameterType;
	TObjectPtr<UUDValueParameterViewModel> ValueParameterInstance;
	FName TextParameterInstanceName = TEXT("TextParameterInstance");
	TSubclassOf<UUDViewModel> TextParameterType;
	TObjectPtr<UUDTextParameterViewModel> TextParameterInstance;
private:
	// Ensure uniqueness of all instances used...
	bool IsUniqueNameDefined = false;
	// Static trick to always have unique names for each instance.
	static int32 UniqueNameDefinition;
};