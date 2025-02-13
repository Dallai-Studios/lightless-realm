#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Enums/ELREnemyType.h"
#include "LR_EnemyPDA.generated.h"

UCLASS()
class LIGHTLESSREALM_API ULR_EnemyPDA : public UPrimaryDataAsset {
	GENERATED_BODY()

public:
	// =========================================
	// Infomações do Inimigo
	// =========================================
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy Info")
	FText enemyName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy Info")
	TObjectPtr<class UPaperFlipbook> enemyFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy Info")
	ELREnemyType enemyType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy Info")
	bool isSmall = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy Info")
	bool isBoss = false;
	
	
	
	// =========================================
	// Atributos do Inimigo
	// =========================================
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attributes")
	float health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attributes")
	float maxHealth;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attributes")
	float mana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attributes")
	float maxMana;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attributes")
	float sightSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attributes")
	float chanceToMissAttack;


	
	// =========================================
	// Skills do Inimigo
	// =========================================
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Skills")
	TObjectPtr<class UGameplayAbility> mainAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Skills")
	TObjectPtr<UGameplayAbility> secondAttack;
};
