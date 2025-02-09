#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LR_PlayerCharacterPDA.generated.h"

UCLASS()
class LIGHTLESSREALM_API ULR_PlayerCharacterPDA : public UPrimaryDataAsset {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character Info")
	FText characterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character Info")
	FText characterDescription;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character Info")
	TObjectPtr<class UPaperFlipbook> characterFlipbook;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Debuffs")
	bool hasNoInnerLight = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Debuffs")
	bool easyToDetect = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Debuffs")
	bool needsMagikaToDealNormalDamage = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Debuffs")
	float magikaUsedToNormalDamage = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Debuffs")
	bool cannotEnterOnChurches = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Passives")
	bool doubleInnerLightSize = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Passives")
	bool hardToDetectOnShadows = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Passives")
	bool animalsArePassive = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Passives")
	bool herbsHealDouble = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Passives")
	bool dropsMoreMoney = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Passives")
	bool canEnterAnyChurch = false;
};
