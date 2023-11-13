// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/UserInterfaces/UDViewModelContent.h"
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

DECLARE_MULTICAST_DELEGATE_OneParam(FUDDealActionUpdated, int32 parameter);
DECLARE_MULTICAST_DELEGATE_OneParam(FUDInvokerUpdated, int32 parameter);
DECLARE_MULTICAST_DELEGATE_OneParam(FUDValuesUpdated, TArray<int32> parameters);
DECLARE_MULTICAST_DELEGATE_OneParam(FUDTextUpdated, FString parameter);

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
	bool HasDealActionParameterValue;
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

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FUDViewModelContent DealActionParameterContent;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FUDViewModelContent FactionInvokerParameterContent;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FUDViewModelContent FactionTargetParameterContent;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FUDViewModelContent TileParameterContent;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FUDViewModelContent ActionParameterContent;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FUDViewModelContent ResourceParameterContent;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FUDViewModelContent ValueParameterContent;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FUDViewModelContent TextParameterContent;

	FUDDealActionUpdated DealActionUpdated;
	FUDInvokerUpdated InvokerUpdated;
	FUDValuesUpdated ValuesUpdated;
	FUDTextUpdated TextUpdated;
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
	void UpdateDealActionParameter(const FUDActionParameter& parameter);
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
	void DefineDealActionParameter(int32 id);
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
	void OnDealActionParameterChanged();
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
	void ResolveValueParameterChange();
private:
	// MVVM Setters & Getters
	void SetHasDealActionParameterValue(bool newHasDealActionParameterValue);
	bool GetHasDealActionParameterValue() const;
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

	void SetDealActionParameterContent(FUDViewModelContent newContent);
	FUDViewModelContent GetDealActionParameterContent() const;
	void SetFactionInvokerParameterContent(FUDViewModelContent newContent);
	FUDViewModelContent GetFactionInvokerParameterContent() const;
	void SetFactionTargetParameterContent(FUDViewModelContent newContent);
	FUDViewModelContent GetFactionTargetParameterContent() const;
	void SetTileParameterContent(FUDViewModelContent newContent);
	FUDViewModelContent GetTileParameterContent() const;
	void SetActionParameterContent(FUDViewModelContent newContent);
	FUDViewModelContent GetActionParameterContent() const;
	void SetResourceParameterContent(FUDViewModelContent newContent);
	FUDViewModelContent GetResourceParameterContent() const;
	void SetValueParameterContent(FUDViewModelContent newContent);
	FUDViewModelContent GetValueParameterContent() const;
	void SetTextParameterContent(FUDViewModelContent newContent);
	FUDViewModelContent GetTextParameterContent() const;
private:
	// Fields
	FUDParameterListInfo Content;

	// Current Instances in use...
	FName DealActionParameterInstanceName = TEXT("DealActionParameterInstance");
	TSubclassOf<UUDViewModel> DealActionParameterType;
	TObjectPtr<UUDActionParameterViewModel> DealActionParameterInstance;
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