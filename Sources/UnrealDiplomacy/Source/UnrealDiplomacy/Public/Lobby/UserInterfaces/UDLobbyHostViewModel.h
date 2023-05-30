// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "UDLobbyHostViewModel.generated.h"

/**
 * ViewModel for defining host settings.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDLobbyHostViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText HostSettingsTitleText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText AICountText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText MapSeedText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText MapWidthText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText MapHeightText;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	float AICountValue;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	float MapSeedValue;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	float MapWidthValue;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	float MapHeightValue;
protected:
	virtual void Initialize() override;
	virtual void Update() override;
private:
	// MVVM Setters & Getters
	void SetHostSettingsTitleText(FText newHostSettingsTitleText);
	FText GetHostSettingsTitleText() const;
	void SetAICountText(FText newAICountText);
	FText GetAICountText() const;
	void SetMapSeedText(FText newMapSeedText);
	FText GetMapSeedText() const;
	void SetMapWidthText(FText newMapWidthText);
	FText GetMapWidthText() const;
	void SetMapHeightText(FText newMapHeightText);
	FText GetMapHeightText() const;

	void SetAICountValue(float newAICountValue);
	float GetAICountValue() const;
	void SetMapSeedValue(float newMapSeedValue);
	float GetMapSeedValue() const;
	void SetMapWidthValue(float newMapWidthValue);
	float GetMapWidthValue() const;
	void SetMapHeightValue(float newMapHeightValue);
	float GetMapHeightValue() const;
};