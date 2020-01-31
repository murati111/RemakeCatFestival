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
		TArray<FVector> Position;
		
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Cat")
		TArray<FRotator> Rotation;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Cat")
		TArray<float> Speed;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Cat")
		TArray<bool> IsStop;

	FCatGhost()
	{
		RecordTime = 0.0f;
	}
	FCatGhost(float InTime,TArray<FVector> InPosition, TArray<FRotator> InRotation, TArray<float> InSpeed, TArray<bool> InIsStop)
	{
		RecordTime = InTime;
		Position = InPosition;
		Rotation = InRotation;
		Speed = InSpeed;
		IsStop = InIsStop;
	}
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

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = GameData)
		FCatGhost GhostData;

	static UMainGameInstance* GetInstance();
};
