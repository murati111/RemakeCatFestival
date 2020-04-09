// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameInstance.h"
#include "Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Public/MainSaveGame.h"

UMainGameInstance* UMainGameInstance::GetInstance()
{
	UMainGameInstance* instance = nullptr;
	if (GEngine)
	{
		FWorldContext* context = GEngine->GetWorldContextFromGameViewport(GEngine->GameViewport);
		instance = Cast<UMainGameInstance>(context->OwningGameInstance);
	}
	return instance;
}
void UMainGameInstance::InitialSaveGame()
{
	if (!UGameplayStatics::DoesSaveGameExist(SaveSlotName,0))
	{
		if (GameData == nullptr)
		{
			GameData = Cast<UMainSaveGame>(UGameplayStatics::CreateSaveGameObject(SaveGameSlot));
		}
		for (int8 i=0; i< MaxSaveGame; i++)
		{
			GameData->GhostRecords.AddZeroed();
			GameData->GhostRecords[i].RecordTime = -1.0f;
		}
		UGameplayStatics::SaveGameToSlot(GameData, SaveSlotName, 0);
	}
}
void UMainGameInstance::Init()
{
	InitialSaveGame();

}
void UMainGameInstance::SaveGameData()
{
	if (GameData == nullptr) 
	{ 
		GameData = Cast<UMainSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
		if (GameData == nullptr) { return; } //ÉçÅ[ÉhÇ™é∏îsÇµÇƒÇ¢ÇΩèÍçá
	}
	for (int8 i = 0; i < MaxSaveGame; i++)
	{
		if (GameData->GhostRecords[i].RecordTime <= 0)
		{
			GameData->GhostRecords.Insert(RecordingGhostData, i);
			GameData->GhostRecords.RemoveAt(MaxSaveGame);
			CurrentRank = i;
			break;
		}
		else if (CurrentTime <= GameData->GhostRecords[i].RecordTime)
		{
			GameData->GhostRecords.Insert(RecordingGhostData, i);
			GameData->GhostRecords.RemoveAt(MaxSaveGame);
			CurrentRank = i;
			break;
		}
	}
	UGameplayStatics::SaveGameToSlot(GameData, SaveSlotName, 0);

}

void UMainGameInstance::LoadGameDataFromIndex(int32 RankIndex)
{
	if(RankIndex>=3){ return; }
	if (GameData == nullptr)
	{
		GameData = Cast<UMainSaveGame>(UGameplayStatics::CreateSaveGameObject(SaveGameSlot));
	}
	GameData = Cast<UMainSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	if (GameData == nullptr) { return; }
	LoadingGhostData = GameData->GhostRecords[RankIndex];
	bIsGhostMode = true;
}

TArray<FCatGhost> UMainGameInstance::LoadGameData()
{
	if (GameData == nullptr)
	{
		GameData = Cast<UMainSaveGame>(UGameplayStatics::CreateSaveGameObject(SaveGameSlot));
	}
	GameData = Cast<UMainSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName,0));
	if (GameData == nullptr) { return TArray<FCatGhost>(); }
	return GameData->GhostRecords;
}

void UMainGameInstance::CreateGhostTimesAndDoesGhostDataExists(TArray<float>& RankingTimes, TArray<bool>& DoesGhostDataExists)
{
	const TArray<FCatGhost> GhostDataes = LoadGameData();
	for (int32 Index = 0; Index < MaxSaveGame; Index++)
	{
		RankingTimes.Add(GhostDataes[Index].RecordTime);
		UE_LOG(LogTemp, Error, TEXT("RANK TIME : % f"), GhostDataes[Index].RecordTime);
		DoesGhostDataExists.Add(GhostDataes[Index].RecordTime > 0.0f);
	}
}

void UMainGameInstance::CreateGhostTimes(TArray<float>& RankingTimes)
{
	const TArray<FCatGhost> GhostDataes = LoadGameData();
	for (int32 Index = 0; Index < MaxSaveGame; Index++)
	{
		RankingTimes.Add(GhostDataes[Index].RecordTime);
	}
}

void UMainGameInstance::InitialInstanceValues()
{
	CurrentTime = 0.0f;
	bIsGhostMode = false;
	LoadingGhostData = FCatGhost();
	RecordingGhostData = FCatGhost();
	CurrentRank = -1;
}

void UMainGameInstance::RestartInstanceValues()
{
	CurrentTime = 0.0f;
	RecordingGhostData = FCatGhost();
	CurrentRank = -1;
}

void UMainGameInstance::AddGhostData(const FVector Position, const float Speed, const bool IsStop)
{
	RecordingGhostData.Positions.Add(Position);
	RecordingGhostData.Speeds.Add(Speed);
	RecordingGhostData.IsStops.Add(IsStop);
}
void UMainGameInstance::SetRecordTime()
{
	RecordingGhostData.RecordTime = CurrentTime;
}
