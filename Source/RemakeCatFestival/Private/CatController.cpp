// Fill out your copyright notice in the Description page of Project Settings.


#include "CatController.h"
#include "Public/MainGameModeBase.h"
#include "Engine/World.h"

void ACatController::BeginPlay()
{
	Super::BeginPlay();
	gameMode = Cast<AMainGameModeBase>(GetWorld()->GetAuthGameMode());
	UE_LOG(LogTemp, Error, TEXT("ControllerBegin"));

}
