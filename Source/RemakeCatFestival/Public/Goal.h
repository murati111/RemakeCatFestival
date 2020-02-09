// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Public/CatInterface.h"
#include "Goal.generated.h"

UCLASS()
class REMAKECATFESTIVAL_API AGoal : public AActor, public ICatInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGoal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Goal)
		class UBoxComponent* BoxComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Goal)
		class USceneComponent* NewRootComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Goal)
		FName ActorTag = "Goal";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* GoalSE;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CatInterface")
		void ReceiveDamage(int32& point);
	virtual void ReceiveDamage_Implementation(int32& point);

};
