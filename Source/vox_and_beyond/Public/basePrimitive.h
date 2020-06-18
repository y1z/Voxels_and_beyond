// Fill out your copyright notice in the Description page of Project Settings.  
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/ConstructorHelpers.h"

#include "basePrimitive.generated.h"

UENUM(BlueprintType)
enum class PrimitiveShape : uint8
{
	cube UMETA(DisplayName = "cube shape"),
	sphere UMETA(DisplayName = "sphere shape"),
  pyramid UMETA(DisplayName = "sphere pyramid"),
	quadPyramid UMETA(DisplayName = "sphere quadPyramid"),
};


UCLASS(Blueprintable)
class VOX_AND_BEYOND_API AbasePrimitive : public AActor
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AbasePrimitive();
public: // operators 

  /**
  * @brief Compares the 2 primitives distance from the coordinate (0,0,0)
  */
  bool 
  operator <(AbasePrimitive const& other) const;

  /**
  * @brief Compares the 2 primitives distance from the coordinate (0,0,0)
  */
  bool 
  operator >(AbasePrimitive const& other) const;


public:

  /**
  * @brief Changes the shape of the current primitive.
  */
	UFUNCTION(BlueprintCallable)
	void setShape(PrimitiveShape shape);

  /**
  * @brief Changes the color of the primitive
  */
	UFUNCTION(BlueprintCallable)
	bool setColor(FColor color);

  /**
  * @returns The height of the primitive 
  */
	UFUNCTION(BlueprintCallable)
	float getHeight()const;


private:

	TCHAR* getMaterialColorName() const
	{
	  return TEXT("JustColor");
	}

  /**
  * @brief Initializes the dynamic material instance.
  * @bug no known bugs.
  */
	bool 
  initMaterialInstanceDynamic();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:// variables

  /**
  * @brief The name of the Material used to color the primitive.
  */
	UPROPERTY(VisibleAnywhere)
	FName m_nameOfColorMat;

  /**
  * @brief The name static mesh used to visualize the primitive
  */
	UPROPERTY(VisibleAnywhere)
  UStaticMeshComponent* m_pMesh;
  


  /**
  * @brief The material used to color the primitive.
  */
  UPROPERTY(EditAnywhere)
  UMaterialInstanceDynamic* m_pMaterialDynamic;
private:  
public:
	UPROPERTY(EditAnywhere)
  FColor m_color;

protected:
	UPROPERTY(VisibleAnywhere)
	PrimitiveShape m_selectedShape;

protected:

  /**
  * @brief A pointer to a cube mesh
  */
  static ConstructorHelpers::FObjectFinder<UStaticMesh>* s_cubeMesh;

  /**
  * @brief A pointer to a sphere mesh. 
  */
  static ConstructorHelpers::FObjectFinder<UStaticMesh>* s_sphereMesh;

  /**
  * @brief A pointer to a pyramid mesh. 
  */
  static ConstructorHelpers::FObjectFinder<UStaticMesh>* s_pyramidMesh;

  /**
  * @brief A pointer to a pyramid quad mesh. 
  */
  static ConstructorHelpers::FObjectFinder<UStaticMesh>* s_pyramidQuadMesh;
};

