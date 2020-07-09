// Fill out your copyright notice in the Description page of Project Settings.

#include "basePrimitive.h"
#include "Engine/StaticMesh.h"
#include <climits>
#include <utility>
#include <vector>
#include <algorithm>

ConstructorHelpers::FObjectFinder<UStaticMesh>* AbasePrimitive::s_cubeMesh = nullptr;
ConstructorHelpers::FObjectFinder<UStaticMesh>* AbasePrimitive::s_sphereMesh = nullptr;
ConstructorHelpers::FObjectFinder<UStaticMesh>* AbasePrimitive::s_pyramidMesh = nullptr;
ConstructorHelpers::FObjectFinder<UStaticMesh>* AbasePrimitive::s_pyramidQuadMesh = nullptr;


AbasePrimitive::AbasePrimitive()
  :
  m_nameOfColorMat(TEXT("")),
  m_pMesh(nullptr),
  m_pMaterialDynamic(nullptr),
  m_id(FIntVector(std::numeric_limits<int32>::min(), std::numeric_limits<int32>::min(), std::numeric_limits<int32>::min())),
  m_frontPoint(FVector::ZeroVector),
  m_backPoint(FVector::ZeroVector),
  m_rightPoint(FVector::ZeroVector),
  m_selectedShape(PrimitiveShape::cube)
{
  // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;
  m_pMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Primitive"));
  m_pMesh->SetupAttachment(RootComponent);

  static ConstructorHelpers::FObjectFinder<UStaticMesh>
  CubeVisual(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));

  static ConstructorHelpers::FObjectFinder<UStaticMesh>
  SphereVisual(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));

  static ConstructorHelpers::FObjectFinder<UStaticMesh>
  PyramidVisual(TEXT("/Game/StarterContent/Shapes/Shape_TriPyramid.Shape_TriPyramid"));

  static ConstructorHelpers::FObjectFinder<UStaticMesh>
  PyramidQuadVisual(TEXT("/Game/StarterContent/Shapes/Shape_QuadPyramid.Shape_QuadPyramid"));


  s_cubeMesh = &CubeVisual;
  s_sphereMesh = &SphereVisual;
  s_pyramidMesh = &PyramidVisual;
  s_pyramidQuadMesh = &PyramidQuadVisual;

  if( s_cubeMesh->Succeeded() )
  {
    m_pMesh->SetStaticMesh(s_cubeMesh->Object);
  }
}

bool
AbasePrimitive::operator<(AbasePrimitive const* other) const
{
  if( m_id.X < other->m_id.X )
    return true;

  else if( m_id.Y < other->m_id.Y )
    return true;

  else if( m_id.Z < other->m_id.Z )
    return true;

  return false;
}

bool
AbasePrimitive::operator>(AbasePrimitive const* other) const
{

  if( m_id.X > other->m_id.X )
    return true;

  else if( m_id.Y > other->m_id.Y )
    return true;

  else if( m_id.Z > other->m_id.Z )
    return true;

  return false;
}

bool
AbasePrimitive::operator==(AbasePrimitive const* other) const
{
  return m_id == other->m_id;
}


bool
AbasePrimitive::operator<(AbasePrimitive const& other) const
{
  if( m_id.X < other.m_id.X )
    return true;

  else if( m_id.Y < other.m_id.Y )
    return true;

  else if( m_id.Z < other.m_id.Z )
    return true;

  return false;
}

bool
AbasePrimitive::operator>(AbasePrimitive const& other) const
{

  if( m_id.X > other.m_id.X )
    return true;

  else if( m_id.Y > other.m_id.Y )
    return true;

  else if( m_id.Z > other.m_id.Z )
    return true;

  return false;
}


bool
AbasePrimitive::operator==(AbasePrimitive const& other) const
{
  return m_id == other.m_id;
}

bool
AbasePrimitive::operator()(AbasePrimitive const* other) const
{
  return (*this == other);
}

