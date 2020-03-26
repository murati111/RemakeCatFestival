// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Cat.generated.h" 

DECLARE_DELEGATE(FRaceStopDelegate)
DECLARE_DELEGATE(FRecordGhostDelegate)

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
	bool bIsEscaping = false;
	bool bIsDamaging = false;
	bool bDamageFlashFlipFloop;
	FTimerHandle EscapeTimerHandle;
	FTimerHandle DamageTimerHandle;
	FTimerHandle DisableInputTimerHandle;
	float EscapeOffset = 0.0f;
	bool bIsHitObscle = false;
	UPROPERTY(EditAnyWhere, Category = Cat)
		float MaxDamageTime = 1.0f;	
	
	UPROPERTY(EditAnywhere,Category= Cat)
		class UAnimMontage* DamageAnimation;

	UPROPERTY(EditAnywhere, Category = Gameplay)
		class USoundBase* EscapeSound;

	UPROPERTY(EditAnyWhere, Category = Cat)
		bool EscapeFlipFloop = true;

	UPROPERTY(EditAnyWhere, Category = Cat)
		float EscapeLength = 300.f;
private:
	UPROPERTY(EditAnyWhere, Category = Dash)
		float DashingMaxSpeed = 2400.f;
	UPROPERTY(EditAnyWhere, Category = Dash)
		float DashingMaxAcceleration = 2400.f;
	UPROPERTY(EditAnyWhere, Category = Dash)
		float DefaultMaxSpeed = 1200.f;
	UPROPERTY(EditAnyWhere, Category = Dash)
		float DefaultMaxAcceleration = 900.f;
	UPROPERTY(EditAnyWhere, Category = Dash)
		float DashingTime = 2.0f;
	UPROPERTY()
		int32 CurrentDashPoint = 0;
	UPROPERTY()
		int32 MaxDashPoint = 200;
	UPROPERTY()
		bool bIsDashing = false;
	UPROPERTY()
		class UMaterialInstanceDynamic* BlurMaterial;
	void AddDashPoint(const int32 Point);
	void DashAction();
	void Dash();
	void OnDashingEvent();
	void ChangeFovInSpeed(const float MaxFov, const float MaxSpeed);
	void SetMotionBlurValue(const float BlurValue);
	void ChangeMotionBlurValueInSpeed(const float MaxBlurValue, const float MaxSpeed);


protected:
	UFUNCTION(BlueprintPure,Category="Dash")
	bool IsDashing() const { return bIsDashing; }

private:
	void DamageFlashing();

	void AfterHitObscale();
	void AfterGoalEvent();
	void OnFinishedDamage();
	void SetMaxSpeedAndAccel(const float Speed, const float Accel);

//GameModeÇÃä÷êîåƒÇ—èoÇµópDelegate
public:
	FRaceStopDelegate RaceStopDelegate;
	FRecordGhostDelegate RecordGhostDelegate;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Cat)
		class UBoxComponent* BoxComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* CatCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SpringArm, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CatSpringArm;

	bool IsHitObscle()const { return bIsHitObscle; }
protected:
	UFUNCTION()
	void EscapeTwoWaysMoving(bool IsRight);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	void Damage();
	class AMainGameModeBase* GetMainGameMode() const;
	APlayerController* GetPlayerController() const;

	void MoveForward(float Val);
	void EscapeTwoWays();

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Cat")
		void OnAddDashPointEvent(const int32 CurrentPoint, const int32 MaxPoint,const bool IsNotDashing);
	UFUNCTION(BlueprintImplementableEvent, Category = "Cat")
		void OnDecreaseDashingPointEvent();

};
