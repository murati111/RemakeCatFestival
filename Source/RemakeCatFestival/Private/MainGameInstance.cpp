// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameInstance.h"
#include "Engine.h"


UMainGameInstance* UMainGameInstance::GetInstance()
{
	UMainGameInstance* instance = nullptr;
	if (GEngine)
	{
		FWorldContext* context = GEngine->GetWorldContextFromGameViewport(GEngine->GameViewport);
		instance = Cast<UMainGameInstance>(context->OwningGameInstance);
	}
	return instance;
}
