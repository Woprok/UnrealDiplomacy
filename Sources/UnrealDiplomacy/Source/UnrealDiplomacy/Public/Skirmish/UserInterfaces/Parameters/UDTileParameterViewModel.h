// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "UDTileParameterViewModel.generated.h"

// Forward Declarations

struct FUDTileParameter;

/**
 * Single editable parameter for an action.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDTileParameterViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	/**
	 * Set content of the parameter option.
	 */
	void SetContent(FUDTileParameter content);
	/** Getter for edited value. */
	TArray<int32> GetAsValueRange();
public:
	// Button Functions
	// MVVM Fields
protected:
	virtual void Initialize() override;
	virtual void Update() override;
private:
	// MVVM Setters & Getters
private:
	// Fields
	FUDTileParameter Content;
};