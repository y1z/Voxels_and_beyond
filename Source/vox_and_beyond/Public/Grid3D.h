// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/ConstructorHelpers.h"
#include "basePrimitive.h"

#include "Grid3D.generated.h"

struct GridDataElement
{
  FIntVector ID;
  FIntVector rotationAmount;
  FColor color;
  PrimitiveShape shape;
};

struct GridData
{
  GridData() = default;
  GridData(const TArray<GridDataElement>& elements)
    :m_element(elements) {};

  ~GridData() = default;

  TArray <GridDataElement> m_element;
};

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

  /**
  * @brief Get a pointer to a certain primitive.
  */
	UFUNCTION(BlueprintCallable)
  AbasePrimitive*
	GetPrimitive(int64 index);

  /**
  * @brief Expands the Grid.
  * @returns A vector that contains how big each axis is .
  * @bug no known bugs
  */
	UFUNCTION(BlueprintCallable)
  FIntVector 
  expandGrid(FIntVector directionOfExpansion);

  /**
  * @brief Resizes the grid.
  * @bug no known bugs
  */
	UFUNCTION(BlueprintCallable)
  void
  resizeGrid(FIntVector newGridSize);


  /**
  * @brief A Representation of how big the Grid is.
  * @returns A vector that contains how big each axis is .
  * @bug no known bugs
  */
	UFUNCTION(BlueprintCallable)
  FIntVector 
  getRelativeGridSizePerAxis()const;

  /**
  * @brief A representation of the size of each axis of the Grid.
  * @returns A vector that contains how big each axis is .
  * @bug no known bugs
  */
	UFUNCTION(BlueprintCallable)
  FVector 
  getAbsoluteGridSizePerAxis()const;

  /**
  * @returns The middle of the Grid in the xy axis.
  * @bug no known bugs
  */
	UFUNCTION(BlueprintCallable)
  FVector 
  getCenterRelativeToWorld()const;

  /**
  * @returns The middle of the Grid in the xy axis.
  * @bug no known bugs
  */
	UFUNCTION(BlueprintCallable)
  FVector 
  getCenterRelativeToItSelf()const;

  /**
  * @returns The middle of the Grid in the xy axis.
  * @bug no known bugs
  */
	UFUNCTION(BlueprintCallable)
  FVector 
  getCenterXY()const;

  /**
  * @returns The height of the grid.
  * @bug no known bugs
  */
	UFUNCTION(BlueprintCallable)
  float
  getHeight()const;


  /**
  * @returns The Width of the grid.
  * @bug no known bugs
  */
	UFUNCTION(BlueprintCallable)
  float
  getWidth()const;


  /**
  * @returns The Depth of the grid.
  * @bug no known bugs
  */
	UFUNCTION(BlueprintCallable)
  float
  getDepth()const;

  /**
  * @returns A point that indicates the top of the Grid.
  * @bug no known bugs
  */
	UFUNCTION(BlueprintCallable)
  FVector
  getTopPoint()const;

  /**
  * @returns A point that indicates the "left" of the Grid.
  * @bug no known bugs
  */
	UFUNCTION(BlueprintCallable)
  FVector
  getLeftPoint()const;

  /**
  * @returns A point that indicates the "Right" of the Grid.
  * @bug no known bugs
  */
	UFUNCTION(BlueprintCallable)
  FVector
  getRightPoint()const;

  /**
  * @returns A point that indicates the bottom of the Grid.
  * @bug no known bugs
  */
	UFUNCTION(BlueprintCallable)
  FVector
  getBottomPoint()const;

public:


	UFUNCTION(BlueprintCallable,Category = "saving and loading")
  bool
  loadDataFromFile(FString fileName);


  /**
  * @brief Uses the GridData to create a series of primitives.
  */
  void
  createGridFromData(const GridData &data);

private:

  /**
  * @brief Is where all the work happens in terms of building
  * the Grid
  */
  void
  buildGridFromData(const GridData &data);



  FVector
  calculatePositionInGrid(const FIntVector relativePosition) const;

  /**
  * @returns the default parameter for spawning a another actor.
  */
  FActorSpawnParameters
  spawnParameters();

  /**
  * @brief Calculates the delta between each Primitive in the Grid.
  * @bug no known bugs.
  */
  void
  calculateSizeBetweenCubes();

  /**
  * @brief updates all variables related with the grid size.
  */
  void
  updateGridSize();

  /**
  * @brief Calculates the 'forward' , 'up' and 'right' directions for the Grid.
  * @bug no known bugs.
  */
  void
  calculateDirectionsForGrid();

  /**
  * @brief Creates the floor for the 3D grid.
  * @note This should ONLY be called for the creation of a grid.
  * @bug no known bugs.
  */
  void
  createFloorForGrid();

  /**
  * @brief Remove the current grid floor.
  */
  void
  destroyGridFloor();

  /**
  * @brief Remove all primitives from the grid.
  */
  void
  destroyAllGrid();

public:
  /**
  * @brief Used to delete An element in the grid.
  */
	UFUNCTION(BlueprintCallable)
  bool 
  destroyPrimitive(AbasePrimitive *primitiveToDestroy);


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

  /**
  * @brief A pointer to a static mesh
  */
  static ConstructorHelpers::FObjectFinder<UStaticMesh>* s_cubeMesh;

  /**
  * @brief A pointer to a static mesh
  */
  static ConstructorHelpers::FObjectFinder<UStaticMesh>* s_planeMesh;

  /**
  * @brief keeps track of how many primitive the grid has.
  */
  int64 m_count;

  /**
  * @brief restricts how many primitive the grid can have.
  */
  int64 m_primitiveLimitAmount;


public:

  /**
  * @brief Where the top left position relative to the grid is located.
  */
  UPROPERTY(VisibleAnywhere)
  FVector m_topLeftPosition;

  /**
  * @brief Where the bottom right position relative to the grid is located.
  */
  UPROPERTY(VisibleAnywhere)
  FVector m_bottomRightPosition;

  /**
  * @brief Defines the 'forward' direction relative to the grid
  */
  UPROPERTY(VisibleAnywhere)
  FVector m_forwardDir;

  /**
  * @brief Defines the 'up' direction relative to the grid.
  */
  UPROPERTY(VisibleAnywhere)
  FVector m_upDir;


  /**
  * @brief Defines the 'right' direction relative to the grid.
  */
  UPROPERTY(VisibleAnywhere)
  FVector m_rightDir;

  /**
  * @brief Contains all the base primitives.
  */
  UPROPERTY(VisibleAnywhere)
  TArray<AbasePrimitive*> m_primitives;

};





