// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameModeBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Public/CatController.h"
#include "Public/Cat.h"
#include "TimerManager.h"
//#include "Public/MainGameInstance.h"
#include "Public/MainSaveGame.h"
#include "Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Public/Ghost.h"

AMainGameModeBase::AMainGameModeBase(const FObjectInitializer& ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<APawn> PawnClass(TEXT("/Game/Cat/BP/BP_Cat"));
	static ConstructorHelpers::FClassFinder<APlayerController> ControllerClass(TEXT("/Game/Cat/BP/BP_CatController"));
	if (PawnClass.Succeeded())
	{
		DefaultPawnClass = PawnClass.Class;
	}
	if (ControllerClass.Succeeded())
	{
		PlayerControllerClass = ControllerClass.Class;
	}
	HUDClass = AHUD::StaticClass();
	PlayerControllerClass = APlayerController::StaticClass();
	PlayerStateClass = APlayerState::StaticClass();
	GameStateClass = AGameStateBase::StaticClass();
	gameInstance = UMainGameInstance::GetInstance();
	
}


void AMainGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Log, TEXT("Begin"));
	
}

void AMainGameModeBase::StartPlay()
{
	Super::StartPlay();
	 UE_LOG(LogTemp, Log, TEXT("Start"));
	 //cat = Cast<ACat>);
	PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	Cat = Cast<ACat>(PlayerPawn);
	PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	RacePrepare();
}

void AMainGameModeBase::TimerCount()
{
	gameInstance->CurrentTime += 0.01f;
	//UE_LOG(LogTemp, Error, TEXT("Overlap %f"), gameInstance->Time);
}

void AMainGameModeBase::StartRecording()
{
	gameInstance->AddGhostData(Cat->GetActorLocation(), Cat->GetVelocity().Size(), Cat->bIsHitObscle);
	GetWorldTimerManager().SetTimer(RecordTimeHandle, this, &AMainGameModeBase::RecordingGhost,RecordingDeltaTime,true);
	
}

void AMainGameModeBase::RecordingGhost()
{
	gameInstance->AddGhostData(Cat->GetActorLocation(), Cat->GetVelocity().Size(), Cat->bIsHitObscle);
}

void AMainGameModeBase::StopRecording()
{
	GetWorldTimerManager().ClearTimer(RecordTimeHandle);
}



EGameState AMainGameModeBase::GetCurrentGameState()
{
	return currentGameState;
}

void AMainGameModeBase::SetCurrentGameState(EGameState gstate)
{
	currentGameState = gstate;
}

void AMainGameModeBase::AddDashPoint(int32 dp)
{
	gameInstance->DashPoint += dp;
}

void AMainGameModeBase::TimerStopAndRecord()
{
	GetWorldTimerManager().ClearTimer(GameTimeHandle);
	gameInstance->SetRecordTime();
	SaveGhostRecord(gameInstance->CurrentTime);
	UE_LOG(LogTemp, Log, TEXT("Start"));
}
void AMainGameModeBase::RacePrepare_Implementation()
{
	SetCurrentGameState(EGameState::Pausing);
	PlayerPawn->DisableInput(PlayerController);
}

void AMainGameModeBase::RaceStart()
{
	if (bIsGhostMode)
	{
			FString path = "/Game/Cat/BP/BP_GhostCat.BP_GhostCat_C"; 
			TSubclassOf<class AGhost> sc = TSoftClassPtr<AGhost>(FSoftObjectPath(*path)).LoadSynchronous(); 
			if (sc != nullptr)
			{
				Ghost = GetWorld()->SpawnActor<AGhost>(sc); // ƒXƒ|[ƒ“ˆ—
				Ghost->StartLoadingGhost();
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
	Ghost->StopLoadingGhost();
	TimerStopAndRecord();
}

void AMainGameModeBase::RacePaused()
{
}

void AMainGameModeBase::RaceUnPaused()
{
}

void AMainGameModeBase::SaveGhostRecord(float time)
{
	UMainSaveGame* SaveGameInst = Cast<UMainSaveGame>(UGameplayStatics::CreateSaveGameObject(UMainSaveGame::StaticClass()));
	//GhostRecordSort(SaveGameInst->GhostRecords, time);
	UGameplayStatics::SaveGameToSlot(SaveGameInst, SaveGameInst->SaveSlotName, SaveGameInst->UserIndex);
}

FCatGhost AMainGameModeBase::LoadGhostRecord(int32 index)
{
	UMainSaveGame* LoadGameInst = Cast<UMainSaveGame>(UGameplayStatics::CreateSaveGameObject(UMainSaveGame::StaticClass()));
	LoadGameInst = Cast<UMainSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInst->SaveSlotName, LoadGameInst->UserIndex));
	return LoadGameInst->GhostRecords[index];
}

void AMainGameModeBase::GhostRecordSort(TArray<FCatGhost>& ghostdatas,float time)
{
	for (int32 i = 0; i < 3; i++)
	{
		if (LoadGhostRecord(i).RecordTime <= time)
		{
			ghostdatas.Insert(gameInstance->RecordingGhostData, i);
			ghostdatas.RemoveAt(3);
			break;
		}
	}
}
