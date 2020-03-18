// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TitleGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class REMAKECATFESTIVAL_API ATitleGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

private:
    UPROPERTY(EditAnywhere, Category = "Widget")
        TSubclassOf<class UUserWidget> TitleWidgetClass;

    UPROPERTY()
    class UUserWidget* TitleWidget;


protected:
    UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Level",meta=(AllowPrivateAcces = "true"))
    FName MainMapName = TEXT("TestMap");

protected:
    virtual void BeginPlay() override;

protected:
    UFUNCTION(BlueprintPure, Category = "Widget")
    class UUserWidget* GetTitleWidget()const;

    UFUNCTION(BlueprintImplementableEvent, Category = "GameData")
    void LoadGhostTimeAndDoesGhostDataExists(const TArray<float>& GhostTimes,const TArray<bool>& DoesExists);
    
};
