// Fill out your copyright notice in the Description page of Project Settings.

#include "Cat.h"
#include "Public/MainGameModeBase.h"
#include "Public/CatInterface.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "TimerManager.h"
#include "Engine/SkeletalMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Materials/MaterialInstanceDynamic.h"

// Sets default values
ACat::ACat()
{

	
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	GetCapsuleComponent()->InitCapsuleSize(80.0f, 100.0f);

	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> PlayerMeshObj(TEXT("/Game/Cat/Charcter/SK_Cat.SK_Cat"));
	//if (PlayerMeshObj.Succeeded())
	//{
	//	GetMesh()->SetSkeletalMesh(PlayerMeshObj.Object);
	//	static ConstructorHelpers::FClassFinder<UAnimInstance> PlayerAnimBPClass(TEXT("/Game/Cat/Animation/ABP_Cat"));
	//	
	//	if (PlayerAnimBPClass.Class)
	//	{
	//		GetMesh()->SetAnimClass(PlayerAnimBPClass.Class);
	//	}
	//	static ConstructorHelpers::FObjectFinder<UAnimMontage> DamageAnimMontageObj(TEXT("/Game/Cat/Animation/AM_Damage.AM_Damage"));
	//	if (DamageAnimMontageObj.Succeeded())
	//	{
	//		DamageAnimation = DamageAnimMontageObj.Object;
	//	}

	//}
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
	
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("MeshCollision"));
	BoxComp->SetupAttachment(GetMesh());
	BoxComp->SetRelativeLocation(FVector(0.0f, -40.0f, 320.0f));
	BoxComp->SetRelativeScale3D(FVector(3.0f, 10.0f, 5.5f));
	FScriptDelegate CollisionBeginOverlap;
	CollisionBeginOverlap.BindUFunction(this, TEXT("OnOverlapBegin"));
	BoxComp->OnComponentBeginOverlap.Add(CollisionBeginOverlap);

}

// Called when the game starts or when spawned
void ACat::BeginPlay()
{
	Super::BeginPlay();
	SetMaxSpeedAndAccel(DefaultMaxSpeed, DefaultMaxAcceleration);
	UMaterialInterface* PostProcessMaterial = Cast<UMaterialInterface>(CatCameraComponent->PostProcessSettings.WeightedBlendables.Array[0].Object);
	if (PostProcessMaterial == nullptr) { return; }
	BlurMaterial = UMaterialInstanceDynamic::Create(PostProcessMaterial, this);
	UE_LOG(LogTemp, Log, TEXT("CatBegin"));
}

// Called every frame
void ACat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsDashing)
	{
		constexpr float MaxFov = 120.f;
		ChangeFovInSpeed(MaxFov, DashingMaxSpeed);

		constexpr float MaxBlurValue = 0.5f;
		ChangeMotionBlurValueInSpeed(MaxBlurValue, DashingMaxSpeed);
	
	}
	else
	{

		constexpr float MaxFov = 100.f;
		ChangeFovInSpeed(MaxFov, DefaultMaxSpeed);

	}
}
// Called to bind functionality to input
void ACat::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	//MoveForwardの入力が来たらMoveForward関数を呼び出す
	PlayerInputComponent->BindAxis("MoveForward", this, &ACat::MoveForward);
	PlayerInputComponent->BindAction("Escape", IE_Pressed,this, &ACat::EscapeTwoWays);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ACat::Dash);
	

}


void ACat::MoveForward(float Val)
{
	if (Val > 0)
	{
		//if (bIsDamaging) { return; }
		AddMovementInput(GetActorForwardVector(), Val);
	}
}

void ACat::EscapeTwoWays()
{
	if (bIsEscaping) { return; }
	//if (bIsDamaging) { return; }

	FTimerDelegate TimerDelegate;
	constexpr float EscapeIntervalTime = 0.001f;
	TimerDelegate.BindUFunction(this, FName("EscapeTwoWaysMoving"), EscapeFlipFloop);
	GetWorldTimerManager().SetTimer(EscapeTimerHandle, TimerDelegate, EscapeIntervalTime, true);
	if (EscapeSound != nullptr)
	{
		UGameplayStatics::PlaySound2D(this, EscapeSound);
	}
	EscapeFlipFloop = !EscapeFlipFloop;
}

void ACat::EscapeTwoWaysMoving(bool IsRight)
{
	constexpr float OffsetPerTime = 1.0f;
	const float ReverseScale = IsRight ? -1.0f : 1.0f;
	if (EscapeOffset < EscapeLength)
	{
		bIsEscaping = true;
		AddActorLocalOffset(FVector(0.f, ReverseScale * OffsetPerTime,0.f));
		EscapeOffset += OffsetPerTime;
	}
	else
	{
		bIsEscaping = false;
		GetWorldTimerManager().ClearTimer(EscapeTimerHandle);
		EscapeOffset = 0.0f;
	}
}

