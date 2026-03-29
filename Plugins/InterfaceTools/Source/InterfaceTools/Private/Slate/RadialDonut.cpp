#include "RadialDonut.h"

void SRadialDonut::Construct(const FArguments& InArgs)
{
	StartAngleDeg = InArgs._StartAngleDeg;
	EndAngleDeg = InArgs._EndAngleDeg;
	Radius = InArgs._Radius;
	Thickness = InArgs._Thickness;
	Color = InArgs._Color;
	NumSegments = InArgs._NumSegments;
}

void SRadialDonut::SetStartAngle(float NewStartAngleDeg)
{
	if (!FMath::IsNearlyEqual(StartAngleDeg, NewStartAngleDeg))
	{
		StartAngleDeg = NewStartAngleDeg;
		Invalidate(EInvalidateWidget::Paint);
	}
}

void SRadialDonut::SetEndAngle(float NewEndAngleDeg)
{
	if (!FMath::IsNearlyEqual(EndAngleDeg, NewEndAngleDeg))
	{
		EndAngleDeg = NewEndAngleDeg;
		Invalidate(EInvalidateWidget::Paint);
	}
}

void SRadialDonut::SetRadius(float NewRadius)
{
	if (!FMath::IsNearlyEqual(Radius, NewRadius))
	{
		Radius = NewRadius;
		Invalidate(EInvalidateWidget::Paint);
	}
}

void SRadialDonut::SetThickness(float NewThickness)
{
	if (!FMath::IsNearlyEqual(Thickness, NewThickness))
	{
		Thickness = NewThickness;
		Invalidate(EInvalidateWidget::Paint);
	}
}

void SRadialDonut::SetColor(FLinearColor NewColor)
{
	if (Color != NewColor)
	{
		Color = NewColor;
		Invalidate(EInvalidateWidget::Paint);
	}
}

void SRadialDonut::SetNumSegments(int32 NewNumSegments)
{
	if (NumSegments != NewNumSegments)
	{
		NumSegments = NewNumSegments;
		Invalidate(EInvalidateWidget::Paint);
	}
}

FVector2D SRadialDonut::ComputeDesiredSize(float) const
{
	return FVector2D(Radius * 2.f, Radius * 2.f);
}

int32 SRadialDonut::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
							const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
							int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	const FVector2D Center = AllottedGeometry.GetLocalSize() * 0.5f;

	float StartRad = FMath::DegreesToRadians(StartAngleDeg);
	float EndRad = FMath::DegreesToRadians(EndAngleDeg);

	int32 Segments = FMath::Max(NumSegments, 2);
	
	// Outer and inner radius calculation
	float OuterRadius = Radius;
	float InnerRadius = FMath::Max(OuterRadius - Thickness, 0.f);

	// Dont draw if inner and outer radius are too similar
	if (FMath::Abs(InnerRadius - OuterRadius) < 0.01)
		return LayerId;
	
	// Precompute points for outer and inner arcs
	TArray<FVector2D> OuterPoints;
	TArray<FVector2D> InnerPoints;
	OuterPoints.Reserve(Segments + 1);
	InnerPoints.Reserve(Segments + 1);

	for (int32 i = 0; i <= Segments; ++i)
	{
		float Alpha = float(i) / Segments;
		float Angle = FMath::Lerp(StartRad, EndRad, Alpha);
		FVector2D Dir(FMath::Cos(Angle), FMath::Sin(Angle));
		OuterPoints.Add(Center + Dir * OuterRadius);
		InnerPoints.Add(Center + Dir * InnerRadius);
	}

	// Prepare vertices and indices for triangulation
	TArray<FSlateVertex> Vertices;
	TArray<uint32> Indices;
	// Inside OnPaint():
	FSlateRenderTransform RenderTransform = AllottedGeometry.GetAccumulatedRenderTransform();
	FColor SlateColor = Color.ToFColor(true);
  
	// Helper lambda to convert FVector2D -> FVector2f
	auto ToFVector2f = [](const FVector2D& Vec) { return FVector2f(Vec.X, Vec.Y); };
 
	// Add outer points vertices
	for (const FVector2D& Pt : OuterPoints)
	{
		Vertices.Add(FSlateVertex::Make(
			RenderTransform,
			ToFVector2f(Pt),
			FVector2f(0.f, 0.f), // UV coords
			SlateColor,
			SlateColor
		));
	}
 
	// Add inner points vertices
	for (const FVector2D& Pt : InnerPoints)
	{
		Vertices.Add(FSlateVertex::Make(
			RenderTransform,
			ToFVector2f(Pt),
			FVector2f(0.f, 0.f),
			SlateColor,
			SlateColor
		));
	}
 
	// MakeCustomVerts call (9 arguments)
	FSlateDrawElement::MakeCustomVerts(
		OutDrawElements,
		LayerId,
		FSlateResourceHandle(), // No material
		Vertices,
		Indices,
		nullptr,     // No instance data
		0,           // Instance offset
		1,           // Num instances
		ESlateDrawEffect::None
	);

	return LayerId + 1;
}