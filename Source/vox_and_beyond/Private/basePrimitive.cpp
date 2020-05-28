// Fill out your copyright notice in the Description page of Project Settings.

// #include "basePrimitive.h"
#include "../Public/basePrimitive.h"



FString AbasePrimitive::s_cubeMeshPath = 
(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));

FString AbasePrimitive::s_sphereMeshPath =
(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
// Sets default values
AbasePrimitive::AbasePrimitive()
		: m_pMesh(nullptr),
			m_selectedShape(PrimitiveShape::cube),
			m_color(FColor(0, 0, 0))
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_pMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Primitive")); 
	m_pMesh->SetupAttachment(RootComponent);

	//changeShape(m_selectedShape);
  static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAssetStart(*s_cubeMeshPath);

      if( CubeVisualAssetStart.Succeeded() )
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
      static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(*s_cubeMeshPath);

      if( CubeVisualAsset.Succeeded() )
      {
        m_pMesh->SetStaticMesh(CubeVisualAsset.Object);
        //m_pMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
      }
    }
	break;
    case PrimitiveShape::sphere:
    {
      static ConstructorHelpers::FObjectFinder<UStaticMesh> VisualAsset(*s_sphereMeshPath);

      if( VisualAsset.Succeeded() )
      {
        m_pMesh->SetStaticMesh(VisualAsset.Object);
        //m_pMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
      }

    }

	default:
		break;
	}
}

bool AbasePrimitive::tempChangeShape(int i)
{
  
	switch (i)
	{
    case 0:
    {
      ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(*s_cubeMeshPath);

      if( CubeVisualAsset.Succeeded() )
      {
        m_pMesh->SetStaticMesh(CubeVisualAsset.Object);
        //m_pMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
      }
    }
	break;
    case 1:
    {
      ConstructorHelpers::FObjectFinder<UStaticMesh> VisualAsset(*s_sphereMeshPath);

      if( VisualAsset.Succeeded() )
      {
        m_pMesh->SetStaticMesh(VisualAsset.Object);
        //m_pMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
      }

    }

    break;
	default:
		break;
	}
  return true;
}
