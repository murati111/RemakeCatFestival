// Fill out your copyright notice in the Description page of Project Settings.

#include "MainSaveGame.h"
#include "RemakeCatFestival.h"
#include "Kismet/GameplayStatics.h"

UMainSaveGame::UMainSaveGame()
{
	//SaveSlotName = TEXT("SaveSlot");
	//UserIndex = 0;
	//MAXSAVESLOT = 3;
	//for (int32 i = 0; i < MAXSAVESLOT; i++)
	//{
	//	GhostRecords.AddZeroed();
	//	GhostRecords[i].RecordTime = -1.0f;
	//}
	IsFirstLaunched = false;
}
