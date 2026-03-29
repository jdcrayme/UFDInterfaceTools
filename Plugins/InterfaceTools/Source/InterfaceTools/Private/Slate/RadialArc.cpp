#include "RadialArc.h"
// ArcWidget.cpp
 
#include "RadialTicks.h"
#include "Math/UnrealMathUtility.h"
 
void SRadialArc::Construct(const FArguments& InArgs)
{
	StartAngleDeg = InArgs._StartAngleDeg;
	EndAngleDeg   = InArgs._EndAngleDeg;
	Radius        = InArgs._Radius;
	Thickness     = InArgs._Thickness;
	Color         = InArgs._Color;
	NumSegments   = InArgs._NumSegments;
}

void SRadialArc::SetStartAngle(float NewStartAngleDeg)
{
	if (!FMath::IsNearlyEqual(StartAngleDeg, NewStartAngleDeg))
	{
		StartAngleDeg = NewStartAngleDeg;
		Invalidate(EInvalidateWidget::Paint);  // Request repaint for the widget
	}
}

void SRadialArc::SetEndAngle(float NewEndAngleDeg)
{
	if (!FMath::IsNearlyEqual(EndAngleDeg, NewEndAngleDeg))
	{
		EndAngleDeg = NewEndAngleDeg;
		Invalidate(EInvalidateWidget::Paint);  // Request repaint for the widget
	}
}

void SRadialArc::SetRadius(float NewRadius)
{
	if (!FMath::IsNearlyEqual(Radius, NewRadius))
	{
		Radius = NewRadius;
		Invalidate(EInvalidateWidget::Paint);  // Request repaint for the widget
	}
}

void SRadialArc::SetThickness(float NewThickness)
{
	if (!FMath::IsNearlyEqual(Thickness, NewThickness))
	{
		Thickness = NewThickness;
		Invalidate(EInvalidateWidget::Paint);  // Request repaint for the widget
	}
}

void SRadialArc::SetColor(FLinearColor NewColor)
{
	if (Color !=NewColor)
	{
		Color = NewColor;
		Invalidate(EInvalidateWidget::Paint);  // Request repaint for the widget
	}
}

void SRadialArc::SetNumSegments(int32 NewNumSegments)
{
	if (NumSegments != NewNumSegments)
	{
		NumSegments = NewNumSegments;
		Invalidate(EInvalidateWidget::Paint);  // Request repaint for the widget
	}
}


FVector2D SRadialArc::ComputeDesiredSize(float) const
{
	// Size is diameter of the circle bounding the arc
	return FVector2D(Radius * 2.f, Radius * 2.f);
}
 
int32 SRadialArc::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
						  const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
						  int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	const FVector2D Center = AllottedGeometry.GetLocalSize() * 0.5f;
 
	// Convert angles from degrees to radians
	float StartRad = FMath::DegreesToRadians(StartAngleDeg);
	float EndRad = FMath::DegreesToRadians(EndAngleDeg);
 
	int32 Segments = FMath::Max(NumSegments, 2);
	TArray<FVector2D> Points;
	Points.Reserve(Segments + 1);
 
	// Generate points along the arc
	for (int32 i = 0; i <= Segments; ++i)
	{
		float Alpha = float(i) / Segments;
		float Angle = FMath::Lerp(StartRad, EndRad, Alpha);
		FVector2D Point = Center + FVector2D(FMath::Cos(Angle), FMath::Sin(Angle)) * Radius;
		Points.Add(Point);
	}
 
	// Draw arc as a polyline with thickness and color
	FSlateDrawElement::MakeLines(
		OutDrawElements,
		LayerId,
		AllottedGeometry.ToPaintGeometry(),
		Points,
		ESlateDrawEffect::None,
		Color,
		true, // Anti-alias
		Thickness
	);
 
	return LayerId + 1;
}