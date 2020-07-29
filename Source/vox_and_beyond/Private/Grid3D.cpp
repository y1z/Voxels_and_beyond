// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid3D.h"
#include "Utility.h"
#include <limits>//std::numeric_limits
#include <cmath>

ConstructorHelpers::FObjectFinder<UStaticMesh>* AGrid3D::s_cubeMesh = nullptr;

ConstructorHelpers::FObjectFinder<UStaticMesh>* AGrid3D::s_planeMesh = nullptr;


// Sets default values
AGrid3D::AGrid3D()
  :m_width(16),
  m_height(16),
  m_depth(16),
  m_deltaWidth(1000.0f),
  m_deltaHeight(1000.0f),
  m_deltaDepth(1000.0f),
  m_pMesh(nullptr),
  m_count(0),
  m_primitiveLimitAmount(int64(std::numeric_limits<int64>::max() / std::numeric_limits<int32>::max()))

{
  // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = false;
  m_pMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("3D Grid"));
  m_pMesh->SetupAttachment(RootComponent);

  updateGridSize();

  m_primitives.Reserve(m_width * m_height * m_depth);

  static ConstructorHelpers::FObjectFinder<UStaticMesh> Visual
  (TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));

  static ConstructorHelpers::FObjectFinder<UStaticMesh> Plane
  (TEXT("/Game/StarterContent/Shapes/Shape_Plane.Shape_Plane"));

  s_cubeMesh = &Visual;
  s_planeMesh = &Plane;

  if( s_cubeMesh->Succeeded() )
  {
    m_pMesh->SetStaticMesh(s_cubeMesh->Object);
    float const smallestValue = std::numeric_limits<float>::epsilon();
    m_pMesh->GetCollisionShape().SetBox(FVector(smallestValue, smallestValue, smallestValue));

    m_pMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
  }


}

// Called when the game starts or when spawned
void AGrid3D::BeginPlay()
{
  Super::BeginPlay();

  m_pMesh->SetWorldLocation(FVector(0, 0, 0));

  updateGridSize();

  this->calculateDirectionsForGrid();

  this->calculateSizeBetweenCubes();

  this->createFloorForGrid();

  m_pMesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

  m_pMesh->SetVisibility(false);
}

void AGrid3D::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);

}

bool
AGrid3D::addPrimitiveToGrid(AbasePrimitive* primitive)
{
  if( m_count < m_primitiveLimitAmount )
  {
    m_primitives.Add(primitive);
    m_primitives.Sort();
    ++m_count;
    return true;
  }

  return false;
}



AbasePrimitive*
AGrid3D::GetPrimitive(int64 index)
{
  if( index <= m_primitiveLimitAmount )
  {
    return m_primitives[index];
  }


  return nullptr;
}


FIntVector
AGrid3D::expandGrid(FIntVector directionOfExpansion)
{
  for( int32 i = 2; i > -1; --i )
  {
    int32 const valuePerAxis = directionOfExpansion(i);
    switch( i )
    {
      case 0:
      {
        m_width += valuePerAxis;
        m_topLeftPosition += m_rightDir * m_deltaWidth * valuePerAxis;
      }
      break;
      case 1:
      {
        m_depth += valuePerAxis;
        m_topLeftPosition += m_forwardDir * m_deltaDepth * valuePerAxis;
      }
      break;
      case 2:
      {
        m_height += valuePerAxis;
        m_topLeftPosition += m_upDir * m_deltaHeight * valuePerAxis;
      }
      break;
    }
  }

  this->calculateSizeBetweenCubes();
  this->createFloorForGrid();

  return FIntVector(m_width, m_depth, m_height);
}

void
AGrid3D::resizeGrid(FIntVector newGridSize)
{
  destroyGridFloor();

  m_width = newGridSize.X;
  m_depth = newGridSize.Y;
  m_height = newGridSize.Z;
  updateGridSize();

  this->calculateSizeBetweenCubes();


  createFloorForGrid();
}

FIntVector
AGrid3D::getGridSizePerAxis() const
{
  return FIntVector(m_width, m_depth, m_height);
}

bool
AGrid3D::loadDataFromFile(FString fileName)
{
  destroyAllGrid();
  return UUtility::loadGridData(this, fileName);
}

void
AGrid3D::createGridFromData(const GridData& data)
{
   // TODO : REMOVE 'createPrimitivesByRelativePosition' and 'colorPrimitives'
  buildGridFromData(data);

}

void
AGrid3D::buildGridFromData(const GridData& data)
{

  FActorSpawnParameters spawnParam = this->spawnParameters();
  UWorld* const worldPtr = GetWorld();
  for( auto& element : data.m_element )
  {
    const FVector locationInGrid = calculatePositionInGrid(element.ID);

    AbasePrimitive* primitiveToGoInGrid = worldPtr->SpawnActor<AbasePrimitive>(spawnParam);
    primitiveToGoInGrid->init(element.ID, locationInGrid);

    primitiveToGoInGrid->setColor(element.color);
    primitiveToGoInGrid->setShape(PrimitiveShape::cube);

    m_primitives.Add(primitiveToGoInGrid);
  }

  m_primitives.Sort();

}

