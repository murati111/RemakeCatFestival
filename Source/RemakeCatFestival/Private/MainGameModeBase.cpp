// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameModeBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Public/CatController.h"
#include "Public/Cat.h"
#include "TimerManager.h"
#include "Public/MainSaveGame.h"
#include "Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Public/Ghost.h"
#include "Blueprint/UserWidget.h"



AMainGameModeBase::AMainGameModeBase(const FObjectInitializer& ObjectInitializer)
{
	GameInstance = UMainGameInstance::GetInstance();
	
}


void AMainGameModeBase::BeginPlay()
{
	AddToViewportWidget();
	Cat = Cast<ACat>(GetPlayerController()->GetPawn());
	if (Cat != nullptr)
	{
		Cat->RaceStopDelegate.BindUObject(this, &AMainGameModeBase::RaceStop);
		Cat->RecordGhostDelegate.BindUObject(this, &AMainGameModeBase::StopRecording);
	}
	RacePrepare();
	Super::BeginPlay();
	
}
void AMainGameModeBase::AddToViewportWidget()
{
	if (GetPlayerController() == nullptr) { return; }
	CatWidget = CreateWidget<UUserWidget>(GetPlayerController(), CatWidgetClass);
	if (CatWidget != nullptr)
	{
		CatWidget->AddToViewport();
	}
}

APlayerController* AMainGameModeBase::GetPlayerController() const
{
	return GetWorld()->GetFirstPlayerController() ? GetWorld()->GetFirstPlayerController() : nullptr;
}

void AMainGameModeBase::TimerStop()
{
	if (GameInstance == nullptr) { return; }
	GetWorldTimerManager().ClearTimer(GameTimeHandle);
	GameInstance->SetRecordTime();
}


void AMainGameModeBase::TimerCount()
{
	if (GameInstance == nullptr) { return; }

	constexpr float MaxTimeSec = 600.f;
	if (GameInstance->GetCurrentTime() >= MaxTimeSec)
	{
		RaceStopForTimeUp();
		StopRecording();
		return;
	}
	constexpr float AdditionalTime = 0.01f;
	GameInstance->AddCurrentTime(AdditionalTime);
	UpdateTimeCount(GameInstance->GetCurrentTime());
	

	//UE_LOG(LogTemp, Error, TEXT("Overlap %f"), GameInstance->Time);
}


void AMainGameModeBase::StartRecording()
{
	if (Cat == nullptr) { return; }
	GameInstance->AddGhostData(Cat->GetActorLocation(), Cat->GetVelocity().Size(), Cat->IsHitObscle());
	GetWorldTimerManager().SetTimer(RecordTimeHandle, this, &AMainGameModeBase::RecordingGhost,RecordingDeltaTime,true);
	
}

void AMainGameModeBase::RecordingGhost()
{
	if (Cat == nullptr) { return; }
	GameInstance->AddGhostData(Cat->GetActorLocation(), Cat->GetVelocity().Size(), Cat->IsHitObscle());
}

void AMainGameModeBase::StopRecording()
{
	GetWorldTimerManager().ClearTimer(RecordTimeHandle);
	if (Ghost != nullptr)
	{
		Ghost->StopLoadingGhost();
	}
	GameInstance->SaveGameData();
	OnSavedEvent();
}
void AMainGameModeBase::RacePrepare_Implementation()
{

	if (GetPlayerController() != nullptr && Cat != nullptr)
	{
		Cat->DisableInput(GetPlayerController());
	}
}

void AMainGameModeBase::RaceStart()
{
	if (GameInstance->IsGhostMode()) 
	{
		if (GhostClass != nullptr && GetWorld() != nullptr)
		{
			const FRotator SpawnRotation = Cat->GetActorRotation();
			const FVector SpawnLocation = Cat->GetActorLocation();

			Ghost = GetWorld()->SpawnActor<AGhost>(GhostClass, SpawnLocation, SpawnRotation);
			if (Ghost != nullptr) 
			{ 
				Ghost->StartLoadingGhost();
			}
		}
	}
	if (GetPlayerController() != nullptr && Cat != nullptr)
	{
		Cat->EnableInput(GetPlayerController());
	}
	GetWorldTimerManager().SetTimer(GameTimeHandle, this, &AMainGameModeBase::TimerCount, 0.01f, true);
	StartRecording();
}

void AMainGameModeBase::RaceStop()
{
	TimerStop();
	GoalEvent(false);
	UE_LOG(LogTemp, Error, TEXT("Race Stop!!!"));
}

void AMainGameModeBase::RaceStopForTimeUp()
{
	TimerStop();
	GoalEvent(true);
	UE_LOG(LogTemp, Error, TEXT("TimeUP!!!"));
}

void AMainGameModeBase::RacePaused()
{
	GetPlayerController()->SetPause(true);
}

void AMainGameModeBase::RaceUnPaused()
{
	GetPlayerController()->SetPause(false);
}

void AMainGameModeBase::RestartRace()
{
	GameInstance->RestartInstanceValues();
}

