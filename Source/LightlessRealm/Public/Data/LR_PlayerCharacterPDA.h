#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LR_PlayerCharacterPDA.generated.h"

UCLASS()
class LIGHTLESSREALM_API ULR_PlayerCharacterPDA : public UPrimaryDataAsset {
	GENERATED_BODY()

public:
	// =========================================
	// Infomações do Personagem
	// =========================================
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character Info")
	FText characterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character Info")
	FText characterDescription;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character Info")
	TObjectPtr<class UPaperFlipbook> characterFlipbook;


	
	// =========================================
	// Debuffs naturais do Personagem
	// =========================================
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



	// =========================================
	// Passivas naturias do Personagem
	// =========================================
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Passives")
	bool doubleDamageOnUndeads = false;



	// =========================================
	// Atributos do Personagem
	// =========================================
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attributes")
	float health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attributes")
	float maxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attributes")
	float magika;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attributes")
	float maxMagika;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attributes")
	int32 attacksPerTurn = 1;
	
	
	
	// =========================================
	// Skills do personagem
	// =========================================
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Skills")
	TObjectPtr<class UGameplayAbility> mainAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Skills")
	TObjectPtr<UGameplayAbility> secondAttack;
};
