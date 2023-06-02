// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "ModularPlayerController.h"
#include "UDControllerInterface.h"
#include "UDPlayerController.generated.h"

// Forward Declarations

enum class EUDControllerType : uint8;

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
	UFUNCTION()
	void OnRep_SetType(const EUDControllerType& oldControllerType);
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
	UFUNCTION()
	virtual void SetControllerType(EUDControllerType type) override;
	UFUNCTION(BlueprintPure)
	virtual EUDControllerType GetControllerType() override;
protected:
	UPROPERTY(ReplicatedUsing = OnRep_SetUniqueControllerId)
	int32 UniqueControllerId;
	UPROPERTY(ReplicatedUsing = OnRep_SetControlledFactionId)
	int32 ControlledFactionId;
	// This is at the moment used only for server to determine how to approach this controller.
	UPROPERTY(ReplicatedUsing = OnRep_SetType)
	EUDControllerType Type;
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
	/**
	 * Allows inherited classes to start custom operation on type change based on IUDControllerInterface.
	 */
	virtual void OnTypeChanged();
};