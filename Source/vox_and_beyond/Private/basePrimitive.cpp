// Fill out your copyright notice in the Description page of Project Settings.

#include "basePrimitive.h"
#include "Engine/StaticMesh.h"
#include <climits>

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
  m_color(FColor(255, 0, 0)),
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

  if( s_sphereMesh->Succeeded() )
  {
    m_pMesh->SetStaticMesh(s_sphereMesh->Object);
    TArray<USceneComponent*>Components;
    m_pMesh->GetParentComponents(Components);
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
AbasePrimitive::operator>(AbasePrimitive const *other) const
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
AbasePrimitive::operator>(AbasePrimitive const &other) const
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



void
AbasePrimitive::init(FIntVector const& PrimitiveID, FVector const& Location)
{
  m_id = PrimitiveID; 
  m_pMesh->SetRelativeLocation(Location);
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

void
AbasePrimitive::rotatePrimitive(int32 horizontalRotations,
                                int32 verticalRotations ,
                                int32 zAxisRotation )
{
  constexpr static int32 rotationStep = 90;

  FRotator const resultRotator(verticalRotations * rotationStep,
                               horizontalRotations * rotationStep,
                               zAxisRotation * rotationStep);
  m_pMesh->AddLocalRotation(resultRotator);
 //m_pMesh->SetRelativeRotation(resultRotator);
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

