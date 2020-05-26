// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <memory>




#include "basePrimitive.generated.h"

UCLASS(Blueprintable)
class VOX_AND_BEYOND_API AbasePrimitive : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AbasePrimitive();
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* m_pMesh;

protected:

  static FString s_cubeMeshPath;
  static FString s_sphereMeshPath;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
