#include "Characters/LR_PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "PaperFlipbookComponent.h"
#include "Components/SpotLightComponent.h"
#include "Data/LR_GameEventsPDA.h"
#include "Data/LR_GameInstance.h"
#include "Data/LR_PlayerCharacterPDA.h"
#include "Enums/ELRPlayerAttackDirection.h"
#include "Enums/ELRPlayerMovementDirection.h"
#include "GameFramework/SpringArmComponent.h"
#include "AbilitySystemComponent.h"


// =================================================
// Metodos de Life Cycle:
// =================================================
ALR_PlayerCharacter::ALR_PlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	this->springArmComponent = this->CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	this->springArmComponent->SetupAttachment(this->RootComponent);
	
	this->cameraComponent = this->CreateDefaultSubobject<UCameraComponent>("Player Camera");
	this->cameraComponent->SetupAttachment(this->springArmComponent);

	this->flipbookComponent = this->CreateDefaultSubobject<UPaperFlipbookComponent>("Character Flipbook");
	this->flipbookComponent->SetupAttachment(this->RootComponent);

	this->innerLight = this->CreateDefaultSubobject<USpotLightComponent>("Inner Light");
	this->innerLight->SetupAttachment(this->RootComponent);

	this->abilitySystemComponent = this->CreateDefaultSubobject<UAbilitySystemComponent>("Ability System");
}


void ALR_PlayerCharacter::BeginPlay() {
	Super::BeginPlay();

	ULR_GameInstance* gameInstance = CastChecked<ULR_GameInstance>(this->GetWorld()->GetGameInstance());

	if (IsValid(gameInstance->gameSelectedCharacter)) {
		this->selectedCharacter = gameInstance->gameSelectedCharacter;
	} 

	this->ConfigureCharacter();
	
	this->destinationLocation = this->GetActorLocation();
}


void ALR_PlayerCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	this->MoveTowardsDestinyLocation();
}



// =================================================
// Metodos de Movimento do Player:
// =================================================
void ALR_PlayerCharacter::MoveUp() {
	if (!this->playerCanReceiveMovementInput || this->CheckForPathBlock(ELRPlayerMovementDirection::DIRECTION_UP)) return;

	if (IsValid(this->gameEvents)) this->gameEvents->OnPlayerPerformAction.Broadcast();

	this->destinationLocation = this->GetActorForwardVector() * this->movementSize;
}

void ALR_PlayerCharacter::MoveDown() {
	if (!this->playerCanReceiveMovementInput || this->CheckForPathBlock(ELRPlayerMovementDirection::DIRECTION_DOWN)) return;
	
	if (IsValid(this->gameEvents)) this->gameEvents->OnPlayerPerformAction.Broadcast();

	this->destinationLocation = this->GetActorForwardVector() * this->movementSize * -1;
}

void ALR_PlayerCharacter::MoveLeft() {
	if (!this->playerCanReceiveMovementInput) return;
	
	FVector flippedScale = this->flipbookComponent->GetRelativeScale3D();
	if (flippedScale.X < 0) flippedScale.X *= -1;
	this->flipbookComponent->SetRelativeScale3D(flippedScale);

	if (IsValid(this->gameEvents)) this->gameEvents->OnPlayerPerformAction.Broadcast();
	
	if (this->CheckForPathBlock(ELRPlayerMovementDirection::DIRECTION_LEFT)) return;
	
	this->currentMovementDirection = ELRPlayerMovementDirection::DIRECTION_LEFT;
	FVector currentLocation = this->GetActorLocation();
	this->destinationLocation = FVector(currentLocation.X, currentLocation.Y + (this->movementSize * -1), currentLocation.Z);
}

