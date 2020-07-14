// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility.h"
#include "Grid3D.h"
#include "Misc/FileHelper.h"
#include "DesktopPlatform/Public/IDesktopPlatform.h"
#include "HAL/PlatformFilemanager.h"


bool
UUtility::saveGridData(AGrid3D* const ptr_grid,
                       FString fileName)
{
  FString const fullDefaultPathForProject = FPaths::ConvertRelativePathToFull(FPaths::GameAgnosticSavedDir());
   
  FString const originalFileName = fileName;

  fileName.InsertAt(0, *fullDefaultPathForProject);


  FText errorText = FText::FromString(FString(TEXT(" ")));
  bool const isPathUsable = FFileHelper::IsFilenameValidForSaving(fileName, errorText);
  
  if(GEngine )
  {
    GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, errorText.ToString());
  }

  if( isPathUsable )
  {

    return FFileHelper::SaveStringToFile(fileName, *fileName);
  }
  else
  {
    UE_LOG(LogTemp, Warning, TEXT("invalid save location"));
  }

  return  isPathUsable;
  //TArray<FString> OutFiles;
  //bool const isSaveSucceful = IDesktopPlatform::SaveFileDialog(nullptr,
  //                                                             TEXT("Saving data In Grid"),
  //                                                             TEXT(""),
  //                                                             fullDefaultPathForProject,
  //                                                             TEXT(""),
  //                                                             EFileDialogFlags::None,
  //                                                             OutFiles);//OpenFileDialog(nullptr, TEXT(""), TEXT(""), , );

}