FVector
AGrid3D::calculatePositionInGrid(const FIntVector relativePosition) const
{
  FVector const InGridPosition = FVector((m_deltaWidth * relativePosition.X),
                                         (m_deltaDepth * relativePosition.Y),
                                         m_deltaHeight * relativePosition.Z);
  return(this->m_bottomRightPosition + InGridPosition);
}

FActorSpawnParameters
AGrid3D::spawnParameters()
{
  FActorSpawnParameters spawnParam;
  spawnParam.Template = nullptr;
  spawnParam.Owner = this;
  return  spawnParam;
}


void
AGrid3D::calculateSizeBetweenCubes()
{
  m_deltaWidth = std::fabsf(m_topLeftPosition.X - m_bottomRightPosition.X) / std::max(1, m_width);
  m_deltaDepth = std::fabsf(m_topLeftPosition.Y - m_bottomRightPosition.Y) / std::max(1, m_depth);
  m_deltaHeight = std::fabsf(m_topLeftPosition.Z - m_bottomRightPosition.Z) / std::max(1, m_height);
}

void
AGrid3D::updateGridSize()
{
  m_pMesh->SetRelativeScale3D(FVector(m_width, m_depth, m_height));

  auto bounds = m_pMesh->CalcBounds(m_pMesh->GetRelativeTransform());
  m_topLeftPosition = bounds.GetBox().Max;
  m_bottomRightPosition = bounds.GetBox().Min;
}

void
AGrid3D::calculateDirectionsForGrid()
{
  float const deltaInZ = m_topLeftPosition.Z - m_bottomRightPosition.Z;
  float const deltaInY = m_topLeftPosition.Y - m_bottomRightPosition.Y;
  float const deltaInX = m_topLeftPosition.X - m_bottomRightPosition.X;
  m_upDir = FVector(0.0f, .0f, deltaInZ).GetSafeNormal();
  m_forwardDir = FVector(0.0f, deltaInY, .0f).GetSafeNormal();
  m_rightDir = FVector(deltaInX, .0f, .0f).GetSafeNormal();
}

void
AGrid3D::createFloorForGrid()
{
  float const lowestZ = m_bottomRightPosition.Z;

  FActorSpawnParameters spawnParam;
  spawnParam.Template = nullptr;
  spawnParam.Owner = this;
  for( int32 i = m_width; i > 0; --i )
  {

    for( int32 j = m_depth; j > 0; --j )
    {
      FVector const InGridPosition = FVector((m_deltaWidth * i), (m_deltaDepth * j), lowestZ);
      FVector const finalLocation = (this->m_bottomRightPosition + InGridPosition);

      FIntVector const ID = FIntVector(i, j, 0);
      auto compareIDs = [&ID](AbasePrimitive const* ptr) {
        return ID.X == ptr->getId().X &&
          ID.Y == ptr->getId().Y &&
          ID.Z == ptr->getId().Z;
      };

      bool const addPrimitive = (nullptr == m_primitives.FindByPredicate(compareIDs));
      if( addPrimitive )
      {
        AbasePrimitive* primitiveToGoInGrid = GetWorld()->SpawnActor<AbasePrimitive>(spawnParam);
        primitiveToGoInGrid->init(ID, finalLocation);

        primitiveToGoInGrid->setColor(FColor(224, 189, 150, 255));
        primitiveToGoInGrid->setShape(PrimitiveShape::cube);

        m_primitives.Add(primitiveToGoInGrid);
      }
    }
  }
  m_primitives.Sort();
}

void
AGrid3D::destroyGridFloor()
{
  UWorld* const worldPtr = GetWorld();

  auto const currentWidth = m_width;
  auto const currentDepth = m_depth;

  auto selectOnlyTheFloorPrimitives = [currentWidth, currentDepth](AbasePrimitive const* ptr) {
    return 0 == ptr->getId().Z ||
      currentDepth < ptr->getId().Y ||
      currentWidth < ptr->getId().X;
  };
  for( int32 i = m_width; i > 0; --i )
  {

    for( int32 j = m_depth; j > 0; --j )
    {
      AbasePrimitive** primitive = m_primitives.FindByPredicate(selectOnlyTheFloorPrimitives);
      if( nullptr != primitive )
      {
        worldPtr->DestroyActor(primitive[0]);
        m_primitives.RemoveSingle(primitive[0]);
      }
    }
  }

  m_primitives.Shrink();
  m_primitives.Sort();
}

void
AGrid3D::destroyAllGrid()
{
  for( TArray<AbasePrimitive*>::SizeType i = m_primitives.Num() - 1; i != 0; --i )
  {
    destroyPrimitive(m_primitives[i]);
  }

}

bool
AGrid3D::destroyPrimitive(AbasePrimitive* primitiveToDestroy)
{
  TArray<AbasePrimitive*>::SizeType indexOfElement;
  bool const isRemoved = m_primitives.Find(primitiveToDestroy, indexOfElement);

  if( INDEX_NONE != indexOfElement )
  {
    GetWorld()->DestroyActor(m_primitives[indexOfElement]);
    m_primitives.RemoveAt(indexOfElement);
    --m_count;
  }


  return isRemoved;
}


