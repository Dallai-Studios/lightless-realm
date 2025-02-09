#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LR_PlayerCharacterPDA.generated.h"

UCLASS()
class LIGHTLESSREALM_API ULR_PlayerCharacterPDA : public UPrimaryDataAsset {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText characterName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UPaperFlipbook> characterFlipbook;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool hasNoInnerLight = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool doubleInnerLightSize;
};
