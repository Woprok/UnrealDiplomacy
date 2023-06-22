// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "UDModifierItemViewModel.generated.h"

// Forward Declarations

struct FUDModifierInfo;

/**
 * Single resource in a list.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDModifierItemViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	/**
	 * Set content of the modifier.
	 */
	void SetContent(FUDModifierInfo content);
public:
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText NameText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText DescriptionText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	int32 IdValue;
protected:
	virtual void Initialize() override;
	virtual void Update() override;
private:
	// MVVM Setters & Getters
	void SetNameText(FText newNameText);
	FText GetNameText() const;
	void SetDescriptionText(FText newDescriptionText);
	FText GetDescriptionText() const;
	void SetIdValue(int32 newIdValue);
	int32 GetIdValue() const;
private:
	// Fields
	FUDModifierInfo Content;
};