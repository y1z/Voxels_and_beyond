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
  pyramid UMETA(DisplayName = "pyramid shape "),
	quadPyramid UMETA(DisplayName = "quadPyramid shape "),
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
  operator <(AbasePrimitive const* other) const;

  /**
  * @brief Compares the 2 primitives distance from the coordinate (0,0,0)
  */
  bool 
  operator >(AbasePrimitive const* other) const;

  /**
  * @brief Compares the 2 primitives distance from the coordinate (0,0,0)
  */
  bool
  operator == (AbasePrimitive const* other)const;


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

  /**
  * @brief Compares the 2 primitives distance from the coordinate (0,0,0)
  */
  bool
  operator == (AbasePrimitive const& other)const;

  /**
  * @brief Compares 2 AbasePrimitive's with the > operator.(doing this because unreal engine)
  */
  bool
  operator() (AbasePrimitive const* other)const; 

public:

	UFUNCTION(BlueprintCallable)
  void 
  init(FIntVector const PrimitiveID,
       FVector const Location,
       AbasePrimitive * primitive = nullptr,
       bool attachToPrimitive = true);

  /**
  * @brief 
  * @bug
  * @param[in] 
  */
  void
  calculatePoints();

  FIntVector 
  calculatePosition(FVector point)const;

  /**
  * @brief Changes the shape of the current primitive.
  */
	UFUNCTION(BlueprintCallable)
	void
  setShape(PrimitiveShape shape);

  /**
  * @brief Changes the color of the primitive
  */
	UFUNCTION(BlueprintCallable)
	bool
  setColor(FColor color);

  /**
  * @returns The height of the primitive 
  */
	UFUNCTION(BlueprintCallable)
	float
  getHeight()const;

  /**
  * @returns The Width of the primitive 
  */
	UFUNCTION(BlueprintCallable)
	float
  getWidth()const;

  /**
  * @returns The Depth of the primitive 
  */
	UFUNCTION(BlueprintCallable)
	float
  getDepth()const;

  /**
  * @returns The center of the primitive relative to world space.
  */
	UFUNCTION(BlueprintCallable)
  FVector 
  getAbsoluteCenter()const;

  /**
  * @returns The center of the primitive relative to its self.
  */
	UFUNCTION(BlueprintCallable)
  FVector 
  getRelativeToSelfCenter()const;


  /**
  * @returns The Top left corner of the primitive.
  */
	UFUNCTION(BlueprintCallable)
  FVector 
  getTopLeftCorner()const;


	UFUNCTION(BlueprintCallable)
  void 
  rotatePrimitive(int32 horizontalRotations, int32 verticalRotations = 0 , int32 zAxisRotation = 0);

  /**
  * @returns The Id of the Primitive
  */
	UFUNCTION(BlueprintCallable)
  FIntVector getId()const;


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
	virtual void BeginPlay() override;
public:
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
  UPROPERTY(EditAnywhere)
  UStaticMeshComponent* m_pMesh;

  /**
  * @brief The material used to color the primitive.
  */
  UPROPERTY(EditAnywhere)
  UMaterialInstanceDynamic* m_pMaterialDynamic;

private:
  /**
  * @brief Defines an Id to be used to find a vector.
  */
	UPROPERTY(VisibleAnywhere)
  FIntVector m_id;

  /**
  * @brief Defines a point directlly in front of the primitive
  */
  FVector m_frontPoint;


  /**
  * @brief Defines a point directlly in front of the primitive
  */
  FVector m_backPoint;

  /**
  * @brief Defines a point directlly in front of the primitive
  */
  FVector m_rightPoint;

  /**
  * @brief Defines a point directlly in front of the primitive
  */
  FVector m_leftPoint;

  /**
  * @brief Defines a point directlly in front of the primitive
  */
  FVector m_downPoint;

  /**
  * @brief Defines a point directlly in front of the primitive
  */
  FVector m_upPoint;
protected:
	UPROPERTY(VisibleAnywhere)
	PrimitiveShape m_selectedShape;

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

