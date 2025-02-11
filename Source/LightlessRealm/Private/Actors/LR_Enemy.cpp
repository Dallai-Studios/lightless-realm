#include "Actors/LR_Enemy.h"
#include "PaperFlipbookComponent.h"
#include "Characters/LR_PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Data/LR_EnemyPDA.h"
#include "Data/LR_GameEventsPDA.h"
#include "Data/LR_GameInstance.h"
#include "Data/LR_PlayerCharacterPDA.h"
#include "Tools/LR_Utils.h"



// =================================================
// Metodos de Life Cycle:
// =================================================
ALR_Enemy::ALR_Enemy() {
	PrimaryActorTick.bCanEverTick = true;

	this->collision = this->CreateDefaultSubobject<UCapsuleComponent>("Collision Capsule");
	this->RootComponent = this->collision;

	this->flipbookComponent = this->CreateDefaultSubobject<UPaperFlipbookComponent>("Flipbook Component");
	this->flipbookComponent->SetupAttachment(this->collision);
	
	if (this->enemyConfig != nullptr) {
		this->flipbookComponent->SetFlipbook(this->enemyConfig->enemyFlipbook);
	}
}

void ALR_Enemy::BeginPlay() {
	Super::BeginPlay();

	// essa configuração serve só pra ler todas as propriedades do data asset que representa esse inimigo
	// nenhuma configuração é feita para validar o personagem selecionado pelo jogador. -Renan
	this->Configure();

	if (this->gameEvents) this->gameEvents->OnPlayerPerformAction.AddDynamic(this, &ALR_Enemy::ALR_Enemy::RespondToPlayerAction);

	this->destinationLocation = this->GetActorLocation();

	ULR_GameInstance* gameInstance = Cast<ULR_GameInstance>(this->GetWorld()->GetGameInstance());
	if (gameInstance->gameSelectedCharacter) this->SetupEnemyBasedOnSelectedCharacter(gameInstance);
}

void ALR_Enemy::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	this->MoveTowardsDestinyLocation();

	if (IsValid(this->activeTarget)) {
		DrawDebugLine(this->GetWorld(), this->GetActorLocation(), this->activeTarget->GetActorLocation(), FColor::Orange);
	}
}



// =================================================
// Metodos de Movemento do Inimigo:
// =================================================
void ALR_Enemy::MoveUp() {
	if (this->canOnlyMoveWithActiveTarget && !this->activeTarget) return;
	
	if (this->HasPathBlock(ELRPlayerMovementDirection::DIRECTION_UP)) return;
	
	this->destinationLocation = this->GetActorLocation() + this->GetActorForwardVector() * this->movementSize;
}

void ALR_Enemy::MoveDown() {
	if (this->canOnlyMoveWithActiveTarget && !this->activeTarget) return;
	
	if (this->HasPathBlock(ELRPlayerMovementDirection::DIRECTION_DOWN)) return;
	
	this->destinationLocation = this->GetActorLocation() + (this->GetActorForwardVector() * this->movementSize * -1);
}

void ALR_Enemy::MoveLeft() {
	if (this->canOnlyMoveWithActiveTarget && !this->activeTarget) return;
	
	if (this->HasPathBlock(ELRPlayerMovementDirection::DIRECTION_LEFT)) return;

	FVector flippedScale = this->flipbookComponent->GetRelativeScale3D();
	if (flippedScale.X < 0) flippedScale.X *= -1;
	this->flipbookComponent->SetRelativeScale3D(flippedScale);
	
	this->destinationLocation = this->GetActorLocation() + (this->GetActorRightVector() * this->movementSize * -1);
}

