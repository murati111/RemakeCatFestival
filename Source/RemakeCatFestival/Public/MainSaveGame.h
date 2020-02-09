// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Public/MainGameInstance.h"
#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MainSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class REMAKECATFESTIVAL_API UMainSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	/*UPROPERTY(VisibleAnyWhere, Category = SaveGame)
		FString SaveSlotName;
	UPROPERTY(VisibleAnyWhere, Category = SaveGame)
		uint32 UserIndex;*/

	UPROPERTY(EditAnyWhere,Category = SaveGame)
		TArray<FCatGhost> GhostRecords;

	UPROPERTY(EditAnyWhere, Category = SaveGame)
		bool IsFirstLaunched;

	int32 MAXSAVESLOT;
	UMainSaveGame();
};
