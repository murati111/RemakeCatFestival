// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class REMAKECATFESTIVAL_API AMainGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
private:
	FTimerHandle GameTimeHandle;
	void TimerCount();

public:
	AMainGameModeBase(const FObjectInitializer& ObjectInitializer);
	class UMainGameInstance* gameInstance;

	UFUNCTION()
		void AddDashPoint(int32 dp);

	UFUNCTION()
		void TimerStopAndRecord();

protected:
	virtual void BeginPlay() override;

};