void ALR_Enemy::MoveRight() {
	if (this->canOnlyMoveWithActiveTarget && !this->activeTarget) return;

	if (this->HasPathBlock(ELRPlayerMovementDirection::DIRECTION_RIGHT)) return;

	FVector flippedScale = this->flipbookComponent->GetRelativeScale3D();
	if (flippedScale.X > 0) flippedScale.X *= -1;
	this->flipbookComponent->SetRelativeScale3D(flippedScale);
	
	this->destinationLocation = this->GetActorLocation() + (this->GetActorRightVector() * this->movementSize * -1);
}

void ALR_Enemy::MoveTowardsDestinyLocation() {
	auto currentPosition = this->GetActorLocation();

	if (FVector::Dist(currentPosition, this->destinationLocation) < 1.0f) {
		this->SetActorLocation(this->destinationLocation);
		return;
	}

	auto newPosition = FMath::VInterpConstantTo(currentPosition, this->destinationLocation, this->GetWorld()->GetDeltaSeconds(), this->movementSpeed);
	this->SetActorLocation(newPosition);
}

bool ALR_Enemy::HasPathBlock(ELRPlayerMovementDirection direction) {
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



// =================================================
// Metodos de Ataque do Inimigo:
// =================================================
void ALR_Enemy::AttackUp() {}

void ALR_Enemy::AttackDown() {}

void ALR_Enemy::AttackRight() {}

void ALR_Enemy::AttackLeft() {}

bool ALR_Enemy::ActiveTargetIsInAttackRange() {
	auto lineStart = this->GetActorLocation();
	auto lineEndUp = lineStart + (this->GetActorForwardVector() * this->movementSize);
	auto lineEndDown = lineStart + (this->GetActorForwardVector() * -1 * this->movementSize);
	auto lineEndRight = lineStart + (this->GetActorRightVector() * this->movementSize);
	auto lineEndLeft = lineStart + (this->GetActorRightVector() * -1 * this->movementSize);

	FHitResult hitResultUp;
	FHitResult hitResultDown;
	FHitResult hitResultRight;
	FHitResult hitResultLeft;
	
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);

	this->GetWorld()->LineTraceSingleByChannel(hitResultUp, lineStart, lineEndUp, ECC_Visibility, params);
	this->GetWorld()->LineTraceSingleByChannel(hitResultUp, lineStart, lineEndDown, ECC_Visibility, params);
	this->GetWorld()->LineTraceSingleByChannel(hitResultUp, lineStart, lineEndRight, ECC_Visibility, params);
	this->GetWorld()->LineTraceSingleByChannel(hitResultUp, lineStart, lineEndLeft, ECC_Visibility, params);

	DrawDebugLine(this->GetWorld(), lineStart, lineEndUp, hitResultUp.IsValidBlockingHit() ? FColor::Green : FColor::Cyan, false, 10);
	DrawDebugLine(this->GetWorld(), lineStart, lineEndDown, hitResultDown.IsValidBlockingHit() ? FColor::Green : FColor::Cyan, false, 10);
	DrawDebugLine(this->GetWorld(), lineStart, lineEndRight, hitResultRight.IsValidBlockingHit() ? FColor::Green : FColor::Cyan, false, 10);
	DrawDebugLine(this->GetWorld(), lineStart, lineEndLeft, hitResultLeft.IsValidBlockingHit() ? FColor::Green : FColor::Cyan, false, 10);

	if (!hitResultUp.IsValidBlockingHit() && !hitResultDown.IsValidBlockingHit() && !hitResultRight.IsValidBlockingHit() && !hitResultLeft.IsValidBlockingHit()) {
		return false;
	}
	
	if (hitResultUp.IsValidBlockingHit()) {
		auto player = Cast<ALR_PlayerCharacter>(hitResultUp.GetActor());
		if (IsValid(player)) {
			this->nextAttackDirection = ELRPlayerAttackDirection::ATTACK_UP;
			return true;
		}
	}

	if (hitResultDown.IsValidBlockingHit()) {
		auto player = Cast<ALR_PlayerCharacter>(hitResultDown.GetActor());
		if (IsValid(player)) {
			this->nextAttackDirection = ELRPlayerAttackDirection::ATTACK_DOWN;
			return true;
		}
	}
	
	if (hitResultRight.IsValidBlockingHit()) {
		auto player = Cast<ALR_PlayerCharacter>(hitResultRight.GetActor());
		if (IsValid(player)) {
			this->nextAttackDirection = ELRPlayerAttackDirection::ATTACK_RIGHT;
			return true;
		}
	}

	if (hitResultLeft.IsValidBlockingHit()) {
		auto player = Cast<ALR_PlayerCharacter>(hitResultUp.GetActor());
		if (IsValid(player)) {
			this->nextAttackDirection = ELRPlayerAttackDirection::ATTACK_LEFT;
			return true;
		}
	}
	
	return false;
}



