// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ResultGameMode.generated.h"

/**
 * 
 */
UCLASS()
class REMAKECATFESTIVAL_API AResultGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:
    UPROPERTY(EditAnywhere, Category = "Widget")
    TSubclassOf<class UUserWidget> ResultWidgetClass;

    class UUserWidget* ResultWidget;
protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
    FName TitleMapName = TEXT("TitleMap");

protected:
    virtual void BeginPlay() override;

protected:
    UFUNCTION(BlueprintPure, Category = "Widget")
    class UUserWidget* GetResultWidget()const;

    class UMainGameInstance* GetMainGameInstance()const;

    UFUNCTION(BlueprintImplementableEvent, Category = "Widget")
    void OnInitialRanking(int32 RankIndex,float CurrentTime,const TArray<float>& RankingTimes);
};
