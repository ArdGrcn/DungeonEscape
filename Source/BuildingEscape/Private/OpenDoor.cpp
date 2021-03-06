// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/AudioComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "OpenDoor.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	FindAudioComponent();
	SetupYawValues();
	FindPressurePlate();
}

void UOpenDoor::SetupYawValues()
{
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	// CloseAngle += CurrentYaw;
}

void UOpenDoor::FindPressurePlate() const
{
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has the OpenDoor component on it, but no PressurePlate set."), *GetOwner()->GetHumanReadableName());
	}
}

void UOpenDoor::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing audio component."), *GetOwner()->GetName());
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetTotalMassOfActors() >= MassTreshold)
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else
	{
		if (GetWorld()->GetTimeSeconds() - DoorLastOpened >= DoorCloseDelay)
		{
			CloseDoor(DeltaTime);
		}
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	CurrentYaw = FMath::FInterpTo(CurrentYaw, InitialYaw, DeltaTime, DoorOpenSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);
	if (!bDoorSoundSwitch)
	{
		if (!AudioComponent) {return;}
		AudioComponent->Play();
		bDoorSoundSwitch = true;
	}
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	CurrentYaw = FMath::FInterpTo(CurrentYaw, CloseAngle, DeltaTime, DoorCloseSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);
	if (bDoorSoundSwitch)
	{
		if (!AudioComponent) {return;}
		AudioComponent->Play();
		bDoorSoundSwitch = false;
	}
}

float UOpenDoor::GetTotalMassOfActors() const
{
	float TotalMass = 0.f;

	// find all overlapping actors
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) {return TotalMass;}
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	// add their masses
	for (AActor* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}

