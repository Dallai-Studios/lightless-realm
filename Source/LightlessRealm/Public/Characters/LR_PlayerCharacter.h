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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player Movement")
	float movementDelayTime;

	UPROPERTY(EditAnywhere, Category="Player Movement")
	float movementSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player Movement")
	float floorDetectionRaySize;

	UPROPERTY(EditAnywhere, Category="Player Attack")
	float meleeAttackRange;

protected:
	UPROPERTY(BlueprintReadWrite, Category="Player Movement")
	bool playerCanReceiveMovementInput = true;

	UPROPERTY(BlueprintReadWrite, Category="Player Attack")
	bool playerCanReceiveAttackInput = true;
	
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

	UFUNCTION(BlueprintCallable, Category="Player Attack")
	void AttackUp();

	UFUNCTION(BlueprintCallable, Category="Player Attack")
	void AttackDown();

	UFUNCTION(BlueprintCallable, Category="Player Attack")
	void AttackLeft();

	UFUNCTION(BlueprintCallable, Category="Player Attack")
	void AttackRight();

	UFUNCTION(BlueprintCallable, Category="Player Attack")
	void AnimateAttack(float flipbookMovementAmount);

private:
	void MoveCharacter(float deltaTime);
	bool CheckForPathBlock(ELRPlayerMovementDirection direction);
};
