// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueComponent.h"

#include "DialogueAIController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UDialogueComponent::UDialogueComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDialogueComponent::BeginPlay()
{
	Super::BeginPlay();

	auto AIController = GetWorld()->SpawnActor<ADialogueAIController>();
}


// Called every frame
void UDialogueComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDialogueComponent::OnInteraction_Implementation()
{
	UWorld* world = GetWorld();
	APlayerController* PlayerController = world->GetFirstPlayerController();
	FInputModeUIOnly InputModeUIOnly;
	
	//Create the HUD Overlay
	DialogueOverlay = CreateWidget<UUserWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), DialogueOverlayAsset);
	//TODO: Bind to OnExit
		
	//Add to viewport/Display on screen
	DialogueOverlay->AddToViewport();
	DialogueOverlay->SetVisibility(ESlateVisibility::Visible);
	InputModeUIOnly.SetWidgetToFocus(DialogueOverlay->TakeWidget());
	InputModeUIOnly.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerController->SetInputMode(InputModeUIOnly);
	PlayerController->bShowMouseCursor = true;
	//TODO: Stop behavior tree
	//TODO: Remove from parent
}

