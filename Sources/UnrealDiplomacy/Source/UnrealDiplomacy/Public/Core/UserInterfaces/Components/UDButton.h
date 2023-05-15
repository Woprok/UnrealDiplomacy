// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "UDButton.generated.h"

// Forward Declarations

class UTextBlock;

/**
 * TODO custom UI elements to simplify workflow.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDButton : public UButton
{
	GENERATED_BODY()
public:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	UPROPERTY()
	TObjectPtr<UTextBlock> ButtonText;
};