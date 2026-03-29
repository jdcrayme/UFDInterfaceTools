// This application is provided as is, without any support

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HSIWidget.generated.h"

class SCompassCard;
class SCompassPointer;
class SCompassCoursePointer;

UCLASS()
class INTERFACETOOLS_API UHSIWidget : public UUserWidget
{
  GENERATED_BODY()

public:
  UHSIWidget(const FObjectInitializer& ObjectInitializer);

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Getter, Setter, BlueprintSetter = "SetHeading", Category = "Data")
  float Heading = 0.f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
  float Course = 0.f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
  float Deviation = 0.f;
  
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
  float Bearing = 0.f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Getter, Setter, BlueprintSetter = "SetToFlag", Category = "Data")
  bool ToFlag;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Getter, Setter, BlueprintSetter = "SetNoSignalFlag", Category = "Data")
  bool NoSignalFlag;

  // The font property exposed to the editor
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Getter, Setter, BlueprintSetter = "SetFont", Category = "Appearance")
  FSlateFontInfo Font;

  UFUNCTION(BlueprintCallable, Category = "Appearance")
  void SetFont(FSlateFontInfo InFontInfo);

  UFUNCTION(BlueprintCallable, Category = "Data")
  void SetHeading(float InHeading);

  UFUNCTION(BlueprintCallable, Category = "Data")
  float GetHeading() const { return Heading; }

  UFUNCTION(BlueprintCallable, Category = "Data")
  float GetCourse() const { return Course; } 

  UFUNCTION(BlueprintCallable, Category = "Data")
  bool GetToFlag() const { return ToFlag; }
  
  UFUNCTION(BlueprintCallable, Category = "Data")
  void SetToFlag(bool InToFlag) { ToFlag = InToFlag; }

  UFUNCTION(BlueprintCallable, Category = "Data")
  bool GetNoSignalFlag() const { return NoSignalFlag; }

  UFUNCTION(BlueprintCallable, Category = "Data")
  void SetNoSignalFlag(bool InNoSignalFlag) { NoSignalFlag = InNoSignalFlag; }

  float GetBearing() const { return Bearing; }
  FSlateFontInfo GetFont() const { return Font; }

  float GetBearingAngle() const
  {
    return FMath::Wrap(Bearing - Heading, 0.0f, 360.0f);
  }

  float GetCourseAngle() const
  {
    return FMath::Wrap(Course - Heading, 0.0f, 360.0f);
  }

protected:
  virtual TSharedRef<SWidget> RebuildWidget() override;
  virtual void SynchronizeProperties() override;

  void UpdateFont();

private:
  TSharedPtr<SCompassCard> CompassWidget;
  TSharedPtr<SCompassPointer> BearingPointerWidget;
  TSharedPtr<SCompassCoursePointer> CoursePointerWidget;

  float TextHeight;
};