void ACat::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	int32 Point = 0;
	const bool bIsImplemted = OtherActor->GetClass()->ImplementsInterface(UCatInterface::StaticClass());
	const ICatInterface* Interface = Cast<ICatInterface>(OtherActor);

	if (!bIsImplemted) { return; }

	Interface->Execute_ReceiveDamage(OtherActor, Point);
	if (OtherActor->ActorHasTag("Item"))
	{
		AddDashPoint(Point);
		OnAddDashPointEvent(CurrentDashPoint,MaxDashPoint,!bIsDashing);
		return;
	}
	else if (OtherActor->ActorHasTag("Goal"))
	{
		RaceStopDelegate.ExecuteIfBound();
		//2秒経過後インプットをしないように
		constexpr float InputWaitTime = 2.0f;
		GetWorldTimerManager().SetTimer(DisableInputTimerHandle, this, &ACat::AfterGoalEvent, InputWaitTime, false);
		return;
	}
	FVector ImpulseVector = FVector(OtherActor->GetActorLocation().X - GetActorLocation().X, OtherActor->GetActorLocation().Y - GetActorLocation().Y,0.f);
	ImpulseVector.Normalize();
	Interface->Execute_ReceiveDamageForObscale(OtherActor, ImpulseVector,bIsDashing);
	if (OtherActor->ActorHasTag("Obstacle"))
	{
		if (bIsDashing) { return; }
		PlayAnimMontage(DamageAnimation);
		bIsDamaging = true;
		bIsHitObscle = true;
		Damage();
		return;
	}
}
void ACat::AddDashPoint(const int32 Point)
{
	if (bIsDashing) { return; }
	//UE_LOG(LogTemp, Error, TEXT("DashPoint:%d"), CurrentDashPoint);
	CurrentDashPoint = FMath::Clamp(CurrentDashPoint + Point, 0, MaxDashPoint);
	UE_LOG(LogTemp, Error, TEXT("DashPoint:%d"), CurrentDashPoint);
}
void ACat::AfterGoalEvent()
{
	//GameMode->RaceStop();
	RecordGhostDelegate.ExecuteIfBound();
	DisableInput(GetPlayerController());
}


void ACat::Damage()
{
	DisableInput(GetPlayerController());

	FTimerHandle HitObscaleTimeHandle;
	GetWorldTimerManager().SetTimer(HitObscaleTimeHandle, this, &ACat::AfterHitObscale, GetMainGameMode()->GetRecordingDeltaTime(), false);

	constexpr float DamageFlashingTime = 0.1f;
	GetWorldTimerManager().SetTimer(DamageTimerHandle, this, &ACat::DamageFlashing, DamageFlashingTime, true);

	FTimerHandle DamageFinishTimerHandle;
	GetWorldTimerManager().SetTimer(DamageFinishTimerHandle, this, &ACat::OnFinishedDamage, MaxDamageTime, false);
	
}
void ACat::OnFinishedDamage()
{
	EnableInput(GetPlayerController());
	bIsDamaging = false;
	SetActorHiddenInGame(false);
	GetWorldTimerManager().ClearTimer(DamageTimerHandle);
}



void ACat::Dash()
{
	UE_LOG(LogTemp, Error, TEXT("DASHHHHH"));
	if (CurrentDashPoint >= MaxDashPoint)
	{
		if (bIsDamaging) { return; }
		if (bIsDashing) { return; }
		bIsDashing = true;
		OnDecreaseDashingPointEvent();
		DashAction();
		
	}
}

void ACat::DashAction()
{
	SetMaxSpeedAndAccel(DashingMaxSpeed, DashingMaxAcceleration);
	FTimerHandle DashingTimerHandle;
	GetWorldTimerManager().SetTimer(DashingTimerHandle,this,&ACat::OnDashingEvent,DashingTime,false);
}

APlayerController* ACat::GetPlayerController() const
{
	return GetWorld()->GetFirstPlayerController();
}

AMainGameModeBase* ACat::GetMainGameMode() const
{
	return Cast<AMainGameModeBase>(GetWorld()->GetAuthGameMode());
}



void ACat::DamageFlashing()
{
	bIsDamaging = true;
	SetActorHiddenInGame(bDamageFlashFlipFloop);
	bDamageFlashFlipFloop = !bDamageFlashFlipFloop;
}

void ACat::OnDashingEvent()
{
	SetMaxSpeedAndAccel(DefaultMaxSpeed, DefaultMaxAcceleration);
	SetMotionBlurValue(0.f);
	bIsDashing = false;
	CurrentDashPoint = 0;
}

void ACat::AfterHitObscale()
{
	bIsHitObscle = false;
}

void ACat::SetMaxSpeedAndAccel(const float Speed, const float Accel)
{
	GetCharacterMovement()->MaxWalkSpeed = Speed;
	GetCharacterMovement()->MaxAcceleration = Accel;
	
}

void ACat::ChangeFovInSpeed(const float MaxFov, const float MaxSpeed)
{
	if (CatCameraComponent == nullptr) { return; }
	const float Speed = GetCharacterMovement()->Velocity.Size();
	constexpr float DefaultFov = 90.f;
	const float CurrentFov = (Speed / MaxSpeed * (MaxFov - DefaultFov)) + DefaultFov;
	CatCameraComponent->SetFieldOfView(CurrentFov);
}

void ACat::SetMotionBlurValue(const float BlurValue)
{
	if (BlurMaterial == nullptr || CatCameraComponent == nullptr) { return; }
	const FName ParameterName = TEXT("Blur");
	BlurMaterial->SetScalarParameterValue(ParameterName, BlurValue);
	CatCameraComponent->PostProcessSettings.WeightedBlendables.Array[0].Object = BlurMaterial;
}

void ACat::ChangeMotionBlurValueInSpeed(const float MaxBlurValue, const float MaxSpeed)
{
	const float Speed = GetCharacterMovement()->Velocity.Size();
	const float BlurValue = Speed / MaxSpeed * MaxBlurValue;
	SetMotionBlurValue(BlurValue); 
}