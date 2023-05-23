// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "UDServerItemViewModel.generated.h"

// Forward Declarations

class FOnlineSessionSearchResult;

/**
 * ViewModel for single server shown in join game.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDServerItemViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	/**
	 * Set content of the session.
	 */
	void SetContent(const FOnlineSessionSearchResult& content);
	/**
	 * Set content of the session.
	 */
	const FOnlineSessionSearchResult& GetContent() const;
public:
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText NameText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	int32 PingValue;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool IsEnabled;
protected:
	virtual void Initialize() override;
	virtual void Update() override;
private:
	// MVVM Setters & Getters
	void SetNameText(FText newNameText);
	FText GetNameText() const;
	void SetPingValue(int32 newPingValue);
	int32 GetPingValue() const;
	void SetIsEnabled(bool newIsEnabled);
	bool GetIsEnabled() const;
private:
	// Fields
	const FOnlineSessionSearchResult* Content;
};