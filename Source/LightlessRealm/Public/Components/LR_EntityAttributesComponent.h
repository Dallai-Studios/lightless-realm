
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LR_EntityAttributesComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttributeEntityEvent);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LIGHTLESSREALM_API ULR_EntityAttributesComponent : public UActorComponent {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attributes")
	float maxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attributes")
	float health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attributes")
	float maxMagika;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attributes")
	float magika;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attributes")
	bool isDead = false;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Events")
	FAttributeEntityEvent OnEntityDie;
	
	
public:
	ULR_EntityAttributesComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category="Attribute Functions")
	void BuildAttributesFromCharacter(class ULR_PlayerCharacterPDA* character);	

	UFUNCTION(BlueprintCallable, Category="Attribute Functions")
	void BuildAttributesFromEnemy(class ULR_EnemyPDA* enemy);
	
	UFUNCTION(BlueprintCallable, Category="Attribute Functions")
	void SetNewMaxHealthAmount(float maxHealthAmount);

	UFUNCTION(BlueprintCallable, Category="Attribute Functions")
	void AddHealth(float healthAmount);

	UFUNCTION(BlueprintCallable, Category="Attribute Functions")
	void RemoveHealth(float amount);

	UFUNCTION(BlueprintCallable, Category="Attribute Functions")
	void SetNewMaxMagikaAmount(float maxMagikaAmount);

	UFUNCTION(BlueprintCallable, Category="Attribute Functions")
	void AddMagika(float magikaAmount);

	UFUNCTION(BlueprintCallable, Category="Attribute Functions")
	bool HasEnoughMagikaToPerformAction(const float amount);	
	
	UFUNCTION(BlueprintCallable, Category="Attribute Functions")
	void RemoveMagika(float magikaAmount);
};
