// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LR_FootstepAudioComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LIGHTLESSREALM_API ULR_FootstepAudioComponent : public UActorComponent {
	GENERATED_BODY()

public:
	// ======================================================
	// Ciclo de vida:
	// ======================================================
	ULR_FootstepAudioComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
