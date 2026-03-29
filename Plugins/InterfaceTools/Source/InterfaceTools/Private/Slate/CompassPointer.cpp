#include "CompassPointer.h"

#include "Fonts/FontMeasure.h"

SLATE_IMPLEMENT_WIDGET(SCompassPointer)
void SCompassPointer::PrivateRegisterAttributes(FSlateAttributeInitializer& AttributeInitializer)
{
  //SLATE_ADD_MEMBER_ATTRIBUTE_DEFINITION(AttributeInitializer, Font, EInvalidationReason::Paint);

}

void SCompassPointer::Construct(const FArguments& InArgs)
{
  Angle.Assign(*this, InArgs._Angle);
  Inset.Assign(*this, InArgs._Inset);
  TickSize.Assign(*this, InArgs._TickSize);
  Color.Assign(*this, InArgs._Color);
}

void SCompassPointer::SetAngle(float InAngle)
{
  Angle.Assign(*this, MoveTemp(InAngle));
}

void SCompassPointer::SetInset(TAttribute<float> InInset)
{
  Inset.Assign(*this, MoveTemp(InInset));
}

void SCompassPointer::SetTickSize(TAttribute<float> InTickSize)
{
  TickSize.Assign(*this, MoveTemp(InTickSize));
}

void SCompassPointer::SetColor(TAttribute<FLinearColor> Incolor)
{
  Color.Assign(*this, MoveTemp(Incolor));
}


FVector2D SCompassPointer::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
  return FVector2D(250,250);
}

int32 SCompassPointer::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
  const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
  int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{

  DrawCompassPointer(AllottedGeometry, OutDrawElements, LayerId++);

  return LayerId;
}

void SCompassPointer::DrawCompassPointer(const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements, int32 LayerId) const
{
  FVector2D Center = AllottedGeometry.GetLocalSize() * 0.5f;

  float Tick = TickSize.Get();
  float Radius = FMath::Min(Center.X, Center.Y) - Inset.Get();
  float CurrentAngle = Angle.Get();

  FLinearColor DrawColor = Color.Get();

  const FVector2D WidgetSize = AllottedGeometry.GetLocalSize();

  FSlateRenderTransform TextTransform;
  FPaintGeometry TextGeometry;

  FQuat2D RollQuat(FMath::DegreesToRadians(CurrentAngle));
  FSlateRenderTransform RollTransform(RollQuat);

  // Draw Text
  TextTransform = Concatenate(
    FSlateRenderTransform(FVector2D(0, 0)), // Translate to origin
    FSlateRenderTransform(FScale2f(1, 1), FVector2f(FVector2D(0, 0))),
    RollTransform,                            // Apply rotation
    FSlateRenderTransform(FVector2f(FVector2D(WidgetSize.X / 2, WidgetSize.Y / 2)))
  );

  auto Geometry = AllottedGeometry.ToPaintGeometry(
    FVector2D(1, 1),
    FSlateLayoutTransform(),
    TextTransform
  );

  // Main needle
  FSlateDrawElement::MakeLines(
    OutDrawElements,
    LayerId,
    Geometry,
    { FVector2D(0,Radius - Tick), FVector2D(0,-Radius) },
    ESlateDrawEffect::None,
    DrawColor,
    true,
    3.0f);

  // Back
  FSlateDrawElement::MakeLines(
    OutDrawElements,
    LayerId,
    Geometry,
    { FVector2D(-Tick,Radius), FVector2D(0,Radius - Tick), FVector2D(Tick,Radius) },
    ESlateDrawEffect::None,
    DrawColor,
    true,
    2.0f);

  // Front
  FSlateDrawElement::MakeLines(
    OutDrawElements,
    LayerId,
    Geometry,
    { FVector2D(-Tick,-Radius + Tick), FVector2D(Tick,-Radius + Tick) },
    ESlateDrawEffect::None,
    DrawColor,
    true,
    2.0f);
}
