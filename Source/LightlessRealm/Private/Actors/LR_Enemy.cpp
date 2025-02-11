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


// lifecycles:
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
	this->MoveEnemy(DeltaTime);

	if (IsValid(this->activeTarget)) {
		DrawDebugLine(this->GetWorld(), this->GetActorLocation(), this->activeTarget->GetActorLocation(), FColor::Orange);
	}
}


void ALR_Enemy::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	this->Destroy();
}



// common:
void ALR_Enemy::MoveUp() {
	if (this->canOnlyMoveWithActiveTarget && !this->activeTarget) return;
	if (this->CheckForPathBlock(ELRPlayerMovementDirection::DIRECTION_UP)) return;
	
	this->movementDirection = ELRPlayerMovementDirection::DIRECTION_UP;
	FVector currentLocation = this->GetActorLocation();
	this->destinationLocation = FVector(currentLocation.X + this->movementSize, currentLocation.Y, currentLocation.Z);
}


void ALR_Enemy::MoveDown() {
	if (this->canOnlyMoveWithActiveTarget && !this->activeTarget) return;
	if (this->CheckForPathBlock(ELRPlayerMovementDirection::DIRECTION_DOWN)) return;
	
	this->movementDirection = ELRPlayerMovementDirection::DIRECTION_DOWN;
	FVector currentLocation = this->GetActorLocation();
	this->destinationLocation = FVector(currentLocation.X + (this->movementSize * -1), currentLocation.Y, currentLocation.Z);
}


void ALR_Enemy::MoveLeft() {
	if (this->canOnlyMoveWithActiveTarget && !this->activeTarget) return;
	if (this->CheckForPathBlock(ELRPlayerMovementDirection::DIRECTION_LEFT)) return;
	
	this->movementDirection = ELRPlayerMovementDirection::DIRECTION_LEFT;
	FVector currentLocation = this->GetActorLocation();
	this->destinationLocation = FVector(currentLocation.X, currentLocation.Y + (this->movementSize * -1), currentLocation.Z);

	FVector flippedScale = this->flipbookComponent->GetRelativeScale3D();
	if (flippedScale.X < 0) flippedScale.X *= -1;
	this->flipbookComponent->SetRelativeScale3D(flippedScale);
}


void ALR_Enemy::MoveRight() {
	if (this->canOnlyMoveWithActiveTarget && !this->activeTarget) return;
	if (this->CheckForPathBlock(ELRPlayerMovementDirection::DIRECTION_RIGHT)) return;
	
	this->movementDirection = ELRPlayerMovementDirection::DIRECTION_RIGHT;
	FVector currentLocation = this->GetActorLocation();
	this->destinationLocation = FVector(currentLocation.X, currentLocation.Y + this->movementSize, currentLocation.Z);

	FVector flippedScale = this->flipbookComponent->GetRelativeScale3D();
	if (flippedScale.X > 0) flippedScale.X *= -1;
	this->flipbookComponent->SetRelativeScale3D(flippedScale);
}


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
		int enemyX = FMath::RoundToInt(enemyPosition.X) / this->movementSize;
		int enemyY = FMath::RoundToInt(enemyPosition.Y) / this->movementSize;

		ULR_Utils::ShowDebugMessage(FString::Printf(TEXT("Enemy Position X: %d Enemy Position Y: %d"), enemyX, enemyY));
		
		int playerX = FMath::RoundToInt(playerPosition.X) / this->movementSize;
		int playerY = FMath::RoundToInt(playerPosition.Y) / this->movementSize;

		ULR_Utils::ShowDebugMessage(FString::Printf(TEXT("Player Position X: %d Player Position Y: %d"), playerX, playerY));

		// Esse delta serve pra ver se a direção em X é mais curta que a direção em Y
		// se o delta for maior na direção X então a direção Y é que vai ser priorizada
		// independente do caso
		int deltaX = playerX - enemyX;
		int deltaY = playerY - enemyY;

		ULR_Utils::ShowDebugMessage(FString::Printf(TEXT("Delta X: %d Delta Y: %d"), deltaX, deltaY));

		if (FMath::Abs(deltaX) > FMath::Abs(deltaY)) {
			if (deltaX > 0) this->MoveRight(); else this->MoveLeft();
		} else {
			if (deltaY > 0) this->MoveUp(); else this->MoveDown();
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


void ALR_Enemy::MoveEnemy(float deltaTime) {
	FVector newLocation = FMath::LerpStable(this->GetActorLocation(), this->destinationLocation, deltaTime * this->movementSpeed);
	this->SetActorLocation(newLocation);
}


bool ALR_Enemy::CheckForPathBlock(ELRPlayerMovementDirection direction) {
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


void ALR_Enemy::Configure() {
	if (this->enemyConfig) {
		this->flipbookComponent->SetFlipbook(this->enemyConfig->enemyFlipbook);
	}
}


void ALR_Enemy::SetupEnemyBasedOnSelectedCharacter(ULR_GameInstance* gameInstance) {
	check(gameInstance);
	check(gameInstance->gameSelectedCharacter);
}


void ALR_Enemy::CheckForTarget(
	UPrimitiveComponent* overlapedComponent, 
	AActor* otherActor, 
	UPrimitiveComponent* otherComponent,
	int32 otherBodyIndex,
	bool fromSweep,
	const FHitResult& SweepResult
) {
	ALR_PlayerCharacter* player = Cast<ALR_PlayerCharacter>(otherActor);

	if (IsValid(player)) {
		this->activeTarget = player;
	}
}