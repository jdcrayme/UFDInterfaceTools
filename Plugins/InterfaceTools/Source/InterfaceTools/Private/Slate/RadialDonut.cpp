#include "RadialDonut.h"

void SRadialDonut::Construct(const FArguments& InArgs)
{
	StartAngleDeg = InArgs._StartAngleDeg;
	EndAngleDeg = InArgs._EndAngleDeg;
	InnerRadius = InArgs._InnerRadius;
	OuterRadius = InArgs._OuterRadius;
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

void SRadialDonut::SetInnerRadius(float NewRadius)
{
	if (!FMath::IsNearlyEqual(InnerRadius, NewRadius))
	{
		InnerRadius = NewRadius;
		Invalidate(EInvalidateWidget::Paint);
	}
}

void SRadialDonut::SetOuterRadius(float NewRadius)
{
	if (!FMath::IsNearlyEqual(OuterRadius, NewRadius))
	{
		OuterRadius = NewRadius;
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
	float radius = FMath::Max(InnerRadius,OuterRadius);
	return FVector2D(radius * 2.f, radius * 2.f);
}

int32 SRadialDonut::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
    const FLinearColor ArcColor = FLinearColor::Red;
 
    // 1. Prepare vertex and index arrays
    TArray<FSlateVertex> Vertices;
    TArray<SlateIndex> Indices;
 
    // Total points = (Segments + 1) * 2
    Vertices.Reserve((NumSegments + 1) * 2);
    Indices.Reserve(NumSegments * 6);
 
    const float RadiansStart = FMath::DegreesToRadians(StartAngleDeg);
    const float RadiansEnd = FMath::DegreesToRadians(EndAngleDeg);
    const FVector2D Center = AllottedGeometry.GetLocalSize() * 0.5f;
 
    for (int32 i = 0; i <= NumSegments; ++i)
    {
        float CurrentAngle = FMath::Lerp(RadiansStart, RadiansEnd, static_cast<float>(i) / NumSegments);
        float SinAngle, CosAngle;
        FMath::SinCos(&SinAngle, &CosAngle, CurrentAngle);
 
        // Vector pointing from center to the arc edge
        FVector2D Direction(CosAngle, SinAngle);
 
        // Create Outer Vertex
        FVector2D OuterPos = Center + (Direction * OuterRadius);
        Vertices.Add(FSlateVertex::Make<ESlateVertexRounding::Disabled>(
            AllottedGeometry.GetAccumulatedRenderTransform(), 
            FVector2f(OuterPos), 
            FVector2f::ZeroVector, 
            ArcColor.ToFColor(true)));
 
        // Create Inner Vertex
        FVector2D InnerPos = Center + (Direction * InnerRadius);
        Vertices.Add(FSlateVertex::Make<ESlateVertexRounding::Disabled>(
            AllottedGeometry.GetAccumulatedRenderTransform(), 
            FVector2f(InnerPos), 
            FVector2f::ZeroVector, 
            ArcColor.ToFColor(true)));
 
        // Define Triangles (except for the last point)
        if (i < NumSegments)
        {
            uint32 BaseIdx = i * 2;
            // Triangle 1
            Indices.Add(BaseIdx);
            Indices.Add(BaseIdx + 1);
            Indices.Add(BaseIdx + 2);
            // Triangle 2
            Indices.Add(BaseIdx + 2);
            Indices.Add(BaseIdx + 1);
            Indices.Add(BaseIdx + 3);
        }
    }
 
    // 2. Create the Custom Vert Draw Element
    FSlateDrawElement::MakeCustomVerts(
        OutDrawElements,
        LayerId,
        FSlateResourceHandle(), // Use a white square texture for solid color
        Vertices,
        Indices,
        nullptr,
        0,
        0
    );
 
    return LayerId;
}