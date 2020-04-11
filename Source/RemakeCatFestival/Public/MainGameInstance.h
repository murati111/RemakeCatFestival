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
private:
	UPROPERTY()
	bool bIsGhostMode = false;

	UPROPERTY()
	FCatGhost RecordingGhostData;

	UPROPERTY()
	FCatGhost LoadingGhostData;

	UPROPERTY()
	class UMainSaveGame* GameData;

	UPROPERTY()
	float CurrentTime = 0.0f;

	int32 CurrentRank = -1;	

	int32 MaxSaveGame = 3;

	FString SaveSlotName = TEXT("SaveGameSlot");

	void InitialSaveGame();

protected:
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = GameData)
	TSubclassOf<class UMainSaveGame> SaveGameSlot;

public:
	bool IsGhostMode() const { return bIsGhostMode; }

	int32 GetCurrentRank() const { return CurrentRank; }

	UFUNCTION(BlueprintPure,Category="GameTime")
	float GetCurrentTime() const { return CurrentTime; }

	void AddCurrentTime(const float Time) { CurrentTime += Time; }

	FCatGhost GetLoadingGhostData()const { return LoadingGhostData; }

	void SaveGameData();

	UFUNCTION(BlueprintCallable, Category = "GameInstance")
	void LoadGameDataFromIndex(int32 RankIndex);

	TArray<FCatGhost> LoadGameData();

	void CreateGhostTimesAndDoesGhostDataExists(TArray<float>& RankingTimes, TArray<bool>& DoesGhostDataExists);

	void CreateGhostTimes(TArray<float>& RankingTimes);

	void InitialInstanceValues();

	void RestartInstanceValues();

	static UMainGameInstance* GetInstance();

	void AddGhostData(const FVector Position, const float Speed, const bool IsStop);

	void SetRecordTime();

	virtual void Init() override;
};
