// Fill out your copyright notice in the Description page of Project Settings.


#include "STUProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ASTUProjectile::ASTUProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(FName("SphereCollision"));
	CollisionComponent->InitSphereRadius(5.0f);
	SetRootComponent(CollisionComponent);
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");

}

// Called when the game starts or when spawned
void ASTUProjectile::BeginPlay()
{
	Super::BeginPlay();
	check(MovementComponent);
	MovementComponent->InitialSpeed = 2000;
	MovementComponent->ProjectileGravityScale = 0;//Remove Gravity
	MovementComponent->Velocity = ShotDirection * MovementComponent->InitialSpeed;
	SetLifeSpan(4.0f);
}


