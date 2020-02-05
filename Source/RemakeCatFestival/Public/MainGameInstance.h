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
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Cat")
		float RecordTime;

	UPROPERTY(EditAnyWhere,BlueprintReadWrite,Category="Cat")
		TArray<FVector> Positions;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Cat")
		TArray<float> Speeds;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Cat")
		TArray<bool> IsStops;
};

UCLASS()
class REMAKECATFESTIVAL_API UMainGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = GameData)
		float CurrentTime = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = GameData)
		int32 DashPoint = 0;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = GameData)
		FCatGhost LoadingGhostData;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = GameData)
		FCatGhost RecordingGhostData;

	static UMainGameInstance* GetInstance();
	void AddGhostData(const FVector Position, const float Speed, const bool IsStop);
	void SetRecordTime();
};
