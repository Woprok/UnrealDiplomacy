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
	FText FactionNameText;
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
	void SetFactionNameText(FText newFactionNameText);
	FText GetFactionNameText() const;
	void SetCloseText(FText newCloseText);
	FText GetCloseText() const;
private:
	// Fields
	FName TileInteractionViewModelCollectionName = TEXT("TileInteractionCollection");
	TSubclassOf<UUDViewModel> TileInteractionViewModelType;
	TArray<TObjectPtr<UUDTileInteractionViewModel>> TileInteractionViewModelCollection;

	FIntPoint SelectedTile;
/*/
	int32 Owner = -1;
	bool CanTakeTile = false; // owned by world
	FIntPoint Position = FIntPoint(-1, -1);
	int32 Type = 0;
	// This is visible during the game and shows current info
	UpdateTile(ActionModel->GetCurrentTileState(Position), ActionModel->CanTakeTile(Position));
	ActionModel->RequestAction(ActionModel->GetAction(UUDGameActionTileTake::ActionTypeId, { Owner, Position.X, Position.Y }));*/
};