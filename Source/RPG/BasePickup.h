// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasePickup.generated.h"

UCLASS()
class RPG_API ABasePickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABasePickup();

	/*Base collision*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Item | Collision")
	class USphereComponent* CollisionVolume;

	/*Base mesh component*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Item | Mesh")
	class UStaticMeshComponent* Mesh;

	//Idle Particle System
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item | ParticleSystems")
	UParticleSystemComponent* IdleParticleSystem;

	//Particle System on overlap
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item | ParticleSystems")
	class UParticleSystem* OverlapParticles;

	//Sound on overlap
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item | Sounds")
	class USoundCue* OverlapSound;

	//Set whether the item can rotate
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item | Properties")
	bool bRotate;

	//Set rotation rate
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item | Properties")
	float RotationRate;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
