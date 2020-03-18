// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Public/CatInterface.h"
#include "Obstacle.generated.h"

UCLASS()
class REMAKECATFESTIVAL_API AObstacle : public AActor, public ICatInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObstacle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere,Category = Obstacle)
		class USoundBase* HitObstacleSound;
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Obstacle)
		FName ActorTag = "Obstacle";
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = Obstacle)
		class UStaticMeshComponent* Ramune;


	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = Obstacle)
		class UBoxComponent* BoxComp;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CatInterface")
		void ReceiveDamage(int32& point);
	virtual void ReceiveDamage_Implementation(int32& point);

};
