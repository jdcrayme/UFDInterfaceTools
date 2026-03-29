#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SADI : public SCompoundWidget
{
public:
  SLATE_BEGIN_ARGS(SADI)
    {
    }
    SLATE_ATTRIBUTE(float, Roll)  // Roll angle in degrees
      SLATE_ATTRIBUTE(float, Pitch) // Pitch angle in degrees
      SLATE_ATTRIBUTE(float, Slip)  // Slip/skid value (-1 to 1)

  SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

protected:
  virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

private:
  TAttribute<float> Roll;  // Roll angle (degrees)
  TAttribute<float> Pitch; // Pitch angle (degrees)
  TAttribute<float> Slip;  // Slip/skid value

  // Helper functions for drawing
  void DrawPitchScale(const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements, int32 LayerId) const;
  void DrawRollScale(const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements, int32 LayerId) const;
  void DrawRollPointer(const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements, int32 LayerId) const;
  void DrawAircraftSymbol(const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements, int32 LayerId) const;
  void DrawSlipSkidIndicator(const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements, int32 LayerId) const;
};