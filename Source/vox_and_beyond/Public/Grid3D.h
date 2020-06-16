// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "basePrimitive.h"
#include "UObject/ConstructorHelpers.h"
#include "Grid3D.generated.h"

  //auto bounds = m_pMesh->CalcBounds(m_pMesh->GetRelativeTransform());
UCLASS(Blueprintable)
class VOX_AND_BEYOND_API AGrid3D : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrid3D();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	UFUNCTION(BlueprintCallable)
  bool 
  addPrimitiveToGrid(AbasePrimitive* primitive);


	UFUNCTION(BlueprintCallable)
  AbasePrimitive*
	GetPrimitive(int64 index);

	UFUNCTION(BlueprintCallable)
  void
  TransFormPrimitive(int64 index);

  // TODO : ADD COLOR PARAM
	UFUNCTION(BlueprintCallable)
  AbasePrimitive*
  SpwanInGrid(FVector point);

private:

  void//between
  calculateSizeBetweenCubes();

public:
  /**
  * @brief controls how big the grid is in the X-Axis.
  */
	UPROPERTY(VisibleAnywhere)
  int32 m_width;

  /**
  * @brief controls how big the grid is in the Z-Axis.(because this is unreal engine)
  */
	UPROPERTY(VisibleAnywhere)
	int32 m_height; 

  /**
  * @brief controls how big the grid is in the Y-Axis.(because this is unreal engine)
  */
	UPROPERTY(VisibleAnywhere)
	int32 m_depth;

private:
  /**
  * @brief The difference Between 2 elements in the X-Axis.
  */
  float m_deltaWidth;

  /**
  * @brief The difference Between 2 elements in the Z-Axis.(because this is unreal engine)
  */
  float m_deltaHeight;

  /**
  * @brief The difference Between 2 elements in the Y-Axis.(because this is unreal engine)
  */
  float m_deltaDepth;

  /**
  * @brief for visualizing the mesh.
  */
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent *m_pMesh;

  static ConstructorHelpers::FObjectFinder<UStaticMesh>* s_pointerToMesh;

  /**
  * @brief keeps track of how many primitive the grid has.
  */
  int64 m_count;

  /**
  * @brief restricts how many primitive the grid can have.
  */
  int64 m_primitiveLimitAmount;


public:

	UPROPERTY(EditAnywhere)
	FVector m_topLeftPosition;

	UPROPERTY(EditAnywhere)
  FVector m_buttomRightPostion;

  UPROPERTY(VisibleAnywhere)
  TArray<AbasePrimitive*> m_primitives;
};