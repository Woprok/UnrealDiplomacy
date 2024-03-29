// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/Actions/UDGameAction.h"
#include "UDGameActionThroneLiberate.generated.h"

/**
 * Enables player to contest the throne from usurper.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDGameActionThroneLiberate : public UUDGameAction
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const override;
	virtual void Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual int32 GetId() const override { return ActionTypeId; };
	bool IsBackupRequired() const override { return true; };
	void Backup(FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual FUDActionPresentation GetPresentation() const override;
	virtual void SetResourceManager(TWeakObjectPtr<UUDResourceManager> resourceManager) override;
	virtual void SetModifierManager(TWeakObjectPtr<UUDModifierManager> modifierManager) override;
public:
	static const int32 ActionTypeId = 2003;
	/** 
	 * Contester has small defensive advantage. (taking capital by force should not be easy)
	 * Over 1.0 it's bonus, under 1.0 it's penalty...
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float UsurperManpowerDefensiveBonus = 0.75f;
	/** Usurper always loses less. (add to total perc for all) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resolution")
	float UsurperSideManpowerCasualties = 0.25f;
	/** Contester always loses more. (add to total perc for all) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resolution")
	float ContesterSideManpowerCasualties = 0.33f;
	/** Winners lose less in total. (add to total perc for all) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resolution")
	float WinnerCasualties = 0.1f;
	/** Losers lose more in total. (add to total perc for all) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resolution")
	float LoserCasualties = 0.2f;
	/** Leader loses additional troops. (add to total perc for leader only) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resolution")
	float LeaderCasualties = 0.1f;
	/** Usurper gains only flat reputation bonus. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resolution")
	int32 UsurperReputationWin = 25;
	/** Liberator gains flat bonus and perc of usurper reputation. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resolution")
	int32 LiberatorReputationWin = 25;
	/** Liberator gains flat bonus and perc of usurper reputation. Usurper is set to 0 or keeps negative. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resolution")
	float UsurperReputationTakePerc = 1.0f;
protected:
	UPROPERTY()
	TWeakObjectPtr<UUDModifierManager> ModifierManager = nullptr;
	UPROPERTY()
	TWeakObjectPtr<UUDResourceManager> ResourceManager = nullptr;
private:
	int32 GetTotalMilitarySupport(const TObjectPtr<UUDWorldState>& state, int32 factionId);
	void ApplySideCasualties(const TObjectPtr<UUDWorldState>& state, int32 factionId, float losesPercantage);
};