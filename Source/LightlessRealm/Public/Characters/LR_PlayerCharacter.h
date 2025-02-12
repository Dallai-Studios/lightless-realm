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

	UPROPERTY(EditAnywhere, Category="Components")
	TObjectPtr<class UTimelineComponent> attackTimelineComponent;	



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

	FTimerHandle movementCoolDownTimeHandle;



	// =========================================
	// Ataque do Player
	// =========================================
	UPROPERTY(EditAnywhere, Category="Player Attack")
	float meleeAttackRange;
	
	UPROPERTY(BlueprintReadWrite, Category="Player Attack")
	bool playerCanReceiveAttackInput = true;
	
	UPROPERTY(EditAnywhere, Category="Player Attack")
	float attackCooldownTime;

	UPROPERTY(EditAnywhere, Category="Player Attack")
	TObjectPtr<class UCurveVector> attackUpAnimationCurve;

	UPROPERTY(EditAnywhere, Category="Player Attack")
	TObjectPtr<UCurveVector> attackDownAnimationCurve;

	UPROPERTY(EditAnywhere, Category="Player Attack")
	TObjectPtr<UCurveVector> attackRightAnimationCurve;

	UPROPERTY(EditAnywhere, Category="Player Attack")
	TObjectPtr<UCurveVector> attackLeftAnimationCurve;

	UPROPERTY(EditAnywhere, Category="Player Attack")
	TSubclassOf<class UCameraShakeBase> baseAttackCameraShake;
	
	FTimerHandle attackCoolDownTimerHandle;

	

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
	FVector targetLocation;
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
	void MovePlayer(ELRPlayerMovementDirection movementDirection);

	UFUNCTION(Category="Player Movement")
	void MoveTowardsTargetLocation();

	UFUNCTION(Category="Player Movement")
	bool HasPathBlock(ELRPlayerMovementDirection direction);

	UFUNCTION(Category="Player Movement")
	void StartMovementCoolDown();

	UFUNCTION(Category="Player Movement")
	void HandleMovementEffects();



	// =================================================
	// Metodos de Ataque do Player:
	// =================================================
	UFUNCTION(BlueprintCallable, Category="Player Attack")
	void Attack(ELRPlayerAttackDirection attackDirection);
	
	UFUNCTION(BlueprintCallable, Category="Player Attack")
	void AnimateAttack(ELRPlayerAttackDirection attackDirection);

	UFUNCTION(Category="Player Attack")
	void UpdateAttackAnimation(FVector vectorValue);

	UFUNCTION(Category="Player Attack")
	void FinishAttackAnimation();

	UFUNCTION(Category="Player Attack")
	void StartAttackCooldown();

	UFUNCTION(Category="Player Attack")
	void HandleAttackCoolOff();

	UFUNCTION(Category="Player Attack")
	bool CheckForAttackableEntity(ELRPlayerAttackDirection AttackDirection);


	
	// =================================================
	// Metodos de Configuração do Player:
	// =================================================
	UFUNCTION(Category="Player Config")
	void ConfigureCharacter();
};
