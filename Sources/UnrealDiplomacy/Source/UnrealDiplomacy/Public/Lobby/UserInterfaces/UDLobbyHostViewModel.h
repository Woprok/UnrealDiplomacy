// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "UDLobbyHostViewModel.generated.h"

// Forward Declarations

enum ETextCommit::Type : int;

/**
 * ViewModel for defining host settings.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDLobbyHostViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	// Changes Commited
	UFUNCTION()
	void StartValueEditation(float InValue);
	UFUNCTION()
	void StopAICountValueEditation(float InValue, ETextCommit::Type CommitMethod);
	UFUNCTION()
	void StopMapSeedValueEditation(float InValue, ETextCommit::Type CommitMethod);
	UFUNCTION()
	void StopMapWidthValueEditation(float InValue, ETextCommit::Type CommitMethod);
	UFUNCTION()
	void StopMapHeightValueEditation(float InValue, ETextCommit::Type CommitMethod);
	UFUNCTION()
	void StopStratagemPointsValueEditation(float InValue, ETextCommit::Type CommitMethod);
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
	FText StratagemPointsText;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	float AICountValue;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	float MaxAICountValue;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	float MapSeedValue;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	float MapWidthValue;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	float MapHeightValue;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	float StratagemPointsValue;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool IsHostValue;
public:
	virtual void Setup() override;
	virtual void Refresh() override;
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
	void SetStratagemPointsText(FText newStratagemPointsText);
	FText GetStratagemPointsText() const;

	void SetAICountValue(float newAICountValue);
	float GetAICountValue() const;
	void SetMaxAICountValue(float newMaxAICountValue);
	float GetMaxAICountValue() const;
	void SetMapSeedValue(float newMapSeedValue);
	float GetMapSeedValue() const;
	void SetMapWidthValue(float newMapWidthValue);
	float GetMapWidthValue() const;
	void SetMapHeightValue(float newMapHeightValue);
	float GetMapHeightValue() const;
	void SetStratagemPointsValue(float newStratagemPointsValue);
	float GetStratagemPointsValue() const;

	void SetIsHostValue(bool newIsHostValue);
	bool GetIsHostValue() const;
private:
	// Fields
	bool IsValueEditing = false;
};