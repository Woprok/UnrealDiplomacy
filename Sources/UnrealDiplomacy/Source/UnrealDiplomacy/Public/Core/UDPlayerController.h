// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "ModularPlayerController.h"
#include "UDControllerInterface.h"
#include "UDPlayerController.generated.h"

// Forward Declarations

/**
 * Overrides default PlayerController to always use Game & UI input.
 * Provides basic implementation to synchronize IUDControllerInterface properties.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API AUDPlayerController : public AModularPlayerController, public IUDControllerInterface
{
	GENERATED_BODY()
public:
	AUDPlayerController();
	/**
	 * Called when the game starts or when spawned.
	 */
	virtual void BeginPlay() override;
public:
	// Replication
	UFUNCTION()
	void OnRep_SetUniqueControllerId(const int32& oldUniqueControllerId);
	UFUNCTION()
	void OnRep_SetControlledFactionId(const int32& oldControlledFactionId);
	/**
	 * Allows replication of properties.
	 * This is used for initial sync as we need to ensure that both
	 * client and server have controller prepared for action.
	 */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:
	UFUNCTION()
	virtual void SetControllerUniqueId(int32 uniqueControllerId) override;
	UFUNCTION(BlueprintPure)
	virtual int32 GetControllerUniqueId() override;
	UFUNCTION()
	virtual void SetControlledFactionId(int32 factionId) override;
	UFUNCTION(BlueprintPure)
	virtual int32 GetControlledFactionId() override;
protected:
	UPROPERTY(ReplicatedUsing = OnRep_SetUniqueControllerId)
	int32 UniqueControllerId;
	UPROPERTY(ReplicatedUsing = OnRep_SetControlledFactionId)
	int32 ControlledFactionId;
protected:
	/**
	 * Verifies that all IUDControllerInterface properties are synchronized.
	 */
	virtual bool IsPropertySynchronized();
	/**
	 * Allows inherited classes to start custom client synchronization based on IUDControllerInterface.
	 */
	virtual void StartClientSynchronization();
	/**
	 * Allows inherited classes to start custom faction change based on IUDControllerInterface.
	 */
	virtual void StartFactionChange();
};