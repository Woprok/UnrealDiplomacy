// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDUserWidget.h"
#include "UDNewsWidget.generated.h"

// Forward Declaratiosn

class UTextBlock;
class UButton;
class UUDNewsViewModel;

/**
 * Ancestor for blueprint.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDNewsWidget : public UUDUserWidget
{
	GENERATED_BODY()
public:
	// FOR ALL 
	// SET MY MODEL - > SET THIS MODEL
	// BUTTON CAN CALL LOCAL FNC LIKE ONCLOSE THAT USES VM
	// ALL LOGIC IS IN C+++
	/**
	 * Invoked for each view exactly once to set a persistent view model reference.
	 */
	 //UFUNCTION(BlueprintImplementableEvent)
	 //void SetViewModel();

	// PER WIDGET
	UFUNCTION(BlueprintImplementableEvent)
	void SetNewsModel(UUDNewsViewModel* model);
public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> TitleText;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> MessageText;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> CloseButton;


	// SET THIS MODEL
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UUDNewsViewModel> ViewModel;
};