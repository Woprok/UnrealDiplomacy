// Copyright Miroslav Valach
// TODO Evalutate solution based on Tile having Id, this would make action interaction same as Faction.

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "UDTileManagementViewModel.generated.h"

// Forward Declarations

class UUDTileInteractionViewModel;

DECLARE_MULTICAST_DELEGATE_OneParam(FUDTileInteractionSourceUpdated, const TArray<TObjectPtr<UUDTileInteractionViewModel>>& tileInteractionViewModels);

/**
 * Window that allows interacting with faction and view informations.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDTileManagementViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	/**
	 * Self handle for faction selection event.
	 */
	UFUNCTION()
	void OnTileSelected(FIntPoint selectedTile);
public:
	// Button Functions
	UFUNCTION()
	void Close();
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText TileManagementTitleText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText TileNameText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText OwnerText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText FactionNameText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText ResourceText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText ResourceValueText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText ResourceTypeText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText CloseText;
	// Events
	FUDTileInteractionSourceUpdated TileInteractionSourceUpdatedEvent;
protected:
	virtual void Initialize() override;
	UFUNCTION()
	virtual void Update() override;
	UFUNCTION()
	void Reload();
private:
	/**
	 * Updates available interaction with the tile.
	 */
	void UpdateTileInteractionList();
private:
	// MVVM Setters & Getters
	void SetTileManagementTitleText(FText newTileManagementTitleText);
	FText GetTileManagementTitleText() const;
	void SetTileNameText(FText newTileNameText);
	FText GetTileNameText() const;
	void SetOwnerText(FText newOwnerText);
	FText GetOwnerText() const;
	void SetFactionNameText(FText newFactionNameText);
	FText GetFactionNameText() const;
	void SetResourceText(FText newResourceText);
	FText GetResourceText() const;
	void SetResourceTypeText(FText newResourceTypeText);
	FText GetResourceTypeText() const;
	void SetResourceValueText(FText newResourceValueText);
	FText GetResourceValueText() const;
	void SetCloseText(FText newCloseText);
	FText GetCloseText() const;
private:
	// Fields
	FName TileInteractionViewModelCollectionName = TEXT("TileInteractionCollection");
	TSubclassOf<UUDViewModel> TileInteractionViewModelType;
	TArray<TObjectPtr<UUDTileInteractionViewModel>> TileInteractionViewModelCollection;

	FIntPoint SelectedTile;
};