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
	FTimerHandle escapeTimerHandle;
	float escapeOffset = 0.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Val);

	void EscapeTwoWays();


	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Cat)
		bool escapeFlipFloop;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Cat)
		float mEscapeLength;


	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Cat)
		class UBoxComponent* BoxComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CatCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SpringArm, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CatSpringArm;

	UFUNCTION()
	void EscapeTwoWaysMoving(bool IsRight);

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


};
