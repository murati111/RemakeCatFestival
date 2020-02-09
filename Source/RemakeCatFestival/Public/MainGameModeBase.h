// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Public/MainGameInstance.h"
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameModeBase.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EGameState : uint8
{
	Title UMETA(DisplayName = "Title"),
	Playing UMETA(DisplayName = "Playing"),
	Pausing UMETA(DisplayName = "Pausing"),
	Ranking UMETA(DisplayName = "Ranking"),
};

UCLASS()
class REMAKECATFESTIVAL_API AMainGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
private:
	FTimerHandle GameTimeHandle;
	FTimerHandle RecordTimeHandle;
	void TimerCount();
	class APawn* PlayerPawn;
	class APlayerController* PlayerController;
	class ACat* cat;
	void StartRecording();
	void RecordingGhost();


public:
	AMainGameModeBase(const FObjectInitializer& ObjectInitializer);
	class UMainGameInstance* GameInstance;
	void StopRecording();
	UPROPERTY(EditDefaultsOnly, BlueprintSetter = SetCurrentGameState,BlueprintGetter = GetCurrentGameState, Category = "GameMode")
		EGameState CurrentGameState;

	UPROPERTY()
		float RecordingDeltaTime = 0.001;

	UPROPERTY()
		class UMainSaveGame* GameData;

	UPROPERTY()
		class AGhost* Ghost;
	UPROPERTY(EditDefaultsOnly , Category=Ghost)
		class TSubclassOf<class AGhost> GhostClass;
	UPROPERTY()
		class ACat* Cat;

	UFUNCTION(BlueprintGetter)
		EGameState GetCurrentGameState();
	UFUNCTION(BlueprintSetter)
		void SetCurrentGameState(EGameState gstate);

	UFUNCTION()
		void AddDashPoint(int32 dp);

	UFUNCTION()
		void TimerStopAndRecord();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GameMode")
		void RaceStart();

	UFUNCTION(BlueprintCallable, Category = "GameMode")
		void RaceStop();

	UFUNCTION(BlueprintCallable, Category = "GameMode")
		void RacePaused();

	UFUNCTION(BlueprintCallable, Category = "GameMode")
		void RaceUnPaused();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GameMode")
		void RacePrepare();

	//UFUNCTION(BlueprintCallable, Category = "GameMode")
	//	void SaveGhostRecord(float time);

	//UFUNCTION(BlueprintCallable, Category = "GameMode")
	//	FCatGhost LoadGhostRecord(int32 index);

///	UFUNCTION(BlueprintCallable, Category = "GameMode")
	//	bool LoadSaveGame();


protected:
	virtual void BeginPlay() override;
	virtual void StartPlay() override;

};
