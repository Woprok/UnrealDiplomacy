// Copyright Miroslav Valach
// TODO verify that pure server will never execute OnRep functions.
// TODO make use of Type on a client.

#include "Core/UDPlayerController.h"
#include "Core/UDGlobalData.h"
#include "Net/UnrealNetwork.h"

AUDPlayerController::AUDPlayerController()
{
	bEnableClickEvents = true;
	UniqueControllerId = UUDGlobalData::InvalidControllerId;
	ControlledFactionId = UUDGlobalData::InvalidFactionId;
}

void AUDPlayerController::BeginPlay()
{
	Super::BeginPlay();
	// Overrides default input mode to be Game & UI allowing player to use cursor & navigate the world.
	FInputModeGameAndUI inputMode = FInputModeGameAndUI();
	inputMode.SetHideCursorDuringCapture(false);
	inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	AUDPlayerController::SetInputMode(inputMode);
	SetShowMouseCursor(true);
}

void AUDPlayerController::OnRep_SetUniqueControllerId(const int32& oldUniqueControllerId)
{
	if (IsLocalPlayerController() && IsPropertySynchronized())
	{
		UE_LOG(LogTemp, Log, TEXT("AUDPlayerController(%d): Synchronized Id from original %d. [CLIENT]"), 
			GetControllerUniqueId(), oldUniqueControllerId);
		// This will never fire on client that runs server...
		StartClientSynchronization();
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("AUDPlayerController(%d): [SERVER] attempted sync on client actor."), 
			GetControllerUniqueId());
	}
}

void AUDPlayerController::OnRep_SetControlledFactionId(const int32& oldControlledFactionId)
{
	if (IsLocalPlayerController() && IsPropertySynchronized())
	{
		UE_LOG(LogTemp, Log, TEXT("AUDPlayerController(%d): Changed faction id from original %d. [CLIENT]"), 
			GetControlledFactionId(), oldControlledFactionId);
		// This will never fire on client that runs server...
		StartFactionChange();
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("AUDPlayerController(%d): [SERVER] attempted sync faction id on client actor."), 
			GetControlledFactionId());
	}
}

void AUDPlayerController::OnRep_SetType(const EUDControllerType& oldControllerType)
{
	UE_LOG(LogTemp, Log, TEXT("AUDPlayerController(%d): Server changed type of this controller."),
		GetControllerUniqueId());
	OnTypeChanged();
}

void AUDPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AUDPlayerController, UniqueControllerId);
	DOREPLIFETIME(AUDPlayerController, ControlledFactionId);
	DOREPLIFETIME(AUDPlayerController, Type);
}

void AUDPlayerController::SetControllerUniqueId(int32 uniqueControllerId)
{
	UniqueControllerId = uniqueControllerId;
}

int32 AUDPlayerController::GetControllerUniqueId()
{
	return UniqueControllerId;
}

void AUDPlayerController::SetControlledFactionId(int32 factionId)
{
	ControlledFactionId = factionId;
}

int32 AUDPlayerController::GetControlledFactionId()
{
	return ControlledFactionId;
}

void AUDPlayerController::SetControllerType(EUDControllerType type)
{
	Type = type;
}

EUDControllerType AUDPlayerController::GetControllerType()
{
	return Type;
}

bool AUDPlayerController::IsPropertySynchronized()
{
	bool hasUniqueId = UniqueControllerId != UUDGlobalData::InvalidControllerId;
	bool hasFactionId = ControlledFactionId != UUDGlobalData::InvalidFactionId;
	return hasUniqueId && hasFactionId;
}

void AUDPlayerController::StartClientSynchronization()
{
	return;
}

void AUDPlayerController::StartFactionChange()
{
	return;
}

void AUDPlayerController::OnTypeChanged()
{
	return;
}