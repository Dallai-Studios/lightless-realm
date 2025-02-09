#pragma once

#include "CoreMinimal.h"
#include "Enums/ELRPlayerMovementDirection.h"
#include "GameFramework/Character.h"
#include "LR_PlayerCharacter.generated.h"

UCLASS()
class LIGHTLESSREALM_API ALR_PlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="Data and Events")
	TObjectPtr<class ULR_GameEventsPDA> gameEvents;
	
	UPROPERTY(EditAnywhere, Category="Components")
	TObjectPtr<class USpringArmComponent> springArmComponent;
	
	UPROPERTY(EditAnywhere, Category="Components")
	TObjectPtr<class UCameraComponent> cameraComponent;

	UPROPERTY(EditAnywhere, Category="Components")
	TObjectPtr<class UPaperFlipbookComponent> flipbookComponent;

	UPROPERTY(EditAnywhere, Category="Player Movement")
	float movementSpeed;

	UPROPERTY(EditAnywhere, Category="Player Movement")
	float movementSize;

	UPROPERTY(EditAnywhere, Category="Player Attack")
	float meleeAttackRange;

private:
	bool canMove = true;
	FVector destinationLocation;
	ELRPlayerMovementDirection currentMovementDirection;
	
public:
	ALR_PlayerCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category="Player Movement")
	void MoveUp();

	UFUNCTION(BlueprintCallable, Category="Player Movement")
	void MoveDown();

	UFUNCTION(BlueprintCallable, Category="Player Movement")
	void MoveLeft();
	
	UFUNCTION(BlueprintCallable, Category="Player Movement")
	void MoveRight();

	UFUNCTION(BlueprintCallable, Category="Player Movement")
	void AttackUp();

	UFUNCTION(BlueprintCallable, Category="Player Movement")
	void AttackDown();

	UFUNCTION(BlueprintCallable, Category="Player Movement")
	void AttackLeft();

	UFUNCTION(BlueprintCallable, Category="Player Movement")
	void AttackRight();

private:
	void MoveCharacter(float deltaTime);
	bool CheckForPathBlock(ELRPlayerMovementDirection direction);
};
