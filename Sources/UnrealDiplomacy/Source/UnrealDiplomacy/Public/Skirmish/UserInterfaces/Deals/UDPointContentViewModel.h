// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "UDPointContentViewModel.generated.h"

// Forward Declarations

struct FUDDealPointMinimalInfo;
class UUDParameterEditorViewModel;

DECLARE_MULTICAST_DELEGATE_OneParam(FUDParameterEditorChanged, const TObjectPtr<UUDParameterEditorViewModel>& editorViewModel);

/**
 * Single faction in a list.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDPointContentViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	/**
	 * Set content of the strategy option.
	 */
	void SetContent(FUDDealPointMinimalInfo content);
public:
	// Button Functions
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText PointTitleText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool IsValidContentValue;
	// Events
	FUDParameterEditorChanged ParameterEditorChangedEvent;
protected:
	virtual void Initialize() override;
	virtual void Update() override;
private:
	/** Updates all parameter lists. */
	void UpdateEditor();
	/** Defines all parameter instances. */
	void DefineInstances();
private:
	// MVVM Setters & Getters
	void SetPointTitleText(FText newPointTitleText);
	FText GetPointTitleText() const;
	void SetIsValidContentValue(bool newIsValidContentValue);
	bool GetIsValidContentValue() const;
private:
	// Fields
	FUDDealPointMinimalInfo Content;
	// Current Instance in use...
	FName ParameterEditorInstanceName = TEXT("PointParameterEditorInstance");
	TSubclassOf<UUDViewModel> ParameterEditorType;
	TObjectPtr<UUDParameterEditorViewModel> ParameterEditorInstance;
private:
	// Ensure uniqueness of all collections used...
	bool IsUniqueNameDefined = false;
	// Static trick to always have unique names for each instance.
	static int32 UniqueNameDefinition;
};