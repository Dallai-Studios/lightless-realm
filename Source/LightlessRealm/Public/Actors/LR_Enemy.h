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

	UPROPERTY(EditAnywhere, Category="Components")
	TObjectPtr<class UTimelineComponent> attackTimelineComponent;

	

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

	FVector targetLocation;


	
	// =================================================
	// Ataque do Inimigo:
	// =================================================
	UPROPERTY(EditAnywhere, Category="Enemy Attack")
	TObjectPtr<class UCurveVector> attackUpAnimationCurve;

	UPROPERTY(EditAnywhere, Category="Enemy Attack")
	TObjectPtr<UCurveVector> attackDownAnimationCurve;

	UPROPERTY(EditAnywhere, Category="Enemy Attack")
	TObjectPtr<UCurveVector> attackRightAnimationCurve;

	UPROPERTY(EditAnywhere, Category="Enemy Attack")
	TObjectPtr<UCurveVector> attackLeftAnimationCurve;
	
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
	UFUNCTION(Category="Enemy Movement")
	void MoveEnemy(ELRPlayerMovementDirection movementDirection);

	UFUNCTION(Category="Enemy Movement")
	void MoveTowardsTargetLocation();

	UFUNCTION(Category="Enemy Movement")
	bool HasPathBlock(ELRPlayerMovementDirection direction);


	
	// =================================================
	// Metodos de Movemento do Inimigo:
	// =================================================
	UFUNCTION(Category="Enemy Attack")
	void Attack(ELRPlayerAttackDirection AttackDirection);

	UFUNCTION(BlueprintCallable, Category="Enemy Attack")
	void AnimateAttack(ELRPlayerAttackDirection attackDirection);

	UFUNCTION(Category="Enemy Attack")
	void UpdateAttackAnimation(FVector vectorValue);
	
	UFUNCTION(Category="Enemy Attack")
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
