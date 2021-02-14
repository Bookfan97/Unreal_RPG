// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class RPG_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	
	/** Set camera and camera boom to player	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta=(AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Forward declaring camera component	 */
	class UCameraComponent* FollowCamera;

	/** Base turn rate to scale turning functions for camera	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;
	
	/** Base turn rate to scale look up functions for camera	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Player forwards/backwards movement */
	void MoveForward(float moveValue);

	/** Player Left/Right movement */
	void MoveRight(float moveValue);

	/** Turn at a set rate given input values
	 *@param turnRate is the normalized rate*/
	void TurnAtRate(float turnRate);

	/** Look Up/Down at a set rate given input values
	 *@param turnRate is the normalized rate*/
	void LookUpRate(float turnRate);

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const {return CameraBoom;}
	FORCEINLINE class UCameraComponent* GetFollowCamera() const {return FollowCamera;}
};
