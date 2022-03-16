// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUHealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeathSignature)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTTHEMUP_API USTUHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USTUHealthComponent();

	UFUNCTION(BlueprintCallable)
	float GetHealth() const { return Health; }

	UFUNCTION()
		void OnTakeAnyDamage(AActor* DamagedActor, float Damage,
			const class UDamageType* DamageType, class AController* InstigatedBy,
			AActor* DamageCauser);

	UFUNCTION(BlueprintCallable, Category="Health")
		bool IsDead() { return FMath::IsNearlyZero(Health); };

	FOnDeathSignature OnDeath;
	FOnHealthChangedSignature OnHealthChanged;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")
		bool AutoHeal = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta=(EditCondition="AutoHeal"))
		float HealUpdateTime=1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta=(EditCondition="AutoHeal"))
		float HealDelay=3.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta=(EditCondition="AutoHeal"))
		float HealModifier=5.0f;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxHealth=100.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (Clampmin="0.0", ClampMax="100.0"))
	float Health = 0.0f;
private:
	FTimerHandle HealTimerHandle;
	void HealUpdate();
	void SetHealth(float UpdateHealth);
};
