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
	void LoadingGhostData();
	FTimerHandle LoadingTimeHandle;
	int64 count = 0;

public:
	// Sets default values for this character's properties
	AGhost();
	UPROPERTY()
		class UMainGameInstance* GameInstance;
	UPROPERTY()
		class AMainGameModeBase* GameMode;

	UPROPERTY()
		class UAnimMontage* DamageAnimation;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Ghost")
		float Speed;

	void StartLoadingGhost();
	void StopLoadingGhost();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
