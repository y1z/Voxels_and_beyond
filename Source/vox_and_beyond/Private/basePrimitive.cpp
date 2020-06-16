// Fill out your copyright notice in the Description page of Project Settings.

#include "basePrimitive.h"
#include "Engine/StaticMesh.h"


FString AbasePrimitive::s_cubeMeshPath =
TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube");

FString AbasePrimitive::s_sphereMeshPath =
TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere");

ConstructorHelpers::FObjectFinder<UStaticMesh>* AbasePrimitive::s_cubeMesh = nullptr;
ConstructorHelpers::FObjectFinder<UStaticMesh>* AbasePrimitive::s_sphereMesh = nullptr;
ConstructorHelpers::FObjectFinder<UStaticMesh>* AbasePrimitive::s_pyramidMesh = nullptr;


AbasePrimitive::AbasePrimitive()
  :
  m_nameOfmat(TEXT("")),
  m_pMesh(nullptr),
  m_pMaterialDynamic(nullptr),
  m_selectedShape(PrimitiveShape::cube),
  m_color(FColor(255, 0, 0))
{
  // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;
  m_pMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Primitive"));
  m_pMesh->SetupAttachment(RootComponent);


  static ConstructorHelpers::FObjectFinder<UStaticMesh>
  CubeVisualAssetStart(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));

  static ConstructorHelpers::FObjectFinder<UStaticMesh> 
  SphereVisualAssetStart(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));

  s_cubeMesh = &CubeVisualAssetStart;
  s_sphereMesh = &SphereVisualAssetStart;

  if( s_sphereMesh->Succeeded() )
  {
    m_pMesh->SetStaticMesh(s_sphereMesh->Object);
    //m_pMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
  }
}

// Called when the game starts or when spawned
void AbasePrimitive::BeginPlay()
{
  Super::BeginPlay();

  if( initMaterialInstanceDynamic() )
  {
    m_pMaterialDynamic->SetVectorParameterValue(m_nameOfmat, FColor(250, 0, 0));
  }
  //if( this->initMaterialInstanceDynamic() )
  //{
  //  TArray<FMaterialParameterInfo> paramInfo;
  //  TArray<FGuid> OutParameterIds;
  //}
}


// Called every frame
void AbasePrimitive::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
}

void AbasePrimitive::changeShape(PrimitiveShape shape)
{
  switch( shape )
  {
    case PrimitiveShape::cube:
    {
      //static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(*s_cubeMeshPath);

      if( s_cubeMesh->Succeeded() )
      {
        m_pMesh->SetStaticMesh(s_cubeMesh->Object);
        //m_pMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
      }
    }
    break;
    case PrimitiveShape::sphere:
    {
      //static ConstructorHelpers::FObjectFinder<UStaticMesh> VisualAsset(*s_sphereMeshPath);

      if( s_sphereMesh->Succeeded() )
      {
        m_pMesh->SetStaticMesh(s_sphereMesh->Object);
        //m_pMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
      }

    }

    default:
    break;
  }
}

bool 
AbasePrimitive::SetColor(FColor color)
{

  if( m_pMaterialDynamic != nullptr )
  {
    m_pMaterialDynamic->SetVectorParameterValue(m_nameOfmat, color);
    return true;
  }

  return false;
}

float 
AbasePrimitive::getHeight() const
{
  auto Top = m_pMesh->CalcBounds(m_pMesh->GetRelativeTransform()).GetBox().Max;
  auto buttom = m_pMesh->CalcBounds(m_pMesh->GetRelativeTransform()).GetBox().Min;

 return std::fabsf(Top.Z - buttom.Z);
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
    //m_pMaterialDynamic->GetAllVectorParameterInfo(OutParameterInfo, OutParameterIds);
    if( OutParameterInfo.Num() != 0 )
    {
      m_nameOfmat = OutParameterInfo[0].Name;

      return true;
    }
  }

  return false;

}