// =================================================
// Event Listeners:
// =================================================
void ALR_Enemy::RespondToPlayerAction() {
	// caso tenho um target ativo, a logica é bem mais complicada. Eu vou ter que calcular o caminho mais simples.
	// mas vou usar só um Breadth First Search simples que resolve o problema. -Renan
	if (this->activeTarget) {
		if (this->ActiveTargetIsInAttackRange()) {
			// this->StartAttackSequence();
			ULR_Utils::ShowDebugMessage(TEXT("Attacking Player"));
			return;
		}
		
		FVector enemyPosition = GetActorLocation();
		FVector playerPosition = activeTarget->GetActorLocation();

		// Normalizando para simular um grid sem grid
		int enemyX = FMath::RoundToInt(enemyPosition.X / this->movementSize);
		int enemyY = FMath::RoundToInt(enemyPosition.Y / this->movementSize);

		ULR_Utils::ShowDebugMessage(FString::Printf(TEXT("Enemy Position X: %d Enemy Position Y: %d"), enemyX, enemyY));
		
		int playerX = FMath::RoundToInt(playerPosition.X / this->movementSize);
		int playerY = FMath::RoundToInt(playerPosition.Y / this->movementSize);

		ULR_Utils::ShowDebugMessage(FString::Printf(TEXT("Player Position X: %d Player Position Y: %d"), playerX, playerY));

		// Esse delta serve pra ver se a direção em X é mais curta que a direção em Y
		// se o delta for maior na direção X então a direção Y é que vai ser priorizada
		// independente do caso
		int deltaX = playerX - enemyX;
		int deltaY = playerY - enemyY;

		ULR_Utils::ShowDebugMessage(FString::Printf(TEXT("Delta X: %d Delta Y: %d"), deltaX, deltaY));

		if (FMath::Abs(deltaX) > FMath::Abs(deltaY)) {
			if (deltaX > 0) this->MoveUp(); else this->MoveDown();
		} else {
			if (deltaY > 0) this->MoveRight(); else this->MoveLeft();
		}
		
		return;
	}
	
	auto randomDirection = FMath::RandRange(0, 5);
	if (randomDirection == 1) this->MoveUp();
	if (randomDirection == 2) this->MoveDown();
	if (randomDirection == 3) this->MoveLeft();
	if (randomDirection == 4) this->MoveRight();
	if (randomDirection == 0 || randomDirection == 5) return;
}

void ALR_Enemy::CheckForTarget(AActor* otherActor) {
	ALR_PlayerCharacter* player = Cast<ALR_PlayerCharacter>(otherActor);

	if (IsValid(player)) {
		this->activeTarget = player;
	}
}



// =================================================
// Metodo de Configuração:
// =================================================
void ALR_Enemy::Configure() {
	if (this->enemyConfig) {
		this->flipbookComponent->SetFlipbook(this->enemyConfig->enemyFlipbook);
	}
}

void ALR_Enemy::SetupEnemyBasedOnSelectedCharacter(ULR_GameInstance* gameInstance) {
	check(gameInstance);
	check(gameInstance->gameSelectedCharacter);
}