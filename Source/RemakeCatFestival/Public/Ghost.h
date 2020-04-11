// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Public/MainGameInstance.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Ghost.generated.h"

UCLASS()
class REMAKECATFESTIVAL_API AGhost : public ACharacter
{
	GENERATED_BODY()

private:

	FTimerHandle LoadingTimeHandle;

	UPROPERTY()
	int32 LoadingGhostCount = 0;

	//UPROPERTY()
	//class UMainGameInstance* GameInstance;
	
	UPROPERTY()
	FCatGhost LoadGhostData;

	void LoadingGhostData();

private:
	UPROPERTY(EditDefaultsOnly,Category = "Ghost")
	class UAnimMontage* DamageAnimation;

	class AMainGameModeBase* GetGameMode();
protected:
	virtual void BeginPlay()override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Ghost")
	void UpdateAnimationSpeed(float Velocity);
public:
	// Sets default values for this character's properties
	AGhost();

	UPROPERTY(BlueprintReadOnly,Category="Ghost")
	float Speed;

	void StartLoadingGhost();
	void StopLoadingGhost();
	virtual void Tick(float DeltaTime)override;
};
