#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SCompassPointer : public SLeafWidget
{
  SLATE_DECLARE_WIDGET(SCompassPointer, SLeafWidget)

public:
  SLATE_BEGIN_ARGS(SCompassPointer)
    {
    }
    SLATE_ATTRIBUTE(float, Angle)

    SLATE_ATTRIBUTE(float, Inset)
    SLATE_ATTRIBUTE(float, TickSize)

    SLATE_ATTRIBUTE(FLinearColor, Color)
  SLATE_END_ARGS()
  
  SCompassPointer() : Angle(*this, 0), Inset(*this, 20), TickSize(*this, 20), Color(*this, FLinearColor(1.0f, 0.1f, 0.3f)) {};
  ~SCompassPointer() {};

  void Construct(const FArguments& InArgs);

  void SetAngle(float InAngle);

  void SetInset(TAttribute<float> InInset);
  void SetTickSize(TAttribute<float> InTickSize);

  void SetColor(TAttribute<FLinearColor> Incolor);

protected:
  // Override to compute the desired size
  virtual FVector2D ComputeDesiredSize(float LayoutScaleMultiplier) const override;

private:
  virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
    const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
    int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

  void DrawCompassPointer(const FGeometry& Geometry, FSlateWindowElementList& OutDrawElements, int32 LayerId) const;

  TSlateAttribute<float, EInvalidateWidgetReason::Paint> Angle;

  TSlateAttribute<float, EInvalidateWidgetReason::Paint> Inset;
  TSlateAttribute<float, EInvalidateWidgetReason::Paint> TickSize;

  TSlateAttribute<FLinearColor, EInvalidateWidgetReason::Paint> Color;
};