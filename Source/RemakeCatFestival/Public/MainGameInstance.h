// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MainGameInstance.generated.h"

/**
 * 
 */

USTRUCT(Blueprinttype)
struct FCatGhost {
	GENERATED_BODY()

	UPROPERTY()
		TArray<FVector> Position;
		
	UPROPERTY()
		TArray<FRotator> Rotation;

	UPROPERTY()
		TArray<float> Speed;

	UPROPERTY()
		TArray<bool> IsStop;
};

UCLASS()
class REMAKECATFESTIVAL_API UMainGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = GameData)
		float Time = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = GameData)
		int32 DashPoint = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = GameData)
		FCatGhost GhostData;

	static UMainGameInstance* GetInstance();
};