void
AbasePrimitive::init(FIntVector const PrimitiveID,
                     FVector const Location,
                     AbasePrimitive* primitive,
                     bool normalPosition)
{

  if( !normalPosition )
  {
    calculatePoints();
    FIntVector const relativeVector = calculatePosition(Location);

    FVector newLocationVector;

    if( nullptr != primitive )
     newLocationVector = primitive->m_pMesh->GetRelativeLocation();
    else
    {
      newLocationVector = Location;
    }

    FVector const conversionVector(relativeVector.X, relativeVector.Y, relativeVector.Z);
    
    FVector const sizePerAxis(getWidth(), getDepth(), getHeight());

    newLocationVector += (sizePerAxis * conversionVector);
    m_pMesh->SetRelativeLocation(newLocationVector);

    m_id = PrimitiveID + relativeVector;
  }
  else
  {
    m_id = PrimitiveID;
    m_pMesh->SetRelativeLocation(Location);
  }
}


void 
AbasePrimitive::calculatePoints()
{
  auto const box = m_pMesh->CalcBounds(m_pMesh->GetRelativeTransform()).GetBox();
 
  FVector const center = box.GetCenter();// (box.Max - box.Min) * 0.5f;

  float const ZDelta = std::fabsf(box.Max.Z - box.Min.Z) * 0.5f;
  float const YDelta = std::fabsf(box.Max.Y - box.Min.Y) * 0.5f;
  float const XDelta = std::fabsf(box.Max.X - box.Min.X) * 0.5f;

  m_upPoint = center + FVector(0.0f, 0.0f, ZDelta);
  m_downPoint = m_upPoint + FVector(0.0f, 0.0f, (-2.0f * ZDelta));

  m_frontPoint = center + FVector(0.0f, YDelta, 0.0f);
  m_backPoint = m_frontPoint + FVector(0.0f, (-2.0f * YDelta), 0.0f);

  m_rightPoint = center + FVector(XDelta, 0.0f, 0.0f);
  m_leftPoint = m_rightPoint + FVector((-2.0f * XDelta), 0.0f, 0.0f);
}

FIntVector 
AbasePrimitive::calculatePosition(FVector point) const
{
  enum class Point { back,front,up,down,right,left };

  struct VecAndPoint 
  {
    FVector vector;
    Point point;
  };

  std::vector<std::pair<VecAndPoint , float >> vectorsAndDistances;

  vectorsAndDistances.push_back({ {m_frontPoint, Point::front },0.0 });
  vectorsAndDistances.push_back({ {m_backPoint, Point::back } ,0.0 });
  vectorsAndDistances.push_back({ {m_upPoint,Point::up},0.0 });
  vectorsAndDistances.push_back({ {m_downPoint,Point::down},0.0 });
  vectorsAndDistances.push_back({ {m_rightPoint,Point::right},0.0 });
  vectorsAndDistances.push_back({ {m_leftPoint,Point::left},0.0 });


  for( auto& vecs : vectorsAndDistances )
  {
    vecs.second = (point - vecs.first.vector).SizeSquared();
  }

  VecAndPoint shortestVector; //shortest
  float smallest = std::numeric_limits<float>::max();
  for( auto& vecs : vectorsAndDistances )
  {
    if( vecs.second < smallest )
    {
      smallest = vecs.second;
      shortestVector = vecs.first;

    }

    
  }

  FIntVector result(0,0,0);
  switch (shortestVector.point) 
  {
    case Point::back:

    result.Y = -1;
    break;
    case Point::front:

    result.Y = 1;
    break;
    case Point::up:

    result.Z = 1;
    break;
    case Point::down: 

    result.Z = -1;
    break;
    case Point::right: 

    result.X = 1;
    break;
    case Point::left: 

    result.X = -1;
    break;
    default:;
  };


 return  result;
}


// Called when the game starts or when spawned
void AbasePrimitive::BeginPlay()
{
  Super::BeginPlay();

  if( initMaterialInstanceDynamic() )
  {
    m_pMaterialDynamic->SetVectorParameterValue(m_nameOfColorMat,
                                                FColor(250, 0, 0));
  }
}


// Called every frame
void
AbasePrimitive::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
}

