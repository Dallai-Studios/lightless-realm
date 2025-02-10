// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums/ELRPlayerAttackDirection.h"
#include "GameFramework/Actor.h"
#include "Enums/ELRPlayerMovementDirection.h"
#include "LR_Enemy.generated.h"

class USphereComponent;
class UCapsuleComponent;
class UPaperFlipbookComponent;
class ULR_GameEventsPDA;
class ULR_EnemyPDA;
class UPrimitiveComponent;
class AActor;

UCLASS()
class LIGHTLESSREALM_API ALR_Enemy : public AActor {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="Components")
	TObjectPtr<USphereComponent> playerDetectionSphere;
	
	UPROPERTY(EditAnywhere, Category="Components")
	TObjectPtr<UCapsuleComponent> collision;
	
	UPROPERTY(EditAnywhere, Category="Components")
	TObjectPtr<UPaperFlipbookComponent> flipbookComponent;

	UPROPERTY(EditAnywhere, Category="Data and Events")
	TObjectPtr<ULR_GameEventsPDA> gameEvents;

	UPROPERTY(EditAnywhere, Category="Data and Events")
	TObjectPtr<ULR_EnemyPDA> enemyConfig;

	UPROPERTY(EditAnywhere, Category="Enemy Movement")
	float movementSize;

	UPROPERTY(EditAnywhere, Category="Enemy Movement")
	float movementSpeed;

	UPROPERTY(EditAnywhere, Category="Enemy Movement")
	bool canMoveUp = true, canMoveDown = true, canMoveLeft = true, canMoveRight = true;

private:
	ELRPlayerMovementDirection movementDirection;
	ELRPlayerAttackDirection attackDirection;
	FVector destinationLocation;
	AActor* activeTarget;
	
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
		UPrimitiveComponent* overlapedComponent,
		AActor* otherActor,
		UPrimitiveComponent otherComponent,
		int32 otherBodyIndex,
		bool fromSweep,
		const FHitResult &SweepResult
	);
	
private:
	UFUNCTION()
	void MoveEnemy(float deltaTime);

	UFUNCTION()
	bool CheckForPathBlock(ELRPlayerMovementDirection direction);
	
	UFUNCTION(CallInEditor)
	void Configure();

	UFUNCTION()
	void SetupEnemyBasedOnSelectedCharacter();
};