void ALR_PlayerCharacter::MoveRight() {
	if (!this->playerCanReceiveMovementInput) return;
	if (IsValid(this->gameEvents)) this->gameEvents->OnPlayerPerformAction.Broadcast();
	FVector flippedScale = this->flipbookComponent->GetRelativeScale3D();
	if (flippedScale.X > 0) flippedScale.X *= -1;
	this->flipbookComponent->SetRelativeScale3D(flippedScale);

	if (this->CheckForPathBlock(ELRPlayerMovementDirection::DIRECTION_RIGHT)) return;
	
	this->currentMovementDirection = ELRPlayerMovementDirection::DIRECTION_RIGHT;
	FVector currentLocation = this->GetActorLocation();
	this->destinationLocation = FVector(currentLocation.X, currentLocation.Y + this->movementSize, currentLocation.Z);
}



// =================================================
// Metodos de Ataque do Player:
// =================================================
void ALR_PlayerCharacter::AttackUp() {
	if (!this->playerCanReceiveAttackInput) return;
	this->currentAttackDirection = ELRPlayerAttackDirection::ATTACK_UP;
	if (IsValid(this->gameEvents)) this->gameEvents->OnPlayerPerformAction.Broadcast();
}


void ALR_PlayerCharacter::AttackDown() {
	if (!this->playerCanReceiveAttackInput) return;
	this->currentAttackDirection = ELRPlayerAttackDirection::ATTACK_DOWN;
	if (IsValid(this->gameEvents)) this->gameEvents->OnPlayerPerformAction.Broadcast();
}


void ALR_PlayerCharacter::AttackLeft() {
	if (!this->playerCanReceiveAttackInput) return;

	FVector flippedScale = this->flipbookComponent->GetRelativeScale3D();
	if (flippedScale.X < 0) flippedScale.X *= -1;
	this->flipbookComponent->SetRelativeScale3D(flippedScale);
	
	this->currentAttackDirection = ELRPlayerAttackDirection::ATTACK_LEFT;
	if (IsValid(this->gameEvents)) this->gameEvents->OnPlayerPerformAction.Broadcast();
}


void ALR_PlayerCharacter::AttackRight() {
	if (!this->playerCanReceiveAttackInput) return;
	
	FVector flippedScale = this->flipbookComponent->GetRelativeScale3D();
	if (flippedScale.X > 0) flippedScale.X *= -1;
	this->flipbookComponent->SetRelativeScale3D(flippedScale);
	
	this->currentAttackDirection = ELRPlayerAttackDirection::ATTACK_RIGHT;
	if (IsValid(this->gameEvents)) this->gameEvents->OnPlayerPerformAction.Broadcast();
}


void ALR_PlayerCharacter::AnimateAttack(float flipbookMovementAmount) {
	auto newPosition = this->flipbookComponent->GetRelativeLocation();
	
	if (this->currentAttackDirection == ELRPlayerAttackDirection::ATTACK_UP) {
		newPosition = FVector(newPosition.X + flipbookMovementAmount, newPosition.Y, newPosition.Z);
	}

	if (this->currentAttackDirection == ELRPlayerAttackDirection::ATTACK_DOWN) {
		newPosition = FVector(newPosition.X - flipbookMovementAmount, newPosition.Y, newPosition.Z);
	}

	if (this->currentAttackDirection == ELRPlayerAttackDirection::ATTACK_LEFT) {
		newPosition = FVector(newPosition.X, newPosition.Y - flipbookMovementAmount, newPosition.Z);
	}

	if (this->currentAttackDirection == ELRPlayerAttackDirection::ATTACK_RIGHT) {
		newPosition = FVector(newPosition.X, newPosition.Y + flipbookMovementAmount, newPosition.Z);
	}

	this->flipbookComponent->SetRelativeLocation(newPosition);
}


void ALR_PlayerCharacter::MoveTowardsDestinyLocation() {
	auto currentPosition = this->GetActorLocation();

	if (FVector::Dist(currentPosition, this->destinationLocation) < 1.0f) {
		this->SetActorLocation(this->destinationLocation);
		return;
	}

	auto newPosition = FMath::VInterpConstantTo(currentPosition, this->destinationLocation, this->GetWorld()->GetDeltaSeconds(), this->movementSpeed);
	this->SetActorLocation(newPosition);
}


void ALR_PlayerCharacter::ConfigureCharacter() {
	check(this->selectedCharacter);

	this->flipbookComponent->SetFlipbook(this->selectedCharacter->characterFlipbook);

	if (this->selectedCharacter->hasNoInnerLight) {
		this->innerLight->SetVisibility(false);	
	}

	if (this->selectedCharacter->doubleInnerLightSize) {
		this->innerLight->SetInnerConeAngle(this->doubleInnerLightSize);
		this->innerLight->SetOuterConeAngle(this->doubleInnerLightSize);
	} else {
		this->innerLight->SetInnerConeAngle(this->defaultInnerLightSize);
		this->innerLight->SetOuterConeAngle(this->defaultInnerLightSize);
	}
}


void ALR_PlayerCharacter::MoveCharacter(float deltaTime) {
	if (!this->canMove) return;
	if (this->destinationLocation == this->GetActorLocation()) return;
	FVector newLocation = FMath::LerpStable(this->GetActorLocation(), this->destinationLocation, deltaTime * this->movementSpeed);
	this->SetActorLocation(newLocation);
}


bool ALR_PlayerCharacter::CheckForPathBlock(ELRPlayerMovementDirection direction) {
	auto lineStart = this->GetActorLocation();
	FHitResult hitResult;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	
	FVector lineEnd;
	
	if (direction == ELRPlayerMovementDirection::DIRECTION_UP) lineEnd = lineStart + (this->GetActorForwardVector() * this->movementSize);
	if (direction == ELRPlayerMovementDirection::DIRECTION_DOWN) lineEnd = lineStart + (this->GetActorForwardVector() * this->movementSize * -1);
	if (direction == ELRPlayerMovementDirection::DIRECTION_RIGHT) lineEnd = lineStart + (this->GetActorRightVector() * this->movementSize);
	if (direction == ELRPlayerMovementDirection::DIRECTION_LEFT) lineEnd = lineStart + (this->GetActorRightVector() * this->movementSize * -1);
	
	this->GetWorld()->LineTraceSingleByChannel(hitResult, lineStart, lineEnd, ECC_Visibility, params);
	
	DrawDebugLine(this->GetWorld(), lineStart, lineEnd, hitResult.IsValidBlockingHit() ? FColor::Green : FColor::Red, false, 10);
	
	return hitResult.IsValidBlockingHit();
}


bool ALR_PlayerCharacter::CheckForAttackableEntity(ELRPlayerAttackDirection attackDirection) {
	auto lineStart = this->GetActorLocation();
	FHitResult hitResult;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	
	FVector lineEnd;
	
	if (attackDirection == ELRPlayerAttackDirection::ATTACK_UP) lineEnd = lineStart + (this->GetActorForwardVector() * this->movementSize);
	if (attackDirection == ELRPlayerAttackDirection::ATTACK_DOWN) lineEnd = lineStart + (this->GetActorForwardVector() * this->movementSize * -1);
	if (attackDirection == ELRPlayerAttackDirection::ATTACK_RIGHT) lineEnd = lineStart + (this->GetActorRightVector() * this->movementSize);
	if (attackDirection == ELRPlayerAttackDirection::ATTACK_LEFT) lineEnd = lineStart + (this->GetActorRightVector() * this->movementSize * -1);
	
	this->GetWorld()->LineTraceSingleByChannel(hitResult, lineStart, lineEnd, ECC_Visibility, params);
	
	DrawDebugLine(this->GetWorld(), lineStart, lineEnd, hitResult.IsValidBlockingHit() ? FColor::Green : FColor::Red, false, 10);
	
	return hitResult.IsValidBlockingHit();
}
