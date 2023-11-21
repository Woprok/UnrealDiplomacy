// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "UDClientItemViewModel.generated.h"

// Forward Declarations

struct FUDFactionMinimalInfo;

/**
 * ViewModel for single joined client used in list view.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDClientItemViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	/** Set content of the strategy option. */
	void SetContent(FUDFactionMinimalInfo content);
public:
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText NameText;
public:
	virtual void Setup() override;
	virtual void Refresh() override;
private:
	// MVVM Setters & Getters
	void SetNameText(FText newNameText);
	FText GetNameText() const;
private:
	// Fields
	FUDFactionMinimalInfo Content;
};