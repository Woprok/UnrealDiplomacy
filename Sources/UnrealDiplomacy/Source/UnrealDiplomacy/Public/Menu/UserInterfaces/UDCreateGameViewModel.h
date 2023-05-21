// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "UDCreateGameViewModel.generated.h"

// Forward Declarations

enum class ECheckBoxState : uint8;

/**
 * ViewModel for creating game.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDCreateGameViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	/**
	 * Initializes new game session.
	 */
	void NewGame();

public:
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText CreateGameTitleText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText BackText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText NewGameText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText SessionNameText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText SessionNameEditableText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText IsLANText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	ECheckBoxState IsLANValue;
	/**
	 * Binding for a SessionName
	 */
	void SetSessionName(FText inputText);
protected:
	/**
	 * Invoked for each update.
	 */
	virtual void Update() override;
	/**
	 * Invoked for each update.
	 */
	virtual void Load();
private:
	// MVVM Setters & Getters
	void SetCreateGameTitleText(FText newCreateGameTitleText);
	FText GetCreateGameTitleText() const;
	void SetBackText(FText newBackText);
	FText GetBackText() const;
	void SetNewGameText(FText newNewGameText);
	FText GetNewGameText() const;
	void SetSessionNameText(FText SessionNameText);
	FText GetSessionNameText() const;
	void SetSessionNameEditableText(FText SessionNameEditableText);
	FText GetSessionNameEditableText() const;
	void SetIsLANText(FText newIsLANText);
	FText GetIsLANText() const;
	void SetIsLANValue(ECheckBoxState newIsLANValue);
	ECheckBoxState GetIsLANValue() const;
};