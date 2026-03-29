#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SCompassCoursePointer : public SLeafWidget
{
  SLATE_DECLARE_WIDGET(SCompassCoursePointer, SLeafWidget)

public:
  SLATE_BEGIN_ARGS(SCompassCoursePointer)
    {
    }
    SLATE_ATTRIBUTE(float, Angle)

    SLATE_ATTRIBUTE(float, Inset)
    SLATE_ATTRIBUTE(float, TickSize)

    SLATE_ATTRIBUTE(float, Deviation)
    SLATE_ATTRIBUTE(bool, ToFlag)
    SLATE_ATTRIBUTE(bool, NoSignalFlag)

    SLATE_ATTRIBUTE(FLinearColor, Color)
  SLATE_END_ARGS()
  
  SCompassCoursePointer() : Angle(*this, 0), Inset(*this, 20), TickSize(*this, 20), Deviation(*this, 0), ToFlag(*this, false), NoSignalFlag(*this, false), Color(*this, FLinearColor(0.2f, 1.0f, 0.3f)) {};
  ~SCompassCoursePointer() {};

  void Construct(const FArguments& InArgs);

  void SetAngle(float InAngle);

  void SetInset(TAttribute<float> InInset);
  void SetTickSize(TAttribute<float> InTickSize);

  void SetDeviation(TAttribute<float> InDeviation);
  void SetToFlag(TAttribute<bool> InToFlag);
  void SetNoSignalFlag(TAttribute<bool> InNoSignalFlag);

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

  TSlateAttribute<float, EInvalidateWidgetReason::Paint> Deviation;
  TSlateAttribute<bool, EInvalidateWidgetReason::Paint> ToFlag;
  TSlateAttribute<bool, EInvalidateWidgetReason::Paint> NoSignalFlag;

  TSlateAttribute<FLinearColor, EInvalidateWidgetReason::Paint> Color;
};