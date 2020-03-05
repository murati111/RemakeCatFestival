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

AMainGameModeBase::AMainGameModeBase(const FObjectInitializer& ObjectInitializer)
{
	/*static ConstructorHelpers::FClassFinder<APawn> PawnClass(TEXT("/Game/Cat/BP/BP_Cat"));
	static ConstructorHelpers::FClassFinder<APlayerController> ControllerClass(TEXT("/Game/Cat/BP/BP_CatController"));
	if (PawnClass.Succeeded())
	{
		DefaultPawnClass = PawnClass.Class;
	}
	if (ControllerClass.Succeeded())
	{
		PlayerControllerClass = ControllerClass.Class;
	}
	PlayerControllerClass = ControllerClass.Class;*/
	GameInstance = UMainGameInstance::GetInstance();
	
}


void AMainGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMainGameModeBase::StartPlay()
{
	Super::StartPlay();
	 UE_LOG(LogTemp, Log, TEXT("StartPlay"));
	PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (PlayerPawn != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("NotNull"));
	}
	Cat = Cast<ACat>(PlayerPawn);
	PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	RacePrepare();
}

void AMainGameModeBase::TimerCount()
{
	GameInstance->CurrentTime += 0.01f;
	//UE_LOG(LogTemp, Error, TEXT("Overlap %f"), GameInstance->Time);
}

void AMainGameModeBase::StartRecording()
{
	GameInstance->AddGhostData(Cat->GetActorLocation(), Cat->GetVelocity().Size(), Cat->bIsHitObscle);
	GetWorldTimerManager().SetTimer(RecordTimeHandle, this, &AMainGameModeBase::RecordingGhost,RecordingDeltaTime,true);
	
}

void AMainGameModeBase::RecordingGhost()
{
	GameInstance->AddGhostData(Cat->GetActorLocation(), Cat->GetVelocity().Size(), Cat->bIsHitObscle);
}

void AMainGameModeBase::StopRecording()
{
	GetWorldTimerManager().ClearTimer(RecordTimeHandle);
}



EGameState AMainGameModeBase::GetCurrentGameState()
{
	return CurrentGameState;
}

bool AMainGameModeBase::IsDashing() const
{
	return bIsDashing;
}

void AMainGameModeBase::SetIsDashing(const bool bIsNewDashing)
{
	bIsDashing = bIsNewDashing;
}

int32 AMainGameModeBase::GetCurrentDashPoint() const
{
	return CurrentDashPoint;
}
int32 AMainGameModeBase::GetMaxDashPoint() const
{
	return MaxDashPoint;
}
void AMainGameModeBase::SetCurrentDashPoint(const int32 Point)
{
	CurrentDashPoint = Point;
}

void AMainGameModeBase::SetCurrentGameState(EGameState gstate)
{
	CurrentGameState = gstate;
}

void AMainGameModeBase::AddDashPoint(int32 Point)
{
	if (IsDashing()) return;
	int32 AdditionalPoint = GetCurrentDashPoint() + Point;
	//ãŒÀ‚Æ‰ºŒÀ‚ÌÝ’è
	AdditionalPoint = FMath::Clamp(AdditionalPoint, 0, MaxDashPoint);
	SetCurrentDashPoint(AdditionalPoint);
	
}

void AMainGameModeBase::TimerStopAndRecord()
{
	GetWorldTimerManager().ClearTimer(GameTimeHandle);
	GameInstance->SetRecordTime();
	GameInstance->SaveGameData();
	UE_LOG(LogTemp, Log, TEXT("Start"));
}
void AMainGameModeBase::RacePrepare_Implementation()
{
	SetCurrentGameState(EGameState::Pausing);
	if (PlayerPawn != nullptr)
	{
		PlayerPawn->DisableInput(PlayerController);
	}
	
}

void AMainGameModeBase::RaceStart_Implementation()
{
	if (GameInstance->bIsGhostMode)
	{
		if (GhostClass != nullptr)
		{
			UWorld* const World = GetWorld();
			if (World != nullptr)
			{

				const FRotator SpawnRotation = Cat->GetActorRotation();
				const FVector SpawnLocation = Cat->GetActorLocation();

				Ghost = World->SpawnActor<AGhost>(GhostClass, SpawnLocation, SpawnRotation);
				Ghost->StartLoadingGhost();
			}
		}
		
	}
	SetCurrentGameState(EGameState::Playing);
	PlayerPawn->EnableInput(PlayerController);
	GetWorldTimerManager().SetTimer(GameTimeHandle, this, &AMainGameModeBase::TimerCount, 0.01f, true);
	StartRecording();
}

void AMainGameModeBase::RaceStop()
{
	StopRecording();
	if (Ghost != nullptr)
	{
		Ghost->StopLoadingGhost();
	}
	TimerStopAndRecord();
}

void AMainGameModeBase::RacePaused()
{
}

void AMainGameModeBase::RaceUnPaused()
{
}

