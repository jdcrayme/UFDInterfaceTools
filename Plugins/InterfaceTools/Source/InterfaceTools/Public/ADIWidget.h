// This application is provided as is, without any support

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ADIWidget.generated.h"

/**
 * 
 */
UCLASS()
class INTERFACETOOLS_API UADIWidget : public UUserWidget
{
  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attitude")
  float Roll;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attitude")
  float Pitch;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attitude")
  float Slip;

  float GetRoll() const { return Roll; }
  float GetPitch() const { return Pitch; }
  float GetSlip() const { return Slip; }

protected:
  virtual TSharedRef<SWidget> RebuildWidget() override;
  virtual void SynchronizeProperties() override;
};