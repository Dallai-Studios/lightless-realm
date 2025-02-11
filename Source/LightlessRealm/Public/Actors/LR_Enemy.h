#pragma once
#include "CoreMinimal.h"
#include "Enums/ELRPlayerAttackDirection.h"
#include "GameFramework/Actor.h"
#include "Enums/ELRPlayerMovementDirection.h"
#include "LR_Enemy.generated.h"

UCLASS()
class LIGHTLESSREALM_API ALR_Enemy : public AActor {
	GENERATED_BODY()

public:
	// =================================================
	// Components:
	// =================================================
	UPROPERTY(EditAnywhere, Category="Components")
	TObjectPtr<class UCapsuleComponent> collision;
	
	UPROPERTY(EditAnywhere, Category="Components")
	TObjectPtr<class UPaperFlipbookComponent> flipbookComponent;



	// =================================================
	// Dados e Eventos:
	// =================================================
	UPROPERTY(EditAnywhere, Category="Data and Events")
	TObjectPtr<class ULR_GameEventsPDA> gameEvents;

	UPROPERTY(EditAnywhere, Category="Data and Events")
	TObjectPtr<class ULR_EnemyPDA> enemyConfig;



	// =================================================
	// Movimento do Inimigo:
	// =================================================
	UPROPERTY(EditAnywhere, Category="Enemy Movement")
	float movementSize;

	UPROPERTY(EditAnywhere, Category="Enemy Movement")
	float movementSpeed;

	UPROPERTY(EditAnywhere, Category="Enemy Movement")
	bool canOnlyMoveWithActiveTarget = false;

	FVector destinationLocation;
	ELRPlayerMovementDirection movementDirection;
	
private:
	ELRPlayerAttackDirection attackDirection;
	class AActor* activeTarget;
	ELRPlayerAttackDirection nextAttackDirection;
	
public:
	// =================================================
	// Metodos de Life Cycle:
	// =================================================
	ALR_Enemy();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;



	// =================================================
	// Metodos de Movemento do Inimigo:
	// =================================================
	UFUNCTION(BlueprintCallable, Category="Enemy Movement")
	void MoveUp();

	UFUNCTION(BlueprintCallable, Category="Enemy Movement")
	void MoveDown();

	UFUNCTION(BlueprintCallable, Category="Enemy, Movement")
	void MoveLeft();

	UFUNCTION(BlueprintCallable, Category="Enemy, Movement")
	void MoveRight();

	void MoveTowardsDestinyLocation();
	bool CheckForPathBlock(ELRPlayerMovementDirection direction);


	
	// =================================================
	// Metodos de Movemento do Inimigo:
	// =================================================
	void AttackUp();
	void AttackDown();
	void AttackRight();
	void AttackLeft();
	bool ActiveTargetIsInAttackRange();

	

	// =================================================
	// Event Listeners:
	// =================================================
	UFUNCTION(BlueprintCallable, Category="Event Listeners")
	void RespondToPlayerAction();

	UFUNCTION(BlueprintCallable)
	void CheckForTarget(AActor* otherActor);



	// =================================================
	// Metodos de Configuração:
	// =================================================
	UFUNCTION(CallInEditor)
	void Configure();

	UFUNCTION()
	void SetupEnemyBasedOnSelectedCharacter(class ULR_GameInstance* gameInstance);
};
