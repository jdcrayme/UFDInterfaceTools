#include "ADI.h"
#include "SlateOptMacros.h"	
#include "Fonts/FontMeasure.h"

#include "Rendering/DrawElements.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SADI::Construct(const FArguments& InArgs)
{
  Roll = InArgs._Roll;
  Pitch = InArgs._Pitch;
  Slip = InArgs._Slip;
}

int32 SADI::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
  int32 CurrentLayer = LayerId;

  // Draw the components in order (background to foreground)
  DrawPitchScale(AllottedGeometry, OutDrawElements, CurrentLayer++);
  DrawRollScale(AllottedGeometry, OutDrawElements, CurrentLayer++);
  DrawRollPointer(AllottedGeometry, OutDrawElements, CurrentLayer++);
  DrawAircraftSymbol(AllottedGeometry, OutDrawElements, CurrentLayer++);
  //DrawSlipSkidIndicator(AllottedGeometry, OutDrawElements, CurrentLayer++);

  return CurrentLayer - 1;
}

const float WidgetDegreesVisible = 60;
void SADI::DrawPitchScale(const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements, int32 LayerId) const
{
  const FVector2D WidgetSize = AllottedGeometry.GetLocalSize();

  float CurrentRoll = Roll.Get(0.0f);
  float CurrentPitch = Pitch.Get(0.0f);

  float DegreesPerPixel = WidgetSize.Y / WidgetDegreesVisible;
  float Width = sqrt(WidgetSize.X * WidgetSize.X + WidgetSize.Y * WidgetSize.Y);

  // Limit pitch to +/- 45 degrees so we don't lose our sense of where up is.
  CurrentPitch = FMath::Clamp(CurrentPitch, -90, 90.0f);

  // Create a transform for roll (rotation around the center)
  FQuat2D RollQuat(FMath::DegreesToRadians(-CurrentRoll));
  FSlateRenderTransform RollTransform(RollQuat);
  FSlateRenderTransform PitchTransform = FSlateRenderTransform(FVector2D(0, CurrentPitch * DegreesPerPixel));
  FSlateRenderTransform OverallTransform = Concatenate(PitchTransform, RollTransform);

  float PixelHeight = DegreesPerPixel * (90 + WidgetDegreesVisible / 2);

  // Draw Sky
  FSlateRenderTransform SkyTransform = Concatenate(
    FSlateRenderTransform(FVector2D(0, 0)), // Translate to origin
    FSlateRenderTransform(FScale2f(Width, PixelHeight), FVector2f(FVector2D(0, -PixelHeight / 2))),
    OverallTransform,                            // Apply rotation
    FSlateRenderTransform(FVector2f(FVector2D(WidgetSize.X / 2, WidgetSize.Y / 2)))
  );

  FSlateDrawElement::MakeBox(
    OutDrawElements,
    LayerId,
    AllottedGeometry.ToPaintGeometry(
      FVector2D(1, 1),
      FSlateLayoutTransform(),
      SkyTransform
    ),
    FAppStyle::GetBrush("WhiteBrush"),
    ESlateDrawEffect::None,
    FLinearColor(0.0f, 0.5f, 1.0f) // Blue for sky
  );

  // Draw Ground
  FSlateRenderTransform GroundTransform = Concatenate(
    FSlateRenderTransform(FVector2D(0, 0)), // Translate to origin
    FSlateRenderTransform(FScale2f(Width, PixelHeight), FVector2f(FVector2D(0, PixelHeight / 2))),
    OverallTransform,                            // Apply rotation
    FSlateRenderTransform(FVector2f(FVector2D(WidgetSize.X / 2, WidgetSize.Y / 2)))
  );

  FSlateDrawElement::MakeBox(
    OutDrawElements,
    LayerId,
    AllottedGeometry.ToPaintGeometry(
      FVector2D(1, 1),
      FSlateLayoutTransform(),
      GroundTransform
    ),
    FAppStyle::GetBrush("WhiteBrush"),
    ESlateDrawEffect::None,
    FLinearColor(0.6f, 0.4f, 0.2f) // Brown for ground
  );

  FSlateRenderTransform CenteredTransform = Concatenate(
    FSlateRenderTransform(FVector2D(0, 0)), // Translate to origin
    OverallTransform,                            // Apply rotation
    FSlateRenderTransform(FVector2D(WidgetSize.X / 2, WidgetSize.Y / 2)) // Translate to origin
  );

  FPaintGeometry HorizonGeometry = AllottedGeometry.ToPaintGeometry(
    FVector2D(1, 1),
    FSlateLayoutTransform(),
    CenteredTransform
  );

  FSlateDrawElement::MakeLines(
    OutDrawElements,
    LayerId,
    HorizonGeometry,
    { FVector2D(-Width, 0) ,FVector2D(Width, 0) },
    ESlateDrawEffect::None,
    FLinearColor::White,
    true,
    2.0f
  );

  int32 CenterTick = FMath::RoundToInt(CurrentPitch / 5) * 5;

  auto Font = FAppStyle::GetFontStyle("NormalFont");
  Font.Size = 12;

  const TSharedRef<FSlateFontMeasure>& FontMeasure = FSlateApplication::Get().GetRenderer()->GetFontMeasureService();

  FString TextToMeasure = "00";
  FVector2D TextSize = FontMeasure->Measure(TextToMeasure, Font);

  // Draw pitch lines (5-degree increments)
  for (int32 PitchMark = CenterTick - WidgetDegreesVisible * 1 / 4; PitchMark <= CenterTick + WidgetDegreesVisible * 1 / 4; PitchMark += 5)
  {
    // Skip horizon line
    if (PitchMark == 0 || abs(PitchMark) > 90)
    {
      continue;
    }

    // The length of the mark varies depending on how large the interval is.
    const float LineWidth = (PitchMark % 10 == 0) ? WidgetSize.X * 0.1f : WidgetSize.X * 0.05f;
    FVector2D LeftStart = FVector2D(-LineWidth, -PitchMark * DegreesPerPixel);
    FVector2D RightEnd = FVector2D(LineWidth, -PitchMark * DegreesPerPixel);

    TArray<FVector2D> LinePoints = { LeftStart, RightEnd };

    FSlateDrawElement::MakeLines(
      OutDrawElements,
      LayerId,
      HorizonGeometry,
      LinePoints,
      ESlateDrawEffect::None,
      FLinearColor::White,
      true,
      1.0f
    );

    if (PitchMark % 10 == 0)
    {
      FSlateRenderTransform TextTransform;
      FPaintGeometry TextGeometry;

      // Draw Text
      TextTransform = Concatenate(
        FSlateRenderTransform(FVector2D(0, 0)), // Translate to origin
        FSlateRenderTransform(FScale2f(1, 1), FVector2f(-LineWidth - TextSize.X, -PitchMark * DegreesPerPixel - TextSize.Y / 2)),
        OverallTransform,                            // Apply rotation
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
        FString::Printf(TEXT("%d"), abs(PitchMark)),
        Font
      );


      TextTransform = Concatenate(
        FSlateRenderTransform(FVector2D(0, 0)), // Translate to origin
        FSlateRenderTransform(FScale2f(1, 1), FVector2f(LineWidth, -PitchMark * DegreesPerPixel - TextSize.Y / 2)),
        OverallTransform,                            // Apply rotation
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
        FString::Printf(TEXT("%d"), abs(PitchMark)),
        Font
      );
    }
  }
}

