// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Cat.generated.h"

UCLASS()
class REMAKECATFESTIVAL_API ACat : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACat();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	bool bIsEscaping;
	bool bIsDamaging;
	bool bDamageFlashFlipFloop;
	FTimerHandle EscapeTimerHandle;
	FTimerHandle DamageTimerHandle;
	FTimerHandle DisableInputTimerHandle;
	float EscapeOffset = 0.0f;
	
	UPROPERTY()
		class APlayerController* PlayerController;

	UPROPERTY(EditAnyWhere, Category = Cat)
		float MaxDamageTime = 1.0f;
	UPROPERTY(EditAnyWhere, Category = Cat)
		float DashingMaxSpeed = 2400.f;
	UPROPERTY(EditAnyWhere, Category = Cat)
		float DashingMaxAcceleration = 2400.f;
	UPROPERTY(EditAnyWhere, Category = Cat)
		float DefaultMaxSpeed = 1200.f;
	UPROPERTY(EditAnyWhere, Category = Cat)
		float DefaultMaxAcceleration = 900.f;
	void DamageFlashing();
	void OnDashingEvent();
	void AfterHitObscale();
	void AfterGoalEvent();
	void OnFinishedDamage();
	void SetMaxSpeedAndAccel(const float Speed, const float Accel);


public:	
	bool bIsHitObscle;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Val);

	void EscapeTwoWays();
	UPROPERTY()
		class AMainGameModeBase* GameMode;

	UPROPERTY()
		class UAnimMontage* DamageAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* EscapeSound;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Cat)
		bool EscapeFlipFloop;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Cat)
		float EscapeLength;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Cat)
		class UBoxComponent* BoxComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* CatCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SpringArm, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CatSpringArm;

	UFUNCTION()
		void EscapeTwoWaysMoving(bool IsRight);

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void Damage();

	UFUNCTION()
		void Dash();

	void DashAction();

	UFUNCTION(BlueprintImplementableEvent, Category = "Cat")
		void OnAddDashPoint();
	UFUNCTION(BlueprintImplementableEvent, Category = "Cat")
		void DecreasePointOnDashing();
	UFUNCTION(BlueprintPure, Category = "Cat")
		class AMainGameModeBase* GetMainGameMode();





};
