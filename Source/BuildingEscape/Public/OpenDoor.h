// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float GetTotalMassOfActors() const;
	void FindAudioComponent();
	void FindPressurePlate() const;
	void SetupYawValues();

private:
	float InitialYaw; 
	float CurrentYaw;

	// Tracks whether door sound is played
	bool bDoorSoundSwitch = true;

	UPROPERTY(EditAnywhere)
	float CloseAngle = 0.f;
	
	float DoorLastOpened = 0.f;

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 2.f;

	UPROPERTY(EditAnywhere)
	float DoorOpenSpeed = 1.75f;

	UPROPERTY(EditAnywhere)
	float DoorCloseSpeed = 1.75f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
	float MassTreshold = 55.f;

	UPROPERTY()
	UAudioComponent* AudioComponent = nullptr;
};
