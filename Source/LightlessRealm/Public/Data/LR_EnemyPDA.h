#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LR_EnemyPDA.generated.h"

UCLASS()
class LIGHTLESSREALM_API ULR_EnemyPDA : public UPrimaryDataAsset {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="Enemy Properties")
	FText enemyName;

	UPROPERTY(EditAnywhere, Category="Enemy Properties")
	TObjectPtr<class UPaperFlipbook> enemyFlipbook;
};