void
AbasePrimitive::setShape(PrimitiveShape shape)
{
  switch( shape )
  {

    case PrimitiveShape::cube:
    {
      if( s_cubeMesh->Succeeded() )
      {
        m_pMesh->SetStaticMesh(s_cubeMesh->Object);
      }
    }
    break;

    case PrimitiveShape::sphere:
    {
      if( s_sphereMesh->Succeeded() )
      {
        m_pMesh->SetStaticMesh(s_sphereMesh->Object);
      }
    }
    break;

    case PrimitiveShape::pyramid:
    {
      if( s_pyramidMesh->Succeeded() )
      {
        m_pMesh->SetStaticMesh(s_pyramidMesh->Object);
      }
    }
    break;

    case PrimitiveShape::quadPyramid:
    {
      if( s_pyramidQuadMesh->Succeeded() )
      {
        m_pMesh->SetStaticMesh(s_pyramidQuadMesh->Object);
      }
    }
    break;

    default:
    break;
  }

}

bool
AbasePrimitive::setColor(FColor color)
{

  if( m_pMaterialDynamic != nullptr )
  {
    m_pMaterialDynamic->SetVectorParameterValue(m_nameOfColorMat, color);
    return true;
  }

  return false;
}

float
AbasePrimitive::getHeight() const
{
  auto const top = m_pMesh->CalcBounds(m_pMesh->GetRelativeTransform()).GetBox().Max;
  auto const bottom = m_pMesh->CalcBounds(m_pMesh->GetRelativeTransform()).GetBox().Min;

  return std::fabsf(top.Z - bottom.Z);
}

float
AbasePrimitive::getWidth() const
{
  auto const left = m_pMesh->CalcBounds(m_pMesh->GetRelativeTransform()).GetBox().Max;
  auto const right = m_pMesh->CalcBounds(m_pMesh->GetRelativeTransform()).GetBox().Min;
  return std::fabsf(left.X - right.X);
}

float
AbasePrimitive::getDepth() const
{
  auto const front = m_pMesh->CalcBounds(m_pMesh->GetRelativeTransform()).GetBox().Max;
  auto const back = m_pMesh->CalcBounds(m_pMesh->GetRelativeTransform()).GetBox().Min;
  return std::fabsf(front.Y - back.Y);
}

FVector
AbasePrimitive::getAbsoluteCenter() const
{
  return m_pMesh->CalcBounds(m_pMesh->GetRelativeTransform()).GetBox().GetCenter(); 
}

FVector
AbasePrimitive::getRelativeToSelfCenter() const
{
  auto const Box = m_pMesh->CalcBounds(m_pMesh->GetRelativeTransform()).GetBox();
  return (Box.Max - Box.Min) * 0.5f;
}

FVector 
AbasePrimitive::getTopLeftCorner() const
{
  return m_pMesh->CalcBounds(m_pMesh->GetRelativeTransform()).GetBox().Max;
}

void
AbasePrimitive::rotatePrimitive(int32 horizontalRotations,
                                int32 verticalRotations,
                                int32 zAxisRotation)
{
  constexpr static int32 rotationStep = 90;

  FRotator const resultRotator(verticalRotations * rotationStep,
                               horizontalRotations * rotationStep,
                               zAxisRotation * rotationStep);
  m_pMesh->AddLocalRotation(resultRotator);
}

FIntVector
AbasePrimitive::getId() const
{
  return m_id;
}

bool
AbasePrimitive::initMaterialInstanceDynamic()
{

  auto material = m_pMesh->GetMaterial(0);
  m_pMaterialDynamic = UMaterialInstanceDynamic::Create(material, nullptr);

  if( nullptr != m_pMaterialDynamic )
  {
    m_pMesh->SetMaterial(0, m_pMaterialDynamic);

    TArray<FMaterialParameterInfo> OutParameterInfo;
    TArray<FGuid> OutParameterIds;

    m_pMesh->GetMaterial(0)->GetAllVectorParameterInfo(OutParameterInfo, OutParameterIds);

    if( OutParameterInfo.Num() != 0 )
    {
      m_nameOfColorMat = OutParameterInfo[0].Name;

      auto result = OutParameterInfo.Find(getMaterialColorName());
      return true;
    }
  }

  return false;
}

