#pragma once

#pragma once

#include "CoreMinimal.h"
#include "Fonts/SlateFontInfo.h"
#include "Misc/Attribute.h"
#include "Widgets/SWidget.h"
#include "Widgets/SLeafWidget.h"
#include "SlateCore.h"

// Declaration of the SCompassCard widget
class SCompassCard : public SLeafWidget
{
  SLATE_DECLARE_WIDGET(SCompassCard, SLeafWidget)

public:
  SLATE_BEGIN_ARGS(SCompassCard)
    {
    }
    SLATE_ATTRIBUTE(float, Heading)
    
    SLATE_ATTRIBUTE(float, Inset)
    SLATE_ATTRIBUTE(float, TickSize)

    SLATE_ATTRIBUTE(FSlateFontInfo, Font)
    SLATE_ATTRIBUTE(FLinearColor, Color)
  SLATE_END_ARGS()

  SCompassCard(): Heading(*this,0), Inset(*this, 20), TickSize(*this, 20), Font(*this, FSlateFontInfo()), Color(*this, FLinearColor(0.8f, 0.8f, 0.8f)) {};
  ~SCompassCard() {};

  // Constructor
  void Construct(const FArguments& InArgs);

  void SetHeading(TAttribute<float> InHeading);

  void SetInset(TAttribute<float> InInset);
  void SetTickSize(TAttribute<float> InTickSize);

  void SetFont(TAttribute<FSlateFontInfo> InFont);
  FSlateFontInfo GetFont() const;

  const FSlateFontInfo& GetFontRef() const;

  void SetColor(TAttribute<FLinearColor> Incolor);

protected:
  // Override to compute the desired size
  virtual FVector2D ComputeDesiredSize(float LayoutScaleMultiplier) const override;

private:
  virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

  void DrawCompassCard(const FGeometry& Geometry, FSlateWindowElementList& OutDrawElements, int32 LayerId) const;

private:

  // TSlateAttribute for our properties
  TSlateAttribute<float, EInvalidateWidgetReason::Paint> Heading;

  TSlateAttribute<float, EInvalidateWidgetReason::Paint> Inset;
  TSlateAttribute<float, EInvalidateWidgetReason::Paint> TickSize;

  TSlateAttribute<FSlateFontInfo, EInvalidateWidgetReason::Paint> Font;
  TSlateAttribute<FLinearColor, EInvalidateWidgetReason::Paint> Color;

  // Colors consistent with glass cockpit displays
  const FLinearColor CompassColor = FLinearColor(0.8f, 0.8f, 0.8f);
};