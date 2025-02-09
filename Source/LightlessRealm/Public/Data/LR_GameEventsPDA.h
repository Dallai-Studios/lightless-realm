#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LR_GameEventsPDA.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDispatchGlobalEvent);

UCLASS()
class LIGHTLESSREALM_API ULR_GameEventsPDA : public UPrimaryDataAsset {
	GENERATED_BODY()

public:
	FOnDispatchGlobalEvent OnPlayerPerformAction;
};
