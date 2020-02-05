// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameInstance.h"
#include "Engine.h"


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