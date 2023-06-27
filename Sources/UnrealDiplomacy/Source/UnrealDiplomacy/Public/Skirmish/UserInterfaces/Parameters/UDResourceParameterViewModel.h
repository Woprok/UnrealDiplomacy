// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "UDResourceParameterViewModel.generated.h"

// Forward Declarations

struct FUDResourceMinimalInfo;
struct FUDResourceParameter;

DECLARE_MULTICAST_DELEGATE(FUDParameterChanged);

/**
 * Single editable parameter for an action.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDResourceParameterViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	/** Set content of the parameter option. */
	void SetContent(FUDResourceParameter content);
	/** Getter for edited value. */
	int32 GetAsValue();
public:
	// Button Functions
	UFUNCTION()
	void PreviousResource();
	UFUNCTION()
	void NextResource();
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText ResourceTitleText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText NameText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText ToolTipText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText PreviousText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText NextText;
	// Events
	FUDParameterChanged OnChangeEvent;
protected:
	virtual void Initialize() override;
	virtual void Update() override;
private:
	/**
	 * Tries to retrieve desired selected item, on fail returns first element.
	 * If options list is empty returns invalid item.
	 */
	FUDResourceMinimalInfo GetSelectedOrDefault(int32 desiredSelectedItem);
private:
	// MVVM Setters & Getters
	void SetResourceTitleText(FText newResourceTitleText);
	FText GetResourceTitleText() const;
	void SetNameText(FText newNameText);
	FText GetNameText() const;
	void SetToolTipText(FText newToolTipText);
	FText GetToolTipText() const;
	void SetPreviousText(FText newPreviousText);
	FText GetPreviousText() const;
	void SetNextText(FText newNextText);
	FText GetNextText() const;
private:
	// Fields
	FUDResourceParameter Content;
	int32 SelectedResource;
	int32 SelectedResourceIndex;
};