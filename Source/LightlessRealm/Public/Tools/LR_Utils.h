// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LR_Utils.generated.h"

UCLASS()
class LIGHTLESSREALM_API ULR_Utils : public UBlueprintFunctionLibrary {
	GENERATED_BODY()

public:
	static void ShowDebugMessage(FString message, FColor messageColor = FColor::Orange);
};
