#pragma once

#include "CoreMinimal.h"
#include "Enums/ELRPlayerAttackDirection.h"
#include "Enums/ELRPlayerMovementDirection.h"
#include "GameFramework/Character.h"
#include "LR_PlayerCharacter.generated.h"

UCLASS()
class LIGHTLESSREALM_API ALR_PlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<class UAbilitySystemComponent> abilitySystemComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Data and Events")
	TObjectPtr<class ULR_GameEventsPDA> gameEvents;
	
	UPROPERTY(EditAnywhere, Category="Components")
	TObjectPtr<class USpringArmComponent> springArmComponent;
	
	UPROPERTY(EditAnywhere, Category="Components")
	TObjectPtr<class UCameraComponent> cameraComponent;

	UPROPERTY(EditAnywhere, Category="Components")
	TObjectPtr<class UPaperFlipbookComponent> flipbookComponent;

	UPROPERTY(EditAnywhere, Category="Components")
	TObjectPtr<class USpotLightComponent> innerLight;

	UPROPERTY(EditAnywhere, Category="Player Character")
	class ULR_PlayerCharacterPDA* selectedCharacter;

	UPROPERTY(EditAnywhere, Category="Player Character")
	float defaultInnerLightSize = 26;

	UPROPERTY(EditAnywhere, Category="Player Character")
	float doubleInnerLightSize = 38;
	
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

	UPROPERTY(BlueprintReadWrite, Category="Player Attack")
	bool playerCanPerformAttackAnimation = true;

	UPROPERTY(BlueprintReadWrite, Category="Player Inputs")
	bool isPlayerPassingTurn = false;
	
private:
	bool canMove = true;
	FVector destinationLocation;
	ELRPlayerMovementDirection currentMovementDirection;
	ELRPlayerAttackDirection currentAttackDirection;
	
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
	void ConfigureCharacter();
	void MoveCharacter(float deltaTime);
	bool CheckForPathBlock(ELRPlayerMovementDirection direction);
	bool CheckForAttackableEntity(ELRPlayerAttackDirection AttackDirection);
};