const float RollScaleRadius = 0.4f;
const float RollScaleMarkSize = 10;

void SADI::DrawRollScale(const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements, int32 LayerId) const
{
  const FVector2D WidgetSize = AllottedGeometry.GetLocalSize();
  const FVector2D Center = WidgetSize * 0.5f;
  const float Radius = WidgetSize.Y * RollScaleRadius;

  float MarkLength = RollScaleMarkSize;

  // Draw roll scale marks (0 to +/-90 degrees)
  for (int32 RollMark = -40; RollMark <= 40; RollMark += 10)
  {
    float Angle = FMath::DegreesToRadians(RollMark);

    FVector2D InnerPoint = Center + FVector2D(FMath::Sin(Angle) * Radius, -FMath::Cos(Angle) * Radius);
    FVector2D OuterPoint = Center + FVector2D(FMath::Sin(Angle) * (Radius + MarkLength), -FMath::Cos(Angle) * (Radius + MarkLength));

    FSlateDrawElement::MakeLines(
      OutDrawElements,
      LayerId,
      AllottedGeometry.ToPaintGeometry(),
      TArray<FVector2D>{InnerPoint, OuterPoint},
      ESlateDrawEffect::None,
      FLinearColor::White,
      true,
      2.0f
    );
  }

  MarkLength *= 2;

  // Draw roll scale marks (0 to +/-90 degrees)
  for (int32 RollMark = -90; RollMark <= 90; RollMark += 45)
  {
    float Angle = FMath::DegreesToRadians(RollMark);

    FVector2D InnerPoint = Center + FVector2D(FMath::Sin(Angle) * Radius, -FMath::Cos(Angle) * Radius);
    FVector2D OuterPoint = Center + FVector2D(FMath::Sin(Angle) * (Radius + MarkLength), -FMath::Cos(Angle) * (Radius + MarkLength));

    FSlateDrawElement::MakeLines(
      OutDrawElements,
      LayerId,
      AllottedGeometry.ToPaintGeometry(),
      TArray<FVector2D>{InnerPoint, OuterPoint},
      ESlateDrawEffect::None,
      FLinearColor::White,
      true,
      3.0f
    );
  }
}

