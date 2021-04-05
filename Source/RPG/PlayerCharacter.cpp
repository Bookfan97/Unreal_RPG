// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"



#include "DrawDebugHelpers.h"
#include "InteractionInterface.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	/** Create Camera boom */
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 600.f;
	CameraBoom->bUsePawnControlRotation = true;

	/** Create Camera */
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Follow Camera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; //Use the controller rotation

	/** Create Sphere Collider*/
	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Interaction Sphere"));
	SphereCollider->SetSphereRadius(25.f);
	SphereCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollider->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereCollider->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereCollider->SetupAttachment(RootComponent);

	/** Set size for collision capsule */
	GetCapsuleComponent()->SetCapsuleSize(48.f, 95.f);
	
	/** Base defaults for rates	 */
	BaseTurnRate = 65.f;
	BaseLookUpRate = 65.f;

	/** Set rotation for the controller	 */
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	/** Set rotation for the character towards movement direction	 */
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 840.f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 650.f;
	GetCharacterMovement()->AirControl = 0.2f;

	sprintSpeed = 900.f;
	walkSpeed = 650.f;

	bShiftKeyDown = false;
	
	/*Default Player Stats*/
	maxHealth=100.f;
	health = 75.f;
	stamina = 120.f;
	maxStamina = 350.f;
	mana = 75.f;
	maxMana = 350.f;
	currency = 0;

	/*Initialize Enums*/
	MovementStatus = EMovementStatus::EMS_Walk;
	StaminaStatus = EStaminaStatus::ESS_Default;

	staminaDrainRate = 25.f;
	minSprintStamina = 50.f;
}

void APlayerCharacter::SetMovementStatus(EMovementStatus status)
{
	MovementStatus = status;
	if (MovementStatus == EMovementStatus::EMS_Sprint)
	{
		GetCharacterMovement()->MaxWalkSpeed = sprintSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	}
}

void APlayerCharacter::ShiftKeyDown()
{
	bShiftKeyDown = true;
}

void APlayerCharacter::ShiftKeyUp()
{
	bShiftKeyDown = false;
}


void APlayerCharacter::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Other Actor Found"));
	if (OtherActor->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor w/ Interaction Found"));
		interactables.AddUnique(OtherActor);
	}
	else
	{
		auto OAComp = OtherActor->GetComponentsByInterface(UInteractionInterface::StaticClass());
		for (auto comp : OAComp)
		{
			IInteractionInterface* InteractiveActor = Cast<IInteractionInterface>(comp);
			interactables.AddUnique(Cast<AActor>(InteractiveActor));
		}
	}
}

void APlayerCharacter::EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Other Actor Found"));
	if (OtherActor->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor w/ Interaction Found"));
		interactables.Remove(OtherActor);
	}
}


// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	SphereCollider->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::BeginOverlap);
	SphereCollider->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::EndOverlap);
}

void APlayerCharacter::SetStaminaUpdate(float DeltaTime)
{
	float deltaStamina = staminaDrainRate * DeltaTime;
	switch (StaminaStatus)
	{
		case EStaminaStatus::ESS_Default:
			if (bShiftKeyDown)
			{
				if(stamina - deltaStamina<= minSprintStamina)
				{
					SetStaminaStatus(EStaminaStatus::ESS_Empty);
					stamina -= deltaStamina;
				}
				else
				{
					stamina -= deltaStamina;
				}
				SetMovementStatus(EMovementStatus::EMS_Sprint);
			}
			else
			{
				if (stamina + deltaStamina >= maxStamina)
				{
					stamina = maxStamina;
				}
				else
				{
					stamina += deltaStamina;
				}
				SetMovementStatus(EMovementStatus::EMS_Walk);
			}
			break;
		case EStaminaStatus::ESS_Min:
			break;
		case EStaminaStatus::ESS_BelowMin:
			if (bShiftKeyDown)
			{
				if (stamina - deltaStamina <= 0.f)
				{
					SetStaminaStatus(EStaminaStatus::ESS_Recover);
					stamina = 0;
					SetMovementStatus(EMovementStatus::EMS_Default);
				}
				else
				{
					stamina -= deltaStamina;
					SetMovementStatus(EMovementStatus::EMS_Sprint);
				}
			}
			else
			{
				if (stamina + deltaStamina >= minSprintStamina)
				{
					SetStaminaStatus(EStaminaStatus::ESS_Default);
					stamina+=deltaStamina;
				}
				else
				{
					stamina += deltaStamina;
				}
				SetMovementStatus(EMovementStatus::EMS_Default);
			}
			break;
		case EStaminaStatus::ESS_Recover:
			if (stamina + deltaStamina >= minSprintStamina)
			{
				SetStaminaStatus(EStaminaStatus::ESS_Default);
				stamina+=deltaStamina;
			}
			else
			{
				stamina+= deltaStamina;
			}
			SetMovementStatus(EMovementStatus::EMS_Default);
			break;
		case EStaminaStatus::ESS_Empty:
			if (bShiftKeyDown)
			{
				stamina=0;
			}
			else
			{
				SetStaminaStatus(EStaminaStatus::ESS_Recover);
				stamina+=deltaStamina;
			}
			SetMovementStatus(EMovementStatus::EMS_Default);
			break;
		case EStaminaStatus::ESS_Max:
			break;
		default:
			break;
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
		SetStaminaUpdate(DeltaTime);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Override with local movement functions
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APlayerCharacter::LookUpRate);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::Interaction);
	//PlayerInputComponent->BindAction("Interact", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &APlayerCharacter::ShiftKeyDown);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &APlayerCharacter::ShiftKeyUp);
}

void APlayerCharacter::MoveForward(float moveValue)
{
	//Check for movement and whether controller is created
	if (Controller != nullptr && moveValue != 0.0f)
	{
		//Find forward direction
		const FRotator rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, rotation.Yaw, 0.0f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, moveValue);
	}
}

void APlayerCharacter::MoveRight(float moveValue)
{
	//Check for movement and whether controller is created
	if (Controller != nullptr && moveValue != 0.0f)
	{
		//Find forward direction
		const FRotator rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, rotation.Yaw, 0.0f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, moveValue);
	}
}

void APlayerCharacter::TurnAtRate(float turnRate)
{
	AddControllerYawInput(turnRate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookUpRate(float turnRate)
{
	AddControllerPitchInput(turnRate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

FVector LineTraceDistance;

void APlayerCharacter::Interaction()
{
	if(interactables.Num()>0)
	{
		//TODO: Add logic to check for closest; will use first object for now
		IInteractionInterface::Execute_OnInteraction(interactables[0]);
	}
}


