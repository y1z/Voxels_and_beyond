// Fill out your copyright notice in the Description page of Project Settings.  
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Misc/Paths.h"
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

	UFUNCTION(BlueprintCallable)
	void changeShape(PrimitiveShape shape);

	UFUNCTION(BlueprintCallable)
	bool SetColor(FColor color);

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

public:// variables

  /**
  * @brief The name of the Material used to color the primitive.
  */
	UPROPERTY(VisibleAnywhere)
	FName m_nameOfmat;

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
	static FString s_cubeMeshPath;

	static FString s_sphereMeshPath;

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
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};