void SADI::DrawRollPointer(const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements, int32 LayerId) const
{
  const FVector2D WidgetSize = AllottedGeometry.GetLocalSize();
  const FVector2D Center = WidgetSize * 0.5f;
  const float Radius = WidgetSize.Y * RollScaleRadius;

  float CurrentRoll = Roll.Get(0.0f);
  float Angle = FMath::DegreesToRadians(-CurrentRoll);

  // Draw a triangle pointer at the top of the roll scale.
  FVector2D Tip = Center + FVector2D(FMath::Sin(Angle) * (Radius), -FMath::Cos(Angle) * (Radius));

  FVector2D Edge = FVector2D(FMath::Cos(Angle) * RollScaleMarkSize, FMath::Sin(Angle) * RollScaleMarkSize);
  FVector2D Inset = FVector2D(FMath::Sin(Angle) * (RollScaleMarkSize), -FMath::Cos(Angle) * (RollScaleMarkSize));

  TArray<FSlateVertex> verts;
  verts.Reserve(3);
  verts.AddDefaulted(3);

  verts[0].Position = FVector2f(AllottedGeometry.LocalToAbsolute(Tip));
  verts[0].Color = FLinearColor::White.ToFColor(false); 

  verts[1].Position = FVector2f(AllottedGeometry.LocalToAbsolute(Tip + Edge - Inset));
  verts[1].Color = FLinearColor::White.ToFColor(false);

  verts[2].Position = FVector2f(AllottedGeometry.LocalToAbsolute(Tip - Edge - Inset));
  verts[2].Color = FLinearColor::White.ToFColor(false);

  TArray<SlateIndex> indicies{0,1,2};

  FSlateResourceHandle ResourceHandle = FSlateApplication::Get().GetRenderer()->GetResourceHandle(*FAppStyle::GetBrush("WhiteBrush"));

  FSlateDrawElement::MakeCustomVerts(
    OutDrawElements,
    LayerId,
    ResourceHandle,
    verts,
    indicies,
    nullptr,
    0,
    0, ESlateDrawEffect::None);
}

