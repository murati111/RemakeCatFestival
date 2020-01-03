// Fill out your copyright notice in the Description page of Project Settings.


#include "Cat.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "TimerManager.h"
#include "Engine/SkeletalMesh.h"


// Sets default values
ACat::ACat()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> PlayerMeshObj(TEXT("/Game/Cat/Charcter/SK_Cat.SK_Cat"));
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->InitCapsuleSize(80.0f, 100.0f);
	GetMesh()->SetSkeletalMesh(PlayerMeshObj.Object);
	GetMesh()->SetRelativeScale3D(FVector(0.25f, 0.25f, 0.25f));
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	CatSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CatSpringArm"));
	CatSpringArm->SetupAttachment(GetCapsuleComponent());
	CatCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CatCameraComponent"));
	CatCameraComponent->SetupAttachment(CatSpringArm);
	CatSpringArm->SetRelativeLocation(FVector(80.0f, 0.0f, 50.0f));
	CatCameraComponent->SetRelativeLocation(FVector(-90, 0, 100));
	CatCameraComponent->SetRelativeRotation(FRotator(-20.0f, 0.0f, 0.0f));
	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->OnComponentBeginOverlap.AddDynamic(this, &ACat::OnOverlapBegin);
	escapeFlipFloop = true;
	isEscaping = false;
}

// Called when the game starts or when spawned
void ACat::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACat::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	//MoveForward‚Ì“ü—Í‚ª—ˆ‚½‚çMoveForwardŠÖ”‚ðŒÄ‚Ño‚·
	PlayerInputComponent->BindAxis("MoveForward", this, &ACat::MoveForward);

	PlayerInputComponent->BindAction("Escape", IE_Pressed,this, &ACat::EscapeTwoWays);
	

}


void ACat::MoveForward(float Val)
{
	if (Val > 0)
	{
		//GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		AddMovementInput(GetActorForwardVector(), Val);
	}
}

void ACat::EscapeTwoWays()
{
	if (!isEscaping)
	{
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUFunction(this, FName("EscapeTwoWaysMoving"), escapeFlipFloop);
		GetWorldTimerManager().SetTimer(escapeTimerHandle, TimerDelegate, 0.001f, true);
		//EscapeTwoWaysMoving(escapeFlipFloop);
	
		escapeFlipFloop = !escapeFlipFloop;
	}

}

void ACat::EscapeTwoWaysMoving(bool IsRight)
{
	const float OffsetPerTime = 1.0f;
	float reverseScale = 1.0f;
	if (IsRight)
	{
		reverseScale = -1.0f;
	}
	else
	{
		reverseScale = 1.0f;
	}
	if (escapeOffset < mEscapeLength)
	{
		isEscaping = true;
		
		FVector addOffset;
		addOffset = FVector(GetActorLocation().X, GetActorLocation().Y+(reverseScale*OffsetPerTime), GetActorLocation().Z);
		SetActorLocation(addOffset);
		escapeOffset += OffsetPerTime;
	}
	else
	{
		isEscaping = false;
		GetWorldTimerManager().ClearTimer(escapeTimerHandle);
		escapeOffset = 0.0f;
	}
}

void ACat::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
		UE_LOG(LogTemp, Error, TEXT("Overlap"));
}