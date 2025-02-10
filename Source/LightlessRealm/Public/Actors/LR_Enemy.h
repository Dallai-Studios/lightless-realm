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

private:
	ELRPlayerMovementDirection movementDirection;
	ELRPlayerAttackDirection attackDirection;
	FVector destinationLocation;
	
public:
	ALR_Enemy();
	virtual void BeginPlay() override;
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

private:
	void MoveEnemy(float deltaTime);
	bool CheckForPathBlock(ELRPlayerMovementDirection direction);
};
