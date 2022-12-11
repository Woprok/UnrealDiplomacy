// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UDActor.generated.h"

/**
 *
 */
UCLASS()
class UNREALDIPLOMACY_API AUDActor : public AActor
{
	GENERATED_BODY()
public:
	/**
	 * Sets default values for this actor's properties.
	 */
	AUDActor();
	/**
	 * Called every frame.
	 */
	virtual void Tick(float DeltaTime) override;
protected:
	/**
	 * Called when the game starts or when spawned
	 */
	virtual void BeginPlay() override;
};