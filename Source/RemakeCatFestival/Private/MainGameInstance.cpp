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
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName,UserIndex))
	{
		if (GameData == nullptr)
		{
			GameData = Cast<UMainSaveGame>(UGameplayStatics::CreateSaveGameObject(SaveGameSlot));
		}
		for (int8 i=0; i<MAXSAVEGAME; i++)
		{
			GameData->GhostRecords.AddZeroed();
			GameData->GhostRecords[i].RecordTime = -1.0f;
		}
		UGameplayStatics::SaveGameToSlot(GameData, SaveSlotName, UserIndex);
	}
}
void UMainGameInstance::Init()
{
	InitialSaveGame();
	for (int8 i = 0; i < MAXSAVEGAME; i++)
	{
		UE_LOG(LogTemp, Log, TEXT("%dˆÊF%f"), i, LoadGameData(i).RecordTime);
	}


}
void UMainGameInstance::SaveGameData()
{
	for (int8 i = 0; i < MAXSAVEGAME; i++)
	{
		if (GameData->GhostRecords[i].RecordTime <= 0)
		{
			GameData->GhostRecords.Insert(RecordingGhostData, i);
			GameData->GhostRecords.RemoveAt(MAXSAVEGAME);
			break;
		}
		else if (CurrentTime <= GameData->GhostRecords[i].RecordTime)
		{
			GameData->GhostRecords.Insert(RecordingGhostData, i);
			GameData->GhostRecords.RemoveAt(MAXSAVEGAME);
			break;
		}
	}
	UGameplayStatics::SaveGameToSlot(GameData, SaveSlotName, UserIndex);

}

FCatGhost UMainGameInstance::LoadGameData(int32 RankIndex)
{
	if (GameData == nullptr)
	{
		TSubclassOf<UMainSaveGame> SaveGameSlot;
		GameData = Cast<UMainSaveGame>(UGameplayStatics::CreateSaveGameObject(SaveGameSlot));
	}
	GameData = Cast<UMainSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, UserIndex));

	return GameData->GhostRecords[RankIndex];
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
