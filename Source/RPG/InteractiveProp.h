// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "InteractionInterface.h"
#include "GameFramework/Actor.h"
#include "InteractiveProp.generated.h"

UCLASS()
class RPG_API AInteractiveProp : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractiveProp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
	void OnInteraction();
	virtual void OnInteraction_Implementation() override;
};
