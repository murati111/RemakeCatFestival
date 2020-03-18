// Fill out your copyright notice in the Description page of Project Settings.


#include "ResultGameMode.h"
#include "Blueprint/UserWidget.h"
#include "MainGameInstance.h"

void AResultGameMode::BeginPlay()
{
	
	if (ResultWidgetClass)
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			ResultWidget = CreateWidget<UUserWidget>(PlayerController, ResultWidgetClass);
			if (ResultWidget)
			{
				ResultWidget->AddToViewport();
			}
		}
		PlayerController->bShowMouseCursor = true;
		UE_LOG(LogTemp,Error,TEXT("CREATEWIDGETTTTT"))
	}
	TArray<float> RankingTimes;
	GetMainGameInstance()->CreateGhostTimes(RankingTimes);
	OnInitialRanking(GetMainGameInstance()->GetCurrentRank(), GetMainGameInstance()->GetCurrentTime(), RankingTimes);
	Super::BeginPlay();
}

UUserWidget* AResultGameMode::GetResultWidget() const
{
	return ResultWidget;
}

UMainGameInstance* AResultGameMode::GetMainGameInstance() const
{
	return UMainGameInstance::GetInstance();
}
