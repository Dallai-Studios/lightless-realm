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
#include "Components/TimelineComponent.h"
#include "Tools/LR_Utils.h"
#include "Camera/CameraShakeBase.h"
#include "Components/LR_EntityAttributesComponent.h"
#include "Components/LR_EntityTextInfoComponent.h"
#include "Components/LR_FootstepAudioComponent.h"



// =================================================
// Metodos de Life Cycle:
// =================================================
ALR_PlayerCharacter::ALR_PlayerCharacter() {
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

	this->attackTimelineComponent = this->CreateDefaultSubobject<UTimelineComponent>("Attack Animation Timeline");

	this->footstepComponent = this->CreateDefaultSubobject<ULR_FootstepAudioComponent>("Footstep Audio Component");

	this->textInfoComponent = this->CreateDefaultSubobject<ULR_EntityTextInfoComponent>("Entity Text Info Component");

	this->attributesComponent = this->CreateDefaultSubobject<ULR_EntityAttributesComponent>("Attributes Component");
}

void ALR_PlayerCharacter::BeginPlay() {
	Super::BeginPlay();

	ULR_GameInstance* gameInstance = CastChecked<ULR_GameInstance>(this->GetWorld()->GetGameInstance());

	if (IsValid(gameInstance->gameSelectedCharacter)) {
		this->selectedCharacter = gameInstance->gameSelectedCharacter;
	} 

	this->ConfigureCharacter();
	
	this->targetLocation = this->GetActorLocation();
}

void ALR_PlayerCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	this->MoveTowardsTargetLocation();
}



// =================================================
// Metodos de Movimento do Player:
// =================================================
void ALR_PlayerCharacter::MovePlayer(ELRPlayerMovementDirection movementDirection) {
	if (!this->playerCanReceiveMovementInput) return;

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
	
	if (this->HasPathBlock(movementDirection)) return;

	this->playerCanReceiveMovementInput = false;
	this->playerCanReceiveAttackInput = false;

	this->StartMovementCoolDown();
	
	if (movementDirection == ELRPlayerMovementDirection::DIRECTION_UP) {
		this->targetLocation = this->GetActorLocation() + (this->GetActorForwardVector() * this->movementSize);
		return;
	}

	if (movementDirection == ELRPlayerMovementDirection::DIRECTION_DOWN) {
		this->targetLocation = this->GetActorLocation() + (this->GetActorForwardVector() * this->movementSize * -1);
		return;
	}

	if (movementDirection == ELRPlayerMovementDirection::DIRECTION_RIGHT) {
		this->targetLocation = this->GetActorLocation() + (this->GetActorRightVector() * this->movementSize);
		return;
	}

	if (movementDirection == ELRPlayerMovementDirection::DIRECTION_LEFT) {
		this->targetLocation = this->GetActorLocation() + (this->GetActorRightVector() * this->movementSize * -1);
		return;
	}
}

void ALR_PlayerCharacter::MoveTowardsTargetLocation() {
	auto currentPosition = this->GetActorLocation();

	if (FVector::Dist(currentPosition, this->targetLocation) < 1.0f) {
		this->SetActorLocation(this->targetLocation);
		return;
	}

	auto newPosition = FMath::VInterpConstantTo(currentPosition, this->targetLocation, this->GetWorld()->GetDeltaSeconds(), this->movementSpeed);
	this->SetActorLocation(newPosition);
}

bool ALR_PlayerCharacter::HasPathBlock(ELRPlayerMovementDirection direction) {
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

void ALR_PlayerCharacter::StartMovementCoolDown() {
	this->GetWorld()->GetTimerManager().SetTimer(
		this->movementCoolDownTimeHandle,
		this,
		&ALR_PlayerCharacter::HandleMovementEffects,
		this->movementDelayTime
	);
}

void ALR_PlayerCharacter::HandleMovementEffects() {
	this->GetWorld()->GetTimerManager().ClearTimer(this->movementCoolDownTimeHandle);

	this->playerCanReceiveMovementInput = true;
	this->playerCanReceiveAttackInput = true;

	this->footstepComponent->PlayFootstepSoundAtActorLocation();
	
	if (IsValid(this->gameEvents)) this->gameEvents->OnPlayerPerformAction.Broadcast(this->GetActorLocation());
}



// =================================================
// Metodos de Ataque do Player:
// =================================================
void ALR_PlayerCharacter::Attack(ELRPlayerAttackDirection attackDirection) {
	if (!this->playerCanReceiveAttackInput) return;

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
	
	this->playerCanReceiveAttackInput = false;
	this->playerCanReceiveMovementInput = false;

	this->AnimateAttack(attackDirection);

	this->StartAttackCooldown();

	if (this->baseAttackCameraShake) {
		this->GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(this->baseAttackCameraShake);
	}
}

void ALR_PlayerCharacter::AnimateAttack(ELRPlayerAttackDirection attackDirection) {
	FOnTimelineVector timelineCallback;
	timelineCallback.BindUFunction(this, FName("UpdateAttackAnimation"));

	FOnTimelineEvent timelineFinishCallback;
	timelineFinishCallback.BindUFunction(this, FName("FinishAttackAnimation"));

	this->attackTimelineComponent->SetTimelineFinishedFunc(timelineFinishCallback);

	switch (attackDirection) {
		case ELRPlayerAttackDirection::ATTACK_UP: this->attackTimelineComponent->AddInterpVector(this->attackUpAnimationCurve, timelineCallback); break;
		case ELRPlayerAttackDirection::ATTACK_DOWN: this->attackTimelineComponent->AddInterpVector(this->attackDownAnimationCurve, timelineCallback); break;
		case ELRPlayerAttackDirection::ATTACK_LEFT: this->attackTimelineComponent->AddInterpVector(this->attackLeftAnimationCurve, timelineCallback); break;
		case ELRPlayerAttackDirection::ATTACK_RIGHT: this->attackTimelineComponent->AddInterpVector(this->attackRightAnimationCurve, timelineCallback); break;
	}
	
	this->attackTimelineComponent->PlayFromStart();
}

void ALR_PlayerCharacter::UpdateAttackAnimation(FVector vectorValue) {
	this->flipbookComponent->SetRelativeLocation(vectorValue);
}

void ALR_PlayerCharacter::FinishAttackAnimation() {
	this->flipbookComponent->SetRelativeLocation(FVector(0, 0 , 0));
	if (IsValid(this->gameEvents)) this->gameEvents->OnPlayerPerformAction.Broadcast(FVector::Zero());
}

void ALR_PlayerCharacter::StartAttackCooldown() {
	this->GetWorld()->GetTimerManager().SetTimer(
		this->attackCoolDownTimerHandle,
		this,
		&ALR_PlayerCharacter::HandleAttackCoolOff,
		this->attackCooldownTime
	);
}

void ALR_PlayerCharacter::HandleAttackCoolOff() {
	this->playerCanReceiveAttackInput = true;
	this->playerCanReceiveMovementInput = true;
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



// =================================================
// Metodos de Configuração do Player:
// =================================================
void ALR_PlayerCharacter::ConfigureCharacter() {
	check(this->selectedCharacter);

	this->attributesComponent->BuildAttributesFromCharacter(this->selectedCharacter);
	
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