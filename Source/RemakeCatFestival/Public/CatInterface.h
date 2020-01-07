// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCatInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

/**
 * 
 */
class REMAKECATFESTIVAL_API ICatInterface
{
	GENERATED_IINTERFACE_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable,Category = "CatInterface")
		void ReceiveDamage(float A);
};
