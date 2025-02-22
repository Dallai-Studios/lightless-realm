#pragma once
#include "CoreMinimal.h"
#include "Enums/ELRPlayerAttackDirection.h"
#include "GameFramework/Actor.h"
#include "Enums/ELRPlayerMovementDirection.h"
#include "Interfaces/LR_DamageInterface.h"
#include "LR_Enemy.generated.h"

UCLASS()
class LIGHTLESSREALM_API ALR_Enemy : public AActor, public ILR_DamageInterface {
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
	TObjectPtr<class UBoxComponent> playerDetectionBox;
	
	UPROPERTY(EditAnywhere, Category="Components")
	TObjectPtr<class UTimelineComponent> attackTimelineComponent;

	UPROPERTY(EditAnywhere, Category="Components")
	TObjectPtr<class ULR_FootstepAudioComponent> footstepComponent;

	

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
	FTimerHandle movementDelayTimer;


	
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

	UPROPERTY(EditAnywhere, Category="Enemy Attack")
	float detectionSize = 100;

	UPROPERTY(EditAnywhere, Category="Enemy Attack")
	bool isPassive = false;
	
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
	void MoveEnemy(ELRPlayerMovementDirection movementDirection, FVector playerNextLocation = FVector::Zero());

	UFUNCTION(Category="Enemy Movement")
	void MoveTowardsTargetLocation();

	UFUNCTION(Category="Enemy Movement")
	bool HasPathBlock(ELRPlayerMovementDirection direction);

	UFUNCTION(Category="Enemy Movement")
	void PlayFootstepSound();


	
	// =================================================
	// Metodos de Movemento do Inimigo:
	// =================================================
	UFUNCTION(Category="Enemy Attack")
	void Attack(ELRPlayerAttackDirection attackDirection);

	UFUNCTION(BlueprintCallable, Category="Enemy Attack")
	void AnimateAttack(ELRPlayerAttackDirection attackDirection);

	UFUNCTION(Category="Enemy Attack")
	void UpdateAttackAnimation(FVector vectorValue);

	UFUNCTION(Category="Enemy Attack")
	void FinishAttackAnimation();
	
	UFUNCTION(Category="Enemy Attack")
	bool ActiveTargetIsInAttackRange();

	

	// =================================================
	// Event Listeners:
	// =================================================
	UFUNCTION(BlueprintCallable, Category="Event Listeners")
	void RespondToPlayerAction(FVector playerNextLocation);

	UFUNCTION(BlueprintCallable)
	void CheckForTarget(AActor* otherActor);



	// =================================================
	// Metodos de Configuração:
	// =================================================
	UFUNCTION(CallInEditor)
	void Configure();

	UFUNCTION()
	void SetupEnemyBasedOnSelectedCharacter();
};
