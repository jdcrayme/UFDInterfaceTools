#include "CompassCoursePointer.h"

#include "Fonts/FontMeasure.h"

SLATE_IMPLEMENT_WIDGET(SCompassCoursePointer)
void SCompassCoursePointer::PrivateRegisterAttributes(FSlateAttributeInitializer& AttributeInitializer)
{
  //SLATE_ADD_MEMBER_ATTRIBUTE_DEFINITION(AttributeInitializer, Font, EInvalidationReason::Paint);

}

void SCompassCoursePointer::Construct(const FArguments& InArgs)
{
  Angle.Assign(*this, InArgs._Angle);
  Inset.Assign(*this, InArgs._Inset);
  TickSize.Assign(*this, InArgs._TickSize);
  Color.Assign(*this, InArgs._Color);
}

void SCompassCoursePointer::SetAngle(float InAngle)
{
  Angle.Assign(*this, MoveTemp(InAngle));
}

void SCompassCoursePointer::SetInset(TAttribute<float> InInset)
{
  Inset.Assign(*this, MoveTemp(InInset));
}

void SCompassCoursePointer::SetTickSize(TAttribute<float> InTickSize)
{
  TickSize.Assign(*this, MoveTemp(InTickSize));
}

void SCompassCoursePointer::SetDeviation(TAttribute<float> InDeviation)
{
  Deviation.Assign(*this, MoveTemp(InDeviation));
}

void SCompassCoursePointer::SetToFlag(TAttribute<bool> InToFlag)
{
  ToFlag.Assign(*this, MoveTemp(InToFlag));
}

void SCompassCoursePointer::SetNoSignalFlag(TAttribute<bool> InNoSignalFlag)
{
  NoSignalFlag.Assign(*this, MoveTemp(InNoSignalFlag));
}

void SCompassCoursePointer::SetColor(TAttribute<FLinearColor> Incolor)
{
  Color.Assign(*this, MoveTemp(Incolor));
}


FVector2D SCompassCoursePointer::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
  return FVector2D(250,250);
}

int32 SCompassCoursePointer::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
  const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
  int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{

  DrawCompassPointer(AllottedGeometry, OutDrawElements, LayerId++);

  return LayerId;
}

void SCompassCoursePointer::DrawCompassPointer(const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements, int32 LayerId) const
{
  FVector2D Center = AllottedGeometry.GetLocalSize() * 0.5f;

  float Tick = TickSize.Get();
  float Radius = FMath::Min(Center.X, Center.Y) - Inset.Get();
  float HalfRadius = Radius / 2;
  float CurrentAngle = Angle.Get();

  float Deflection = Deviation.Get();

  FLinearColor DrawColor = Color.Get();
  FLinearColor SecondColor(0.8, 0.8, 0.8);

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

  // Main Front
  FSlateDrawElement::MakeLines(
    OutDrawElements,
    LayerId,
    Geometry,
    { FVector2D(0,-HalfRadius), FVector2D(0,-Radius+Tick) },
    ESlateDrawEffect::None,
    DrawColor,
    true,
    3.0f);

  // Draw dots for scale
  for (float i = -1; i <= 1; i+=0.5f)
  {
    FVector2D DotPos = FVector2D(i * HalfRadius, 0);

    FSlateDrawElement::MakeBox(
      OutDrawElements,
      LayerId,
      AllottedGeometry.ToPaintGeometry(
        FVector2D(1, 1),
        FSlateLayoutTransform(),
        Concatenate(
          FSlateRenderTransform(FVector2D(0, 0)), // Translate to origin
          FSlateRenderTransform(FScale2f(Tick/4, Tick/4), DotPos),
          RollTransform,                            // Apply rotation
          FSlateRenderTransform(FVector2f(FVector2D(WidgetSize.X / 2, WidgetSize.Y / 2)))
        )),
      FCoreStyle::Get().GetBrush("WhiteBrush"),
      ESlateDrawEffect::None,
      SecondColor
    );
  }

  // Deviation
  FSlateDrawElement::MakeLines(
    OutDrawElements,
    LayerId,
    Geometry,
    { FVector2D(Deflection* HalfRadius,HalfRadius), FVector2D(Deflection * HalfRadius,-HalfRadius) },
    ESlateDrawEffect::None,
    DrawColor,
    true,
    3.0f);

  if (ToFlag.Get())
  {
    // To indicator
    FSlateDrawElement::MakeLines(
      OutDrawElements,
      LayerId,
      Geometry,
      {
        FVector2D(-Tick,-HalfRadius + Tick / 2),
        FVector2D(0,-HalfRadius),
        FVector2D(Tick,-HalfRadius + Tick / 2),
        FVector2D(Tick,-HalfRadius + Tick),
        FVector2D(-Tick,-HalfRadius + Tick),
        FVector2D(-Tick,-HalfRadius + Tick / 2) },
        ESlateDrawEffect::None,
        SecondColor,
        true,
        2.0f);
  }
  else {
    // From indicator
    FSlateDrawElement::MakeLines(
      OutDrawElements,
      LayerId,
      Geometry,
      {
        FVector2D(-Tick,HalfRadius - Tick / 2),
        FVector2D(0,HalfRadius),
        FVector2D(Tick,HalfRadius - Tick / 2),
        FVector2D(Tick,HalfRadius - Tick),
        FVector2D(-Tick,HalfRadius - Tick),
        FVector2D(-Tick,HalfRadius - Tick / 2) },
        ESlateDrawEffect::None,
        SecondColor,
        true,
        2.0f);
  }

  // Main Back
  FSlateDrawElement::MakeLines(
    OutDrawElements,
    LayerId,
    Geometry,
    { FVector2D(0,Radius - Tick), FVector2D(0,HalfRadius) },
    ESlateDrawEffect::None,
    DrawColor,
    true,
    3.0f);

  // Arrowhead
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
    { FVector2D(-Tick,-Radius + Tick),FVector2D(0,-Radius), FVector2D(Tick,-Radius + Tick), FVector2D(-Tick,-Radius + Tick) },
    ESlateDrawEffect::None,
    DrawColor,
    true,
    2.0f);
}

