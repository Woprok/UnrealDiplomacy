// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "UDValueParameterViewModel.generated.h"

// Forward Declarations

struct FUDValueParameter;

/**
 * Single editable parameter for an action.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDValueParameterViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	/**
	 * Set content of the parameter option.
	 */
	void SetContent(FUDValueParameter content);
	/** Getter for edited value. */
	int32 GetAsValue();
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
	FUDValueParameter Content;
};