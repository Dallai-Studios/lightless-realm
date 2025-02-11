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
	// =========================================
	// Components
	// =========================================
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<class UAbilitySystemComponent> abilitySystemComponent;
	
	UPROPERTY(EditAnywhere, Category="Components")
	TObjectPtr<class USpringArmComponent> springArmComponent;
	
	UPROPERTY(EditAnywhere, Category="Components")
	TObjectPtr<class UCameraComponent> cameraComponent;

	UPROPERTY(EditAnywhere, Category="Components")
	TObjectPtr<class UPaperFlipbookComponent> flipbookComponent;

	UPROPERTY(EditAnywhere, Category="Components")
	TObjectPtr<class USpotLightComponent> innerLight;



	// =========================================
	// Data e Eventos
	// =========================================
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Data and Events")
	TObjectPtr<class ULR_GameEventsPDA> gameEvents;

	UPROPERTY(EditAnywhere, Category="Data and Events")
	class ULR_PlayerCharacterPDA* selectedCharacter;


	
	// =========================================
	// Movimento do Player
	// =========================================
	UPROPERTY(EditAnywhere, Category="Player Movement")
	float movementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player Movement")
	float movementDelayTime;

	UPROPERTY(EditAnywhere, Category="Player Movement")
	float movementSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player Movement")
	float floorDetectionRaySize;

	UPROPERTY(BlueprintReadWrite, Category="Player Movement")
	bool playerCanReceiveMovementInput = true;

	UPROPERTY(BlueprintReadWrite, Category="Player Movement")
	bool isPlayerPassingTurn = false;



	// =========================================
	// Movimento do Player
	// =========================================
	UPROPERTY(EditAnywhere, Category="Player Attack")
	float meleeAttackRange;
	
	UPROPERTY(BlueprintReadWrite, Category="Player Attack")
	bool playerCanReceiveAttackInput = true;

	UPROPERTY(BlueprintReadWrite, Category="Player Attack")
	bool playerCanPerformAttackAnimation = true;

	

	// =========================================
	// Valores Auxiliares
	// =========================================
	UPROPERTY(EditAnywhere, Category="Player Values")
	float defaultInnerLightSize = 26;

	UPROPERTY(EditAnywhere, Category="Player Values")
	float doubleInnerLightSize = 38;
	

	
	// =========================================
	// Variavei de Controle
	// =========================================
	FVector destinationLocation;
	ELRPlayerMovementDirection currentMovementDirection;
	ELRPlayerAttackDirection currentAttackDirection;
	
	
public:
	// =================================================
	// Metodos de Life Cycle:
	// =================================================
	ALR_PlayerCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;



	// =================================================
	// Metodos de Movimento do Player:
	// =================================================
	UFUNCTION(BlueprintCallable, Category="Player Movement")
	void MoveUp();

	UFUNCTION(BlueprintCallable, Category="Player Movement")
	void MoveDown();

	UFUNCTION(BlueprintCallable, Category="Player Movement")
	void MoveLeft();
	
	UFUNCTION(BlueprintCallable, Category="Player Movement")
	void MoveRight();

	void MoveTowardsDestinyLocation();
	bool CheckForPathBlock(ELRPlayerMovementDirection direction);



	// =================================================
	// Metodos de Ataque do Player:
	// =================================================
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

	bool CheckForAttackableEntity(ELRPlayerAttackDirection AttackDirection);


	
	// =================================================
	// Metodos de Configuração do Player:
	// =================================================
	void ConfigureCharacter();

private:
	void MoveCharacter(float deltaTime);
};
