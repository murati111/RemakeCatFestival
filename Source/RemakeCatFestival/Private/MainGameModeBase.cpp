// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameModeBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Public/CatController.h"
#include "TimerManager.h"
#include "Public/MainGameInstance.h"

AMainGameModeBase::AMainGameModeBase(const FObjectInitializer& ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<APawn> PawnClass(TEXT("/Game/Cat/BP/BP_Cat"));
	if (PawnClass.Succeeded())
	{
		DefaultPawnClass = PawnClass.Class;
	}
	PlayerControllerClass = ACatController::StaticClass();
	gameInstance = UMainGameInstance::GetInstance();

}

void AMainGameModeBase::BeginPlay()
{
	GetWorldTimerManager().SetTimer(GameTimeHandle,this,&AMainGameModeBase::TimerCount,0.01f, true);
}

void AMainGameModeBase::TimerCount()
{
	gameInstance->Time += 0.01f;
	UE_LOG(LogTemp, Error, TEXT("Overlap %f"), gameInstance->Time);
}