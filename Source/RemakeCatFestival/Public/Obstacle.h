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
	UPROPERTY(EditDefaultsOnly,Category = Obstacle)
	class USoundBase* HitObstacleSound;

	UPROPERTY(EditDefaultsOnly, Category = Obstacle)
	class USoundBase* BreakObstacleSound;

	UPROPERTY(EditDefaultsOnly, Category = Obstacle)
	FName ActorTag = "Obstacle";
public:


	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = Obstacle)
	class UStaticMeshComponent* Ramune;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = Obstacle)
	class UDestructibleComponent* DestructibleComponent;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = Obstacle)
	class UBoxComponent* BoxComp;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CatInterface")
	void ReceiveDamageForObscale(FVector HitNormalVector, bool IsDashing);
	virtual void ReceiveDamageForObscale_Implementation(FVector HitNormalVector,bool IsDashing);

};
