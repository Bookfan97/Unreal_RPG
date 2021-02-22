// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePickup.h"
#include "Pickup.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API APickup : public ABasePickup
{
	GENERATED_BODY()
	public:
	APickup();

	///*/*Base collision#1#
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Item | Collision")
	//class USphereComponent* CollisionVolume;



	///*//Idle Particle System
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item | ParticleSystems")
	//UParticleSystemComponent* IdleParticleSystem;*/
	
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
};
