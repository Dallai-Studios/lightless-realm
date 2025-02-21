#include "Actors/LR_Enemy.h"
#include "PaperFlipbookComponent.h"
#include "Characters/LR_PlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/LR_FootstepAudioComponent.h"
#include "Components/SphereComponent.h"
#include "Components/TimelineComponent.h"
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

	this->playerDetectionBox = this->CreateDefaultSubobject<UBoxComponent>("Player Detection Box");
	this->playerDetectionBox->SetupAttachment(this->RootComponent);
	
	if (this->enemyConfig != nullptr) {
		this->flipbookComponent->SetFlipbook(this->enemyConfig->enemyFlipbook);
	}

	this->attackTimelineComponent = this->CreateDefaultSubobject<UTimelineComponent>("Attack Timeline Component");

	this->footstepComponent = this->CreateDefaultSubobject<ULR_FootstepAudioComponent>("Footstep Audio Component");
}

void ALR_Enemy::BeginPlay() {
	Super::BeginPlay();

	// essa configuração serve só pra ler todas as propriedades do data asset que representa esse inimigo
	// nenhuma configuração é feita para validar o personagem selecionado pelo jogador. -Renan
	this->Configure();
	this->SetupEnemyBasedOnSelectedCharacter();
	
	if (this->gameEvents) this->gameEvents->OnPlayerPerformAction.AddDynamic(this, &ALR_Enemy::RespondToPlayerAction);

	this->targetLocation = this->GetActorLocation();
}

void ALR_Enemy::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	this->MoveTowardsTargetLocation();

	if (IsValid(this->activeTarget)) {
		DrawDebugLine(this->GetWorld(), this->GetActorLocation(), this->activeTarget->GetActorLocation(), FColor::Orange);
	}
}



// =================================================
// Metodos de Movemento do Inimigo:
// =================================================
void ALR_Enemy::MoveEnemy(ELRPlayerMovementDirection movementDirection, FVector playerNextLocation) {
	if (this->canOnlyMoveWithActiveTarget && !this->activeTarget) return;
	
	if (this->HasPathBlock(movementDirection)) return;
	
	if (movementDirection == ELRPlayerMovementDirection::DIRECTION_RIGHT) {
		FVector flippedScale = this->flipbookComponent->GetRelativeScale3D();
		if (flippedScale.X > 0) flippedScale.X *= -1;
		this->flipbookComponent->SetRelativeScale3D(flippedScale);
	}
	
	if (movementDirection == ELRPlayerMovementDirection::DIRECTION_LEFT) {
		FVector flippedScale = this->flipbookComponent->GetRelativeScale3D();
		if (flippedScale.X < 0) flippedScale.X *= -1;
		this->flipbookComponent->SetRelativeScale3D(flippedScale);
	}

	// Adiciona um pequeno delay pra tocar o som dos passos do inimigo no final da animação de movimento. O rate time ta hardcoded mesmo por enquanto.
	this->GetWorld()->GetTimerManager().SetTimer(this->movementDelayTimer, this, &ALR_Enemy::PlayFootstepSound, 0.1);
	
	if (movementDirection == ELRPlayerMovementDirection::DIRECTION_UP) {
		FVector calculatedNextLocation = this->GetActorLocation() + (this->GetActorForwardVector() * this->movementSize);
		if (calculatedNextLocation == playerNextLocation) return;
		this->targetLocation = calculatedNextLocation;
		return;
	}

	if (movementDirection == ELRPlayerMovementDirection::DIRECTION_DOWN) {
		FVector calculatedNextLocation = this->GetActorLocation() + (this->GetActorForwardVector() * this->movementSize * -1);
		if (calculatedNextLocation == playerNextLocation) return;
		this->targetLocation = calculatedNextLocation;
		return;
	}

	if (movementDirection == ELRPlayerMovementDirection::DIRECTION_RIGHT) {
		FVector calculatedNextLocation = this->GetActorLocation() + (this->GetActorRightVector() * this->movementSize);
		if (calculatedNextLocation == playerNextLocation) return;
		this->targetLocation = calculatedNextLocation;
		return;
	}

	if (movementDirection == ELRPlayerMovementDirection::DIRECTION_LEFT) {
		FVector calculatedNextLocation = this->GetActorLocation() + (this->GetActorRightVector() * this->movementSize * -1);
		if (calculatedNextLocation == playerNextLocation) return;
		this->targetLocation = calculatedNextLocation;
		return;
	}
}

void ALR_Enemy::PlayFootstepSound() {
	this->footstepComponent->PlayFootstepSoundAtActorLocation();
	this->GetWorld()->GetTimerManager().ClearTimer(this->movementDelayTimer);
}