void SADI::DrawAircraftSymbol(const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements, int32 LayerId) const
{
  const FVector2D WidgetSize = AllottedGeometry.GetLocalSize();
  const FVector2D Center = WidgetSize * 0.5f;

  const float BorderThickness = 7;
  const FLinearColor BorderColor = FLinearColor::Yellow;

  const float CenterThickness = 4;
  const FLinearColor CenterColor = FLinearColor::Black;

  const float AircraftSize = WidgetSize.X * 0.1f;

  float WingOutside = -AircraftSize;
  float WingInside = -AircraftSize/3;

  float Top = 0;
  float Bottom = AircraftSize/3;

  FVector2D W0 = FVector2D(WingOutside, Top);
  FVector2D W1 = FVector2D(WingInside, Top);

  // Draw a simple aircraft symbol (triangle with wings)

  FSlateDrawElement::MakeLines(
    OutDrawElements,
    LayerId,
    AllottedGeometry.ToPaintGeometry(),
    TArray<FVector2D>{Center + W0, Center + W1, Center + FVector2D(WingInside, Bottom) },
    ESlateDrawEffect::None,
    BorderColor,
    true,
    BorderThickness
  );

  FSlateDrawElement::MakeLines(
    OutDrawElements,
    LayerId,
    AllottedGeometry.ToPaintGeometry(),
    TArray<FVector2D>{Center + W0, Center + W1, Center + FVector2D(WingInside, Bottom) },
    ESlateDrawEffect::None,
    CenterColor,
    true,
    CenterThickness
  );

  FSlateDrawElement::MakeLines(
    OutDrawElements,
    LayerId,
    AllottedGeometry.ToPaintGeometry(),
    TArray<FVector2D>{Center - W0, Center - W1, Center + FVector2D(-WingInside, Bottom) },
    ESlateDrawEffect::None,
    BorderColor,
    true,
    BorderThickness
  );

  FSlateDrawElement::MakeLines(
    OutDrawElements,
    LayerId,
    AllottedGeometry.ToPaintGeometry(),
    TArray<FVector2D>{Center - W0, Center - W1, Center + FVector2D(-WingInside, Bottom) },
    ESlateDrawEffect::None,
    CenterColor,
    true,
    CenterThickness
  );

  FSlateDrawElement::MakeLines(
    OutDrawElements,
    LayerId,
    AllottedGeometry.ToPaintGeometry(),
    TArray<FVector2D>{Center + FVector2D(-WingInside / 4, Top), Center + FVector2D(WingInside / 4, Top)},
    ESlateDrawEffect::None,
    BorderColor,
    true,
    BorderThickness
  );

  FSlateDrawElement::MakeLines(
    OutDrawElements,
    LayerId,
    AllottedGeometry.ToPaintGeometry(),
    TArray<FVector2D>{Center + FVector2D(-WingInside / 4, Top), Center + FVector2D(WingInside / 4, Top)},
    ESlateDrawEffect::None,
    CenterColor,
    true,
    CenterThickness
  );
}

void SADI::DrawSlipSkidIndicator(const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements, int32 LayerId) const
{
  const FVector2D WidgetSize = AllottedGeometry.GetLocalSize();
  const FVector2D Center = WidgetSize * 0.5f;

  float CurrentSlip = Slip.Get(0.0f);
  float MaxOffset = WidgetSize.X * 0.25f; // Max slip offset
  float SlipOffset = CurrentSlip * MaxOffset;

  FVector2D SlipCenter = Center + FVector2D(SlipOffset, WidgetSize.Y * 0.1f);
  FVector2D LeftEnd = Center + FVector2D(-WidgetSize.X * 0.25f, WidgetSize.Y * 0.1f);
  FVector2D RightEnd = Center + FVector2D(WidgetSize.X * 0.25f, WidgetSize.Y * 0.1f);

  FSlateDrawElement::MakeLines(
    OutDrawElements,
    LayerId,
    AllottedGeometry.ToPaintGeometry(),
    TArray<FVector2D>{LeftEnd, RightEnd},
    ESlateDrawEffect::None,
    FLinearColor::White,
    true,
    1.0f
  );

  FVector2D SlipSize = FVector2D(10.0f, 5.0f);

  FSlateDrawElement::MakeBox(
    OutDrawElements,
    LayerId,
    AllottedGeometry.ToPaintGeometry(
        FVector2f(SlipSize), 
        FSlateLayoutTransform(FVector2f(SlipCenter - SlipSize * 0.5f))),
    FAppStyle::GetBrush("WhiteBrush"),
    ESlateDrawEffect::None,
    FLinearColor::White
  );
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION