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
	UPROPERTY()
		bool bIsDashing=false;
	UPROPERTY()
		int32 CurrentDashPoint = 0;

	UPROPERTY()
		int32 MaxDashPoint = 200;
	void TimerCount();
	UPROPERTY()
		class APawn* PlayerPawn;
	UPROPERTY()
		class APlayerController* PlayerController;

	void StartRecording();
	void RecordingGhost();


public:
	AMainGameModeBase(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY()
		class UMainGameInstance* GameInstance;
	
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

	UFUNCTION(BlueprintPure,Category="Dash")
	bool IsDashing() const;
	void SetIsDashing(const bool bIsNewDashing);
	
	UFUNCTION(BlueprintPure,Category="Cat")
	int32 GetCurrentDashPoint() const;

	UFUNCTION(BlueprintPure, Category = "Cat")
	int32 GetMaxDashPoint()const;

	void SetCurrentDashPoint(const int32 Point);
	UFUNCTION(BlueprintSetter)
		void SetCurrentGameState(EGameState gstate);

	UFUNCTION()
		void AddDashPoint(int32 Point);

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

		void StopRecording();
protected:
	virtual void BeginPlay() override;
	virtual void StartPlay() override;

};
