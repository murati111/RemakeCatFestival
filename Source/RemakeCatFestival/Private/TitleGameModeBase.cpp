// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "MainGameInstance.h"

void ATitleGameModeBase::BeginPlay()
{
	if (TitleWidgetClass)
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			TitleWidget = CreateWidget<UUserWidget>(PlayerController, TitleWidgetClass);
			if (TitleWidget)
			{
				TitleWidget->AddToViewport();
			}
			PlayerController->bShowMouseCursor = true;
		}
	}
	UMainGameInstance::GetInstance()->InitialInstanceValues();
	TArray<float> RankingTimes;
	TArray<bool> DoesGhostDataExists;
	UMainGameInstance::GetInstance()->CreateGhostTimesAndDoesGhostDataExists(RankingTimes, DoesGhostDataExists);
	LoadGhostTimeAndDoesGhostDataExists(RankingTimes, DoesGhostDataExists);
	Super::BeginPlay();
}

UUserWidget* ATitleGameModeBase::GetTitleWidget() const
{
	return TitleWidget;
}