void ALR_Enemy::MoveTowardsTargetLocation() {
	auto currentPosition = this->GetActorLocation();

	if (FVector::Dist(currentPosition, this->targetLocation) < 1.0f) {
		this->SetActorLocation(this->targetLocation);
		return;
	}

	auto newPosition = FMath::VInterpConstantTo(currentPosition, this->targetLocation, this->GetWorld()->GetDeltaSeconds(), this->movementSpeed);
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
void ALR_Enemy::Attack(ELRPlayerAttackDirection attackDirection) {
	if (attackDirection == ELRPlayerAttackDirection::ATTACK_RIGHT) {
		FVector flippedScale = this->flipbookComponent->GetRelativeScale3D();
		if (flippedScale.X > 0) flippedScale.X *= -1;
		this->flipbookComponent->SetRelativeScale3D(flippedScale);
	}
	
	if (attackDirection == ELRPlayerAttackDirection::ATTACK_LEFT) {
		FVector flippedScale = this->flipbookComponent->GetRelativeScale3D();
		if (flippedScale.X < 0) flippedScale.X *= -1;
		this->flipbookComponent->SetRelativeScale3D(flippedScale);
	}

	this->AnimateAttack(attackDirection);
}

void ALR_Enemy::AnimateAttack(ELRPlayerAttackDirection attackDirection) {
	FOnTimelineVector timelineCallback;
	timelineCallback.BindUFunction(this, FName("UpdateAttackAnimation"));

	FOnTimelineEvent timelineFinishCallback;
	timelineFinishCallback.BindUFunction(this, FName("FinishAttackAnimation"));

	this->attackTimelineComponent->SetTimelineFinishedFunc(timelineFinishCallback);
	
	if (attackDirection == ELRPlayerAttackDirection::ATTACK_UP) {
		this->attackTimelineComponent->AddInterpVector(this->attackUpAnimationCurve, timelineCallback);
		this->attackTimelineComponent->PlayFromStart();
		return;
	}

	if (attackDirection == ELRPlayerAttackDirection::ATTACK_DOWN) {
		this->attackTimelineComponent->AddInterpVector(this->attackDownAnimationCurve, timelineCallback);
		this->attackTimelineComponent->PlayFromStart();
		return;
	}

	if (attackDirection == ELRPlayerAttackDirection::ATTACK_RIGHT) {
		this->attackTimelineComponent->AddInterpVector(this->attackRightAnimationCurve, timelineCallback);
		this->attackTimelineComponent->PlayFromStart();
		return;
	}

	if (attackDirection == ELRPlayerAttackDirection::ATTACK_LEFT) {
		this->attackTimelineComponent->AddInterpVector(this->attackLeftAnimationCurve, timelineCallback);
		this->attackTimelineComponent->PlayFromStart();
		return;
	}
}

void ALR_Enemy::UpdateAttackAnimation(FVector vectorValue) {
	this->flipbookComponent->SetRelativeLocation(vectorValue);
}

void ALR_Enemy::FinishAttackAnimation() {
	this->flipbookComponent->SetRelativeLocation(FVector(0, 0 , 0));
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

    // Corrigido: Cada Line Trace agora usa seu respectivo FHitResult
    this->GetWorld()->LineTraceSingleByChannel(hitResultUp, lineStart, lineEndUp, ECC_Pawn, params);
    this->GetWorld()->LineTraceSingleByChannel(hitResultDown, lineStart, lineEndDown, ECC_Pawn, params);
    this->GetWorld()->LineTraceSingleByChannel(hitResultRight, lineStart, lineEndRight, ECC_Pawn, params);
    this->GetWorld()->LineTraceSingleByChannel(hitResultLeft, lineStart, lineEndLeft, ECC_Pawn, params);

    // Desenho das linhas de debug
    DrawDebugLine(this->GetWorld(), lineStart, lineEndUp, hitResultUp.bBlockingHit ? FColor::Green : FColor::Cyan, false, 10);
    DrawDebugLine(this->GetWorld(), lineStart, lineEndDown, hitResultDown.bBlockingHit ? FColor::Green : FColor::Cyan, false, 10);
    DrawDebugLine(this->GetWorld(), lineStart, lineEndRight, hitResultRight.bBlockingHit ? FColor::Green : FColor::Cyan, false, 10);
    DrawDebugLine(this->GetWorld(), lineStart, lineEndLeft, hitResultLeft.bBlockingHit ? FColor::Green : FColor::Cyan, false, 10);

    // Verificação para cada direção
    if (hitResultUp.bBlockingHit) {
        auto player = Cast<ALR_PlayerCharacter>(hitResultUp.GetActor());
        if (IsValid(player)) {
            this->nextAttackDirection = ELRPlayerAttackDirection::ATTACK_UP;
            return true;
        }
    }

    if (hitResultDown.bBlockingHit) {
        auto player = Cast<ALR_PlayerCharacter>(hitResultDown.GetActor());
        if (IsValid(player)) {
            this->nextAttackDirection = ELRPlayerAttackDirection::ATTACK_DOWN;
            return true;
        }
    }
    
    if (hitResultRight.bBlockingHit) {
        auto player = Cast<ALR_PlayerCharacter>(hitResultRight.GetActor());
        if (IsValid(player)) {
            this->nextAttackDirection = ELRPlayerAttackDirection::ATTACK_RIGHT;
            return true;
        }
    }

    if (hitResultLeft.bBlockingHit) {
        // Corrigido: Usando hitResultLeft corretamente
        auto player = Cast<ALR_PlayerCharacter>(hitResultLeft.GetActor());
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
void ALR_Enemy::RespondToPlayerAction(FVector playerNextLocation) {
	if (this->activeTarget) {
		if (this->ActiveTargetIsInAttackRange()) {
			this->Attack(this->nextAttackDirection);
			ULR_Utils::ShowDebugMessage(TEXT("Attacking Player"));
			return;
		}
		
		FVector enemyPosition = GetActorLocation();
		FVector playerPosition = activeTarget->GetActorLocation();

		// Normalizando para simular um grid sem grid
		int enemyX = FMath::RoundToInt(enemyPosition.X / this->movementSize);
		int enemyY = FMath::RoundToInt(enemyPosition.Y / this->movementSize);
		
		int playerX = FMath::RoundToInt(playerPosition.X / this->movementSize);
		int playerY = FMath::RoundToInt(playerPosition.Y / this->movementSize);
		
		// Esse delta serve pra ver se a direção em X é mais curta que a direção em Y
		// se o delta for maior na direção X então a direção Y é que vai ser priorizada
		// independente do caso
		int deltaX = playerX - enemyX;
		int deltaY = playerY - enemyY;
		
		if (FMath::Abs(deltaX) > FMath::Abs(deltaY)) {
			if (deltaX > 0) this->MoveEnemy(ELRPlayerMovementDirection::DIRECTION_UP, playerNextLocation);
			else this->MoveEnemy(ELRPlayerMovementDirection::DIRECTION_DOWN, playerNextLocation);
		} else {
			if (deltaY > 0) this->MoveEnemy(ELRPlayerMovementDirection::DIRECTION_RIGHT, playerNextLocation);
			else this->MoveEnemy(ELRPlayerMovementDirection::DIRECTION_LEFT, playerNextLocation);
		}
		
		return;
	}
	
	int32 randomDirection = FMath::RandRange(0, 5);
	if (randomDirection == 1) this->MoveEnemy(ELRPlayerMovementDirection::DIRECTION_UP);
	if (randomDirection == 2) this->MoveEnemy(ELRPlayerMovementDirection::DIRECTION_DOWN);
	if (randomDirection == 3) this->MoveEnemy(ELRPlayerMovementDirection::DIRECTION_LEFT);
	if (randomDirection == 4) this->MoveEnemy(ELRPlayerMovementDirection::DIRECTION_RIGHT);
	if (randomDirection == 0 || randomDirection == 5) return;
}

void ALR_Enemy::CheckForTarget(AActor* otherActor) {
	if (this->isPassive) return;
	
	ALR_PlayerCharacter* player = CastChecked<ALR_PlayerCharacter>(otherActor);

	if (IsValid(player)) {
		this->activeTarget = player;
	}
}



// =================================================
// Metodo de Configuração:
// =================================================
void ALR_Enemy::Configure() {
	if (!this->enemyConfig) {
		ULR_Utils::ShowDebugMessage("No Enemy config provded for all the enemies on the scene");
		return;
	}

	this->flipbookComponent->SetFlipbook(this->enemyConfig->enemyFlipbook);
	
	if (this->enemyConfig->isSmall) this->flipbookComponent->SetRelativeScale3D(FVector(1, 1, 1));
	if (this->enemyConfig->isBoss) this->flipbookComponent->SetRelativeScale3D(FVector(3, 3, 3));
}

void ALR_Enemy::SetupEnemyBasedOnSelectedCharacter() {
	ULR_GameInstance* currentGameInstance = CastChecked<ULR_GameInstance>(this->GetWorld()->GetGameInstance());

	if (!currentGameInstance->gameSelectedCharacter) {
		ULR_Utils::ShowDebugMessage(TEXT("Selected player character is not define on the gameinstance"));
		return;
	}

	this->playerDetectionBox->SetBoxExtent(FVector(this->detectionSize, this->detectionSize, 0.f));
	
	if (currentGameInstance->gameSelectedCharacter->easyToDetect) {
		this->playerDetectionBox->SetBoxExtent(FVector(this->detectionSize * 2, this->detectionSize * 2, 0.f));
	}

	if (currentGameInstance->gameSelectedCharacter->hardToDetectOnShadows) {
		this->playerDetectionBox->SetBoxExtent(FVector(this->detectionSize / 2, this->detectionSize / 2, 0.f));
	}

	if (currentGameInstance->gameSelectedCharacter->animalsArePassive && this->enemyConfig->enemyType == ELREnemyType::ANIMAL) {
		this->isPassive = true;
	}
}