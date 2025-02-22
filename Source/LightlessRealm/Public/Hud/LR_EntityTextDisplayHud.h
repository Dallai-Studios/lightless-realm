// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LR_EntityTextDisplayHud.generated.h"

UCLASS()
class LIGHTLESSREALM_API ULR_EntityTextDisplayHud : public UUserWidget {
	GENERATED_BODY()

public:
	FText textContent;

	
public:
	UFUNCTION(BlueprintCallable, Category="Text Modeling")
	void UpdateText(FText content);
	
};
