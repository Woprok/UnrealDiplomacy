// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModelBase.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "UDTileOverviewViewModel.generated.h"

/**
 * Tile Overview/Managment widget
 */
UCLASS(BlueprintType, Blueprintable)
class UNREALDIPLOMACY_API UUDTileOverviewViewModel : public UUDViewModelBase
{
	GENERATED_BODY()
public:
	/**
	 * MVVM Field.
	 */
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	int32 Owner = -1;
	/**
	 * MVVM Field.
	 */
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool CanTakeTile = false;
	/**
	 * MVVM Field.
	 */
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FIntPoint Position = FIntPoint(-1, -1);
public:
	virtual void OnUpdate() override
	{
		if (ActionModel->IsGameInProgress() && Position != FIntPoint(-1,-1))
		{
			// This is visible during the game and shows current info
			UpdateTile(ActionModel->GetCurrentTileState(Position), ActionModel->CanTakeTile(Position));
		}
		else
		{
			// This should not be visible ?
		}
	}
	/**
	 * Called by button.
	 */
	UFUNCTION(BlueprintCallable)
	void ClaimTile()
	{
		ActionModel->RequestAction(ActionModel->GetTakeTileAction(Position));
	}
	UFUNCTION(BlueprintCallable)
	void SetSelectedPosition(FIntPoint newPosition)
	{
		SetPosition(newPosition);
		OnUpdate();
	}
protected:
	void UpdateTile(FUDTileInfo info, bool isOwnedByWorld)
	{
		SetOwner(info.Owner);
		SetCanTakeTile(isOwnedByWorld);
		SetPosition(info.Position);
	}
private:
	/**
	 * MVVM Binding.
	 */
	void SetOwner(int32 newOwner)
	{
		// Set checks if value changed.
		UE_MVVM_SET_PROPERTY_VALUE(Owner, newOwner);
	}
	/**
	 * MVVM Binding.
	 */
	void SetCanTakeTile(bool newCanTakeTile)
	{
		// Set checks if value changed.
		UE_MVVM_SET_PROPERTY_VALUE(CanTakeTile, newCanTakeTile);
	}
	/**
	 * MVVM Binding.
	 */
	void SetPosition(FIntPoint newPosition)
	{
		// Set checks if value changed.
		UE_MVVM_SET_PROPERTY_VALUE(Position, newPosition);
	}
	/**
	 * MVVM Binding.
	 */
	int32 GetOwner() const
	{
		return Owner;
	}
	/**
	 * MVVM Binding.
	 */
	bool GetCanTakeTile() const
	{
		return CanTakeTile;
	}
	/**
	 * MVVM Binding.
	 */
	FIntPoint GetPosition() const
	{
		return Position;
	}
};