// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Public/CatInterface.h"
#include "SuperItem.generated.h"

UCLASS()
class REMAKECATFESTIVAL_API ASuperItem : public AActor,public ICatInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASuperItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:
	void PlaneRotation();

public:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Item)
		FName ActorTag = "Item";

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = Item)
	class UStaticMeshComponent* Plane;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = Item)
		class UBoxComponent* BoxComp;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Item)
		class UMaterialInterface* PlaneMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Item)
	class USoundBase* ItemGetSound;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Item)
		int32 DashPoint = 10;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CatInterface")
		void ReceiveDamage(int32 &point);
		virtual void ReceiveDamage_Implementation(int32& point);

};
