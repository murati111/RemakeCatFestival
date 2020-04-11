// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Public/MainGameInstance.h"
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameModeBase.generated.h"

/**
 * 
 */

//UENUM(BlueprintType)
//enum class EGameState : uint8
//{
//	Title UMETA(DisplayName = "Title"),
//	Playing UMETA(DisplayName = "Playing"),
//	Pausing UMETA(DisplayName = "Pausing"),
//	Ranking UMETA(DisplayName = "Ranking"),
//};



UCLASS()
class REMAKECATFESTIVAL_API AMainGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

//UI�֘A
private:
	UPROPERTY(EditAnywhere,Category="Widget")
	TSubclassOf<class UUserWidget> CatWidgetClass;

	class UUserWidget* CatWidget;

	void AddToViewportWidget();
protected:
	UFUNCTION(BlueprintPure, Category = "Widget")
	class UUserWidget* GetCatWidget()const { return CatWidget; }

//�S�[�X�g���R�[�h
private:
	UPROPERTY()
	float RecordingDeltaTime = 0.001;

	FTimerHandle RecordTimeHandle;

	UPROPERTY()
	class AGhost* Ghost;

	UPROPERTY(EditDefaultsOnly, Category=Ghost)
	class TSubclassOf<class AGhost> GhostClass;

	void StartRecording();

	void RecordingGhost();
public:
	float GetRecordingDeltaTime()const { return RecordingDeltaTime; }

//�L�����N�^�[�E�R���g���[���[�E�C���X�^���X
private:
	UPROPERTY()
	class ACat* Cat;

	APlayerController* GetPlayerController()const;
protected:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Level")
	FName ResultMapName = TEXT("Result");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	FName TitleMapName = TEXT("Title");

public:
	AMainGameModeBase(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY()
	class UMainGameInstance* GameInstance;

//�^�C���v��
private:
	FTimerHandle GameTimeHandle;
	void TimerCount();

//�Q�[���i�s�Ǘ�
private:
	void StopRecording();
	void TimerStop();
	void RaceStop();
	void RaceStopForTimeUp();
protected:
	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void RaceStart();

	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void RacePaused();

	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void RaceUnPaused();


	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void RestartRace();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GameMode")
	void RacePrepare();

	UFUNCTION(BlueprintImplementableEvent,Category="GameMode")
	void GoalEvent(bool bIsTimeUp);

	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void OnSavedEvent();

	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void UpdateTimeCount(float CurrentTime);
};
