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
	UE_LOG(LogTemp, Error, TEXT("%s"), (Cat->bIsHitObscle ? TEXT("True") : TEXT("false")));
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

void AMainGameModeBase::SetCurrentGameState(EGameState gstate)
{
	CurrentGameState = gstate;
}

void AMainGameModeBase::AddDashPoint(int32 dp)
{
	GameInstance->DashPoint += dp;
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

//void AMainGameModeBase::SaveGhostRecord(float time)
//{
//	GameData = Cast<UMainSaveGame>(UGameplayStatics::CreateSaveGameObject(UMainSaveGame::StaticClass()));
//	if (LoadSaveGame())
//	{
//		UE_LOG(LogTemp, Log, TEXT("WhyNull"));
//		for (int8 i = 0; i < 3; i++)
//		{
//			GameData->GhostRecords.AddZeroed();
//			GameData->GhostRecords[i].RecordTime = -1.0f;
//		}
//	}
//
//	//GhostRecordSort(SaveGameInst->GhostRecords, time);
//	for (int8 i = 0; i < 3; i++)
//	{
//		float SaveRecordTime = LoadGhostRecord(i).RecordTime;
//		if (SaveRecordTime <= 0)
//		{
//			GameData->GhostRecords.Insert(GameInstance->RecordingGhostData, i);
//			GameData->GhostRecords.RemoveAt(3);
//			break;
//		}
//		else if((SaveRecordTime >= GameInstance->CurrentTime))
//		{			
//			GameData->GhostRecords.Insert(GameInstance->RecordingGhostData, i);
//			GameData->GhostRecords.RemoveAt(3);
//			break;
//		}
//	}
//	UGameplayStatics::SaveGameToSlot(GameData, GameData->SaveSlotName, GameData->UserIndex);
//}

//FCatGhost AMainGameModeBase::LoadGhostRecord(int32 index)
//{
//	UMainSaveGame* LoadGameInst = Cast<UMainSaveGame>(UGameplayStatics::CreateSaveGameObject(UMainSaveGame::StaticClass()));
//	LoadGameInst = Cast<UMainSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInst->SaveSlotName, LoadGameInst->UserIndex));
//	return LoadGameInst->GhostRecords[index];
//
//}

/*bool AMainGameModeBase::LoadSaveGame()
{
	UMainSaveGame* LoadGameInst = Cast<UMainSaveGame>(UGameplayStatics::CreateSaveGameObject(UMainSaveGame::StaticClass()));
	LoadGameInst = Cast<UMainSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInst->SaveSlotName, LoadGameInst->UserIndex));
	return	LoadGameInst == nullptr;
}*/
