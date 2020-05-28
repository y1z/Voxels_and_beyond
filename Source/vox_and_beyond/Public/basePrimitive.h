// Fill out your copyright notice in the Description page of Project Settings.  
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <memory>

#include "basePrimitive.generated.h"

UENUM(BlueprintType)
enum class PrimitiveShape : uint8
{
	cube UMETA(DisplayName = "cube shape"),
	sphere UMETA(DisplayName = "sphere shape"),

};

class PossibleShapes
{
public:
  std::unique_ptr<UStaticMesh> m_cubeMesh = nullptr;
  std::unique_ptr<UStaticMesh> m_sphereMesh = nullptr;
  std::unique_ptr<UStaticMesh> m_pyramidMesh = nullptr;
  
};

UCLASS(Blueprintable)
class VOX_AND_BEYOND_API AbasePrimitive : public AActor
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AbasePrimitive();
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent *m_pMesh;

	UFUNCTION(BlueprintCallable)
	void changeShape(PrimitiveShape shape);


	UFUNCTION(BlueprintCallable)
	bool tempChangeShape(int shape);

protected:
	UPROPERTY(VisibleAnywhere)
	PrimitiveShape m_selectedShape;

public:
	UPROPERTY(EditAnywhere)
	FColor m_color;

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
