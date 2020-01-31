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
	bool isEscaping;
	bool isDamaging;
	bool damageFlashFlipFloop;
	FTimerHandle escapeTimerHandle;
	FTimerHandle damageTimerHandle;
	FTimerHandle disableInputTimerHandle;
	float damageTime = 0.0f;
	
	float escapeOffset = 0.0f;

	void DamageFlashing();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Val);

	void EscapeTwoWays();
	UPROPERTY()
		class AMainGameModeBase* gameMode;

	UPROPERTY()
		class UAnimMontage* DamageAnimation;


	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Cat)
		bool escapeFlipFloop;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Cat)
		float mEscapeLength;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Cat)
		float maxDamageTime = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Cat)
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


};
