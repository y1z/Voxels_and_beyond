// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid3D.h"
#include <limits>//std::numeric_limits

// Sets default values
AGrid3D::AGrid3D()
  :m_width(32),
  m_height(32),
  m_depth(32),
  m_pMesh(nullptr),
  m_count(0),
  m_primitiveLimitAmount(std::numeric_limits<int32>::max() * 2)

{
  // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;
	m_pMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("3D Grid")); 
	m_pMesh->SetupAttachment(RootComponent);

  m_primitives.reserve( m_width * m_height * m_depth);

 static ConstructorHelpers::FObjectFinder<UStaticMesh> Visual ( TEXT ("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));

 if( Visual.Succeeded() )
 {
   m_pMesh->SetStaticMesh(Visual.Object);
    m_pMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
  }
}

// Called when the game starts or when spawned
void AGrid3D::BeginPlay()
{
  Super::BeginPlay();
  m_pMesh->SetRelativeScale3D(FVector(m_width,m_depth,m_height));

  auto bounds = m_pMesh->CalcBounds(m_pMesh->GetRelativeTransform());
  m_topLeftPosition = bounds.GetBox().Max;

}

// Called every frame
void AGrid3D::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);

}


bool
AGrid3D::addPrimitiveToGrid(AbasePrimitive* primitive)
{
  if( m_primitiveLimitAmount >= m_count )
  {
    m_primitives.push_back(primitive);
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
    return m_primitives.at(index);
  }
  return nullptr;
}

