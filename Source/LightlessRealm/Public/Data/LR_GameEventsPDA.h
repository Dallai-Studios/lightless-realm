#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LR_GameEventsPDA.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDispatchGlobalEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDispatchGlobalEventOneParam, FVector, playerNextLocation);

UCLASS()
class LIGHTLESSREALM_API ULR_GameEventsPDA : public UPrimaryDataAsset {
	GENERATED_BODY()

public:
	// =================================================
	// Eventos Globais do Jogo:
	// =================================================
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Game Events")
	FOnDispatchGlobalEventOneParam OnPlayerPerformAction;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Game Events")
	FOnDispatchGlobalEvent OnReturnToMainMenu;
};
