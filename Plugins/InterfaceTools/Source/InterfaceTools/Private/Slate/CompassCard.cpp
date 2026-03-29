#include "CompassCard.h"
#include "SlateGlobals.h"
#include "Framework/Text/PlainTextLayoutMarshaller.h"
#include "Widgets/Text/SlateTextBlockLayout.h"
#include "Types/ReflectionMetadata.h"
#include "Rendering/DrawElements.h"
#include "Framework/Application/SlateApplication.h"
#include "Fonts/FontMeasure.h"

SLATE_IMPLEMENT_WIDGET(SCompassCard)
void SCompassCard::PrivateRegisterAttributes(FSlateAttributeInitializer& AttributeInitializer)
{
  //SLATE_ADD_MEMBER_ATTRIBUTE_DEFINITION(AttributeInitializer, Font, EInvalidationReason::Paint);

}


void SCompassCard::Construct(const FArguments& InArgs)
{
  Heading.Assign(*this, InArgs._Heading); 
  Font.Assign(*this, InArgs._Font); 
}

void SCompassCard::SetHeading(TAttribute<float> InHeading)
{
  Heading.Assign(*this, MoveTemp(InHeading));
}

void SCompassCard::SetInset(TAttribute<float> InInset)
{
  Inset.Assign(*this, MoveTemp(InInset));
}

void SCompassCard::SetTickSize(TAttribute<float> InTickSize)
{
  TickSize.Assign(*this, MoveTemp(InTickSize));
}

void SCompassCard::SetFont(TAttribute<FSlateFontInfo> InFont)
{
  Font.Assign(*this, MoveTemp(InFont));
}

FSlateFontInfo SCompassCard::GetFont() const
{
  return GetFontRef();
}

const FSlateFontInfo& SCompassCard::GetFontRef() const
{
  return Font.Get();
}

void SCompassCard::SetColor(TAttribute<FLinearColor> Incolor)
{
  Color.Assign(*this, MoveTemp(Incolor));
}

int32 SCompassCard::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
  const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
  int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{

  DrawCompassCard(AllottedGeometry, OutDrawElements, LayerId++);

  return LayerId;
}

void SCompassCard::DrawCompassCard(const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements, int32 LayerId) const
{
  const FVector2D WidgetSize = AllottedGeometry.GetLocalSize();
  FVector2D Center = WidgetSize * 0.5f;

  float Radius = FMath::Min(Center.X, Center.Y) - Inset.Get();
  float TextHeight = TickSize.Get();
  float CurrentHeading = Heading.Get();

  // Draw cardinal directions and minor ticks
  for (int i = 0; i < 360; i += 10)
  {
    float Angle = FMath::DegreesToRadians(i - CurrentHeading);
    float TickLength = (i % 30 == 0) ? TextHeight : TextHeight/2;
    FVector2D Start = Center + FVector2D(FMath::Cos(Angle), FMath::Sin(Angle)) * (Radius - TickLength);
    FVector2D End = Center + FVector2D(FMath::Cos(Angle), FMath::Sin(Angle)) * Radius;

    FSlateDrawElement::MakeLines(
      OutDrawElements,
      LayerId,
      AllottedGeometry.ToPaintGeometry(),
      { Start, End },
      ESlateDrawEffect::None,
      CompassColor,
      true,
      (i % 30 == 0) ? 2.0f : 1.0f);

    const TSharedRef<FSlateFontMeasure>& FontMeasure = FSlateApplication::Get().GetRenderer()->GetFontMeasureService();

    // Add 2-digit labels at 30-degree intervals
    if (i % 30 == 0)
    {
      // Convert heading to 2-digit format (e.g., 0 -> "36", 30 -> "03")
      int DisplayHeading = i / 10; // Convert to 36, 03, etc.
      FString LabelText = DisplayHeading < 10 ? FString::Printf(TEXT("0%d"), DisplayHeading) : FString::Printf(TEXT("%d"), DisplayHeading);
      if (DisplayHeading == 0) LabelText = TEXT("36"); // Special case for 360�

      FVector2D TextSize = FontMeasure->Measure(LabelText, Font.Get());

      FSlateRenderTransform TextTransform;
      FPaintGeometry TextGeometry;

      FQuat2D RollQuat(Angle);
      FSlateRenderTransform RollTransform(RollQuat);

      // Draw Text
      TextTransform = Concatenate(
        FSlateRenderTransform(FVector2D(0, 0)), // Translate to origin
        FSlateRenderTransform(FScale2f(1, 1), FVector2f(FVector2D(-TextSize.X / 2, -(Radius - TickLength)))),
        RollTransform,                            // Apply rotation
        FSlateRenderTransform(FVector2f(FVector2D(WidgetSize.X / 2, WidgetSize.Y / 2)))
      );

      TextGeometry = AllottedGeometry.ToPaintGeometry(
        FVector2D(1, 1),
        FSlateLayoutTransform(),
        TextTransform
      );

      FSlateDrawElement::MakeText(
        OutDrawElements,
        LayerId,
        TextGeometry,
        LabelText,
        Font.Get()
      );
    }
  }
  
  // Draw outer ring
  {
    constexpr int32 Segments = 64;
    TArray<FVector2D> Points;
    Points.Reserve(Segments + 1);
 
    // Generate points along the arc
    for (int32 i = 0; i <= Segments; ++i)
    {
      float Alpha = static_cast<float>(i) / Segments;
      float Angle = FMath::Lerp(0, 2*PI, Alpha);
      FVector2D Point = Center + FVector2D(FMath::Cos(Angle), FMath::Sin(Angle)) * Radius;
      Points.Add(Point);
    }
  
    FSlateDrawElement::MakeLines(
      OutDrawElements,
      LayerId,
      AllottedGeometry.ToPaintGeometry(),
      Points, // Helper function to generate circle points
      ESlateDrawEffect::None,
      CompassColor,
      true,
      1.5f);
  }
}

FVector2D SCompassCard::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
  return FVector2D(250,250);
}