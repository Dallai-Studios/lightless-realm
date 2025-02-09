#include "Characters/LR_PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "PaperFlipbookComponent.h"
#include "Data/LR_GameEventsPDA.h"
#include "Enums/ELRPlayerMovementDirection.h"
#include "GameFramework/SpringArmComponent.h"

ALR_PlayerCharacter::ALR_PlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	this->springArmComponent = this->CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	this->springArmComponent->SetupAttachment(this->RootComponent);
	
	this->cameraComponent = this->CreateDefaultSubobject<UCameraComponent>("Player Camera");
	this->cameraComponent->SetupAttachment(this->springArmComponent);

	this->flipbookComponent = this->CreateDefaultSubobject<UPaperFlipbookComponent>("Character Flipbook");
	this->flipbookComponent->SetupAttachment(this->RootComponent);
}

void ALR_PlayerCharacter::BeginPlay() {
	Super::BeginPlay();
	this->destinationLocation = this->GetActorLocation();
}

void ALR_PlayerCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	this->MoveCharacter(DeltaTime);
}

void ALR_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ALR_PlayerCharacter::MoveUp() {
	if (!this->playerCanReceiveMovementInput) return;
	if (IsValid(this->gameEvents)) this->gameEvents->OnPlayerPerformAction.Broadcast();
	if (this->CheckForPathBlock(ELRPlayerMovementDirection::DIRECTION_UP)) return;
	
	this->currentMovementDirection = ELRPlayerMovementDirection::DIRECTION_UP;
	FVector currentLocation = this->GetActorLocation();
	this->destinationLocation = FVector(currentLocation.X + this->movementSize, currentLocation.Y, currentLocation.Z);
}

void ALR_PlayerCharacter::MoveDown() {
	if (!this->playerCanReceiveMovementInput) return;
	if (IsValid(this->gameEvents)) this->gameEvents->OnPlayerPerformAction.Broadcast();
	if (this->CheckForPathBlock(ELRPlayerMovementDirection::DIRECTION_DOWN)) return;
	
	this->currentMovementDirection = ELRPlayerMovementDirection::DIRECTION_DOWN;
	FVector currentLocation = this->GetActorLocation();
	this->destinationLocation = FVector(currentLocation.X + (this->movementSize * -1), currentLocation.Y, currentLocation.Z);
}

void ALR_PlayerCharacter::MoveLeft() {
	if (!this->playerCanReceiveMovementInput) return;
	if (IsValid(this->gameEvents)) this->gameEvents->OnPlayerPerformAction.Broadcast();
	FVector flippedScale = this->flipbookComponent->GetRelativeScale3D();
	if (flippedScale.X < 0) flippedScale.X *= -1;
	this->flipbookComponent->SetRelativeScale3D(flippedScale);
	
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

void ALR_PlayerCharacter::AttackUp() {
	if (!this->playerCanReceiveAttackInput) return;
	if (IsValid(this->gameEvents)) this->gameEvents->OnPlayerPerformAction.Broadcast();
}

void ALR_PlayerCharacter::AttackDown() {
	if (!this->playerCanReceiveAttackInput) return;
	if (IsValid(this->gameEvents)) this->gameEvents->OnPlayerPerformAction.Broadcast();
}

void ALR_PlayerCharacter::AttackLeft() {
	if (!this->playerCanReceiveAttackInput) return;
	if (IsValid(this->gameEvents)) this->gameEvents->OnPlayerPerformAction.Broadcast();
}

void ALR_PlayerCharacter::AttackRight() {
	if (!this->playerCanReceiveAttackInput) return;
	if (IsValid(this->gameEvents)) this->gameEvents->OnPlayerPerformAction.Broadcast();
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
