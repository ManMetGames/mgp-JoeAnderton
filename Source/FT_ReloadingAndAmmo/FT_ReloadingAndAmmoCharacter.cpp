// Copyright Epic Games, Inc. All Rights Reserved.

#include "FT_ReloadingAndAmmoCharacter.h"
#include "FT_ReloadingAndAmmoProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include <TargetPanel.h>

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AFT_ReloadingAndAmmoCharacter

AFT_ReloadingAndAmmoCharacter::AFT_ReloadingAndAmmoCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));
}

void AFT_ReloadingAndAmmoCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AFT_ReloadingAndAmmoCharacter::OnHit);
	// initialise the hud, we can activate a HUD here but we will do a seperate focus on this
	if (HUDType)
	{
		HUD = CreateWidget<UUserWidget>(GetWorld(), HUDType);
		if (HUD)
		{
			HUD->AddToViewport();
		}
	}
}

//////////////////////////////////////////////////////////////////////////// Input

void AFT_ReloadingAndAmmoCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFT_ReloadingAndAmmoCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFT_ReloadingAndAmmoCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void AFT_ReloadingAndAmmoCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AFT_ReloadingAndAmmoCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AFT_ReloadingAndAmmoCharacter::Raycast() {
	FHitResult* hitResult = new FHitResult();
	//raycast starts from the gun and follows a bullet's trajectory
	FVector traceStart = FirstPersonCameraComponent->GetComponentLocation()+ FirstPersonCameraComponent->GetRightVector()*15+ FirstPersonCameraComponent->GetUpVector() * -15;
	FVector forwardVector = FirstPersonCameraComponent->GetForwardVector();
	FVector traceEnd = (forwardVector * 5000) + traceStart;
	FCollisionQueryParams* CQP = new FCollisionQueryParams();
	if (GetWorld()->LineTraceSingleByChannel(*hitResult, traceStart, traceEnd, ECC_Visibility, *CQP)) {
		//DrawDebugLine(GetWorld(), traceStart, traceEnd, FColor(0,50,100),false,0,0.5f,1);
		if (hitResult->GetActor() != nullptr) {  //checks if player is facing a target
			ATargetPanel* target = Cast<ATargetPanel>(hitResult->GetActor());
			if (target) {
				target->ChangeTarget(); //switches a target's material
			}
		}
	}
}

void AFT_ReloadingAndAmmoCharacter::RaycastDown() {
	FHitResult* hitResult = new FHitResult();
	FVector traceStart = GetActorLocation()-15*GetActorForwardVector();
	FVector forwardVector = GetActorUpVector();
	FVector traceEnd = (forwardVector * -5000) + traceStart;
	FCollisionQueryParams* CQP = new FCollisionQueryParams();
	if (GetWorld()->LineTraceSingleByChannel(*hitResult, traceStart, traceEnd, ECC_Visibility, *CQP)) {
		//DrawDebugLine(GetWorld(), traceStart, traceEnd, FColor(0, 50, 100), false, 0, 0.5f, 1);
		if (hitResult->GetActor() != nullptr) {
			ATargetPanel* target = Cast<ATargetPanel>(hitResult->GetActor());
			//changes a target to disabled if the player is directly above it
			if (target && target != targetTouched && targetTouched) {
  				targetTouched->CurrentState = TargetState::Enabled;
			}
			if (target) {
				target->CurrentState = TargetState::Disabled;
				targetTouched = target;
			}
			else if (targetTouched) {
				targetTouched->CurrentState = TargetState::Enabled; //resets previous target if player is no longer above it
			}
		}
	}
}

void AFT_ReloadingAndAmmoCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	RaycastDown(); //check for targets underneath the player 
	switch (NowState)
	{
	case ArmedState::Unarmed:
		break;
	case ArmedState::Armed:
		Raycast();
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Player ENUMS broke!"));
		break;
	}
}

void AFT_ReloadingAndAmmoCharacter::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//unused but I can't figure out how to get rid of it without causing errors
}