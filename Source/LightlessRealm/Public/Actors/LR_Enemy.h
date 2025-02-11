// Fill out your copyright notice in the Description page of Project Settings.

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
	UPROPERTY(EditAnywhere, Category="Components")
	TObjectPtr<class UCapsuleComponent> collision;
	
	UPROPERTY(EditAnywhere, Category="Components")
	TObjectPtr<class UPaperFlipbookComponent> flipbookComponent;
	
	UPROPERTY(EditAnywhere, Category="Data and Events")
	TObjectPtr<class ULR_GameEventsPDA> gameEvents;

	UPROPERTY(EditAnywhere, Category="Data and Events")
	TObjectPtr<class ULR_EnemyPDA> enemyConfig;

	UPROPERTY(EditAnywhere, Category="Enemy Movement")
	float movementSize;

	UPROPERTY(EditAnywhere, Category="Enemy Movement")
	float movementSpeed;

	UPROPERTY(EditAnywhere, Category="Enemy Movement")
	bool canOnlyMoveWithActiveTarget = false;

private:
	ELRPlayerMovementDirection movementDirection;
	ELRPlayerAttackDirection attackDirection;
	FVector destinationLocation;
	class AActor* activeTarget;
	ELRPlayerAttackDirection nextAttackDirection;
	
public:
	ALR_Enemy();
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category="Enemy Movement")
	void MoveUp();

	UFUNCTION(BlueprintCallable, Category="Enemy Movement")
	void MoveDown();

	UFUNCTION(BlueprintCallable, Category="Enemy, Movement")
	void MoveLeft();

	UFUNCTION(BlueprintCallable, Category="Enemy, Movement")
	void MoveRight();

	UFUNCTION(BlueprintCallable, Category="Enemy Response")
	void RespondToPlayerAction();

	UFUNCTION(BlueprintCallable)
	void CheckForTarget(
		class UPrimitiveComponent* overlapedComponent,
		AActor* otherActor,
		UPrimitiveComponent* otherComponent,
		int32 otherBodyIndex,
		bool fromSweep,
		const FHitResult &SweepResult
	);
	
private:
	UFUNCTION()
	void MoveEnemy(float deltaTime);

	UFUNCTION()
	bool CheckForPathBlock(ELRPlayerMovementDirection direction);

	UFUNCTION()
	bool ActiveTargetIsInAttackRange();
	
	UFUNCTION(CallInEditor)
	void Configure();

	UFUNCTION()
	void SetupEnemyBasedOnSelectedCharacter(class ULR_GameInstance* gameInstance);
};
