// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CatController.generated.h"

/**
 * 
 */
UCLASS()
class REMAKECATFESTIVAL_API ACatController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY()
	class AMainGameModeBase* gameMode;

protected:
	virtual void BeginPlay() override;
};
