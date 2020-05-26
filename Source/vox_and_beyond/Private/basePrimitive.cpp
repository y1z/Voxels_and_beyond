// Fill out your copyright notice in the Description page of Project Settings.

//#include "basePrimitive.h"
#include "../Public/basePrimitive.h"

FString AbasePrimitive::s_cubeMeshPath = (TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));
FString AbasePrimitive::s_sphereMeshPath = (TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
// Sets default values
AbasePrimitive::AbasePrimitive()
	:m_pMesh(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_pMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Primitive"));
	m_pMesh->SetupAttachment(RootComponent);


	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(*s_cubeMeshPath);

	if( CubeVisualAsset.Succeeded() )
	{
		m_pMesh->SetStaticMesh(CubeVisualAsset.Object);
		m_pMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	}
}

// Called when the game starts or when spawned
void 
AbasePrimitive::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void 
AbasePrimitive::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

