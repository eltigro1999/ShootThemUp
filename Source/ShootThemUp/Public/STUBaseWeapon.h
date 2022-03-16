// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUBaseWeapon.generated.h"

class USkeletalMeshComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASTUBaseWeapon();
	virtual void StartFire();
	virtual void StopFire();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
		USkeletalMeshComponent* WeaponMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FName MuzzleSocketName="MuzzleSocket";

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float TraceMaxDistance = 1500;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		float Damage = 10.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float TimeBetweenShots = 0.1f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float BulletSpread = 1.5f;

	void MakeShot();

	APlayerController* GetPlayerController();
	bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation);
	FVector GetMuzzleWorldLocation()const;
	bool GetTraceData(FVector& TraceStart, FVector& TraceEnd);
	void MakeHit(const FVector& TraceStart, const FVector& TraceEnd, FHitResult& HitResult);
	void MakeDamage(const FHitResult& HitResult);
private:
	FTimerHandle ShotTimerHandle;
};