// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "UDTileParameterViewModel.generated.h"

// Forward Declarations

struct FUDTileMinimalInfo;
struct FUDTileParameter;

DECLARE_MULTICAST_DELEGATE(FUDParameterChanged);

/**
 * Single editable parameter for an action.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDTileParameterViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	/** Set content of the parameter option. */
	void SetContent(FUDTileParameter content);
	/** Getter for edited value. */
	TArray<int32> GetAsValueRange();
	/** Getter for edited value. */
	FIntPoint GetAsValuePoint();
public:
	// Button Functions
	UFUNCTION()
	void PreviousTile();
	UFUNCTION()
	void NextTile();
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText TileTitleText;
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
public:
	virtual void Setup() override;
	virtual void Refresh() override;
private:
	/** 
	 * Tries to retrieve desired selected item, on fail returns first element.
	 * If options list is empty returns invalid item.
	 */
	FUDTileMinimalInfo GetSelectedOrDefault(FIntPoint desiredSelectedItem);
private:
	void ChangeAttempted();
private:
	// MVVM Setters & Getters
	void SetTileTitleText(FText newTileTitleText);
	FText GetTileTitleText() const;
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
	FUDTileParameter Content;
	FIntPoint SelectedTile;
	int32 SelectedTileIndex;
};