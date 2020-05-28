// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/UserPawn.h"
#include "../Public/basePrimitive.h"
#include <functional>

// Sets default values
AUserPawn::AUserPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AUserPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUserPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AUserPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

