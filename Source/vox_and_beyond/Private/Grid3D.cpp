// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid3D.h"
#include <limits>//std::numeric_limits
#include <cmath>

ConstructorHelpers::FObjectFinder<UStaticMesh>* AGrid3D::s_pointerToMesh = nullptr;
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
  m_primitiveLimitAmount(int64(std::numeric_limits<int64>::max() /  std::numeric_limits<int32>::max()))

{
  // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = false;
  m_pMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("3D Grid"));
  m_pMesh->SetupAttachment(RootComponent);

  m_primitives.Reserve(m_width * m_height * m_depth);

  static ConstructorHelpers::FObjectFinder<UStaticMesh> Visual(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));

  s_pointerToMesh = &Visual;

  if( s_pointerToMesh->Succeeded() )
  {
    m_pMesh->SetStaticMesh( s_pointerToMesh->Object);
    float const smallestValue = std::numeric_limits<float>::epsilon();
    m_pMesh->GetCollisionShape().SetBox(FVector(smallestValue, smallestValue, smallestValue));
    
    m_pMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
  }


}

// Called when the game starts or when spawned
void AGrid3D::BeginPlay()
{
  Super::BeginPlay();

  m_pMesh->SetWorldLocation(FVector(0,0,0));
  m_pMesh->SetRelativeScale3D(FVector(m_width, m_depth, m_height));

  auto bounds = m_pMesh->CalcBounds(m_pMesh->GetRelativeTransform());
  m_topLeftPosition = bounds.GetBox().Max;
  m_buttomRightPostion = bounds.GetBox().Min;

  float const lowestZ = m_buttomRightPostion.Z;

  FActorSpawnParameters spawnParam;
  spawnParam.Template = nullptr;
  spawnParam.Owner = this;

  this->calculateSizeBetweenCubes();

  for( int32 i = m_width; i > 0 ; --i )
  {
    
    for (int32 j = m_depth; j > 0 ; --j )
    {
      AbasePrimitive* tempPtr = GetWorld()->SpawnActor<AbasePrimitive>(spawnParam);
      FVector const InGridPosition = FVector((m_deltaWidth * i), (m_deltaDepth * j), lowestZ);
      FTransform const transformForGridPosition(this->m_buttomRightPostion + InGridPosition);

      tempPtr->m_pMesh->SetWorldTransform(transformForGridPosition);

      tempPtr->setColor(FColor(224, 189,150,255));
      tempPtr->setShape(PrimitiveShape::cube);
      m_primitives.Add(tempPtr);
    }

  }

  m_pMesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

  m_pMesh->SetVisibility(false);
  
  //m_pMesh->MoveComponent(m_topLeftPosition,
  //                       FRotator(0, 0, 0)
  //                       ,false);
}

// Called every frame
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

void
AGrid3D::TransFormPrimitive(int64 index)
{

  m_primitives[index]->m_pMesh->SetRelativeLocation(m_topLeftPosition);
}

AbasePrimitive*
AGrid3D::SpwanInGrid(FVector point)
{
  auto bounds = m_pMesh->CalcBounds(m_pMesh->GetRelativeTransform());
  FActorSpawnParameters spawnParam;
  spawnParam.Template = nullptr;
  spawnParam.Owner = nullptr;
  if(bounds.GetBox().IsInsideOrOn(point))
  {
    AbasePrimitive* tempPtr = GetWorld()->SpawnActor<AbasePrimitive>(spawnParam);
    tempPtr->m_pMesh->SetRelativeLocation(point);
    tempPtr->setColor(FColor::Cyan);

    m_primitives.Add(tempPtr);
    m_primitives.Sort();
    return tempPtr;
    
  }

  return nullptr;
}

void 
AGrid3D::calculateSizeBetweenCubes() 
{
  m_deltaWidth = std::fabsf(m_topLeftPosition.X - m_buttomRightPostion.X) / std::max(1, m_width);
  m_deltaDepth = std::fabsf(m_topLeftPosition.Y - m_buttomRightPostion.Y) / std::max(1, m_depth);
  m_deltaHeight = std::fabsf(m_topLeftPosition.Z - m_buttomRightPostion.Z) / std::max(1, m_height);
}


