// Fill out your copyright notice in the Description page of Project Settings.

// #include "basePrimitive.h"
#include "../Public/basePrimitive.h"
#include "Engine/StaticMesh.h"


FString AbasePrimitive::s_cubeMeshPath = 
(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));

FString AbasePrimitive::s_sphereMeshPath =
(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));

ConstructorHelpers::FObjectFinder<UStaticMesh>* AbasePrimitive::s_cubeMesh = nullptr;
ConstructorHelpers::FObjectFinder<UStaticMesh>* AbasePrimitive::s_sphereMesh = nullptr;
ConstructorHelpers::FObjectFinder<UStaticMesh>* AbasePrimitive::s_pyramidMesh = nullptr;


AbasePrimitive::AbasePrimitive()
		: m_pMesh(nullptr),
			m_selectedShape(PrimitiveShape::cube),
			m_color(FColor(0, 0, 0))
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_pMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Primitive")); 
	m_pMesh->SetupAttachment(RootComponent);

  //auto currentFilePath = std::filesystem::current_path();

  static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAssetStart(*s_cubeMeshPath);
  static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAssetStart(*s_sphereMeshPath);
  s_cubeMesh = &CubeVisualAssetStart;
  s_sphereMesh = &SphereVisualAssetStart;

  //auto currentPath = std::filesystem::current_path();
  if( s_sphereMesh->Succeeded() )
  {
    m_pMesh->SetStaticMesh(CubeVisualAssetStart.Object);
    //m_pMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
  }
}

// Called when the game starts or when spawned
void AbasePrimitive::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AbasePrimitive::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AbasePrimitive::changeShape(PrimitiveShape shape)
{
	switch (shape)
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

      if(s_sphereMesh->Succeeded() )
      {
        m_pMesh->SetStaticMesh(s_sphereMesh->Object);
        //m_pMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
      }

    }

	default:
		break;
	}
}

