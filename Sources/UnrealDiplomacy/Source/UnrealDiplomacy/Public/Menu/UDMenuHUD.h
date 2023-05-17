// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UDHUD.h"
#include "UDMenuHUD.generated.h"

/**
 * 
 */
UCLASS()
class UNREALDIPLOMACY_API AUDMenuHUD : public AUDHUD
{
	GENERATED_BODY()
public:
	/**
	 * Shortcut to retrieve casted MenuHUD.
	 * Requries World pointer that can be retrieved by GetWorld()
	 */
	static TObjectPtr<AUDMenuHUD> Get(TObjectPtr<UWorld> world);
};