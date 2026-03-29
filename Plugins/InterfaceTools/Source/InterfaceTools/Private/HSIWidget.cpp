#include "HSIWidget.h"
#include "Slate/CompassCard.h"
#include "Slate/CompassPointer.h"
#include "Slate/CompassCoursePointer.h"
#include "Engine/Font.h"

UHSIWidget::UHSIWidget(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
  if (!IsRunningDedicatedServer())
  {
    static ConstructorHelpers::FObjectFinder<UFont> RobotoFontObj(*UWidget::GetDefaultFontName());
    Font = FSlateFontInfo(RobotoFontObj.Object, 24, FName("Bold"));
  }
}

void UHSIWidget::SetFont(FSlateFontInfo InFontInfo)
{
  Font = InFontInfo;
  RebuildWidget();
}

void UHSIWidget::SetHeading(float InHeading)
{
  if (CompassWidget.IsValid())
    CompassWidget->SetHeading(InHeading);

}

TSharedRef<SWidget> UHSIWidget::RebuildWidget()
{
  return SNew(SOverlay)
    // Base image
    + SOverlay::Slot()
    .HAlign(HAlign_Fill)
    .VAlign(VAlign_Fill)
    [
      SAssignNew(CompassWidget, SCompassCard)
        .Heading(TAttribute<float>::Create(TAttribute<float>::FGetter::CreateUObject(this, &UHSIWidget::GetHeading)))
        .Font(TAttribute<FSlateFontInfo>::Create(TAttribute<FSlateFontInfo>::FGetter::CreateUObject(this, &UHSIWidget::GetFont)))
        .Inset(TextHeight)
        .TickSize(TextHeight)
    ]
    + SOverlay::Slot()
    .HAlign(HAlign_Fill)
    .VAlign(VAlign_Fill)
    [
      SAssignNew(BearingPointerWidget, SCompassPointer)
        .Angle(TAttribute<float>::Create(TAttribute<float>::FGetter::CreateUObject(this, &UHSIWidget::GetBearingAngle)))
    ]
    + SOverlay::Slot()
    .HAlign(HAlign_Fill)
    .VAlign(VAlign_Fill)
    [
      SAssignNew(CoursePointerWidget, SCompassCoursePointer)
        .Angle(TAttribute<float>::Create(TAttribute<float>::FGetter::CreateUObject(this, &UHSIWidget::GetCourseAngle)))
        //.Deviation(TAttribute<float>::Create(TAttribute<float>::FGetter::CreateUObject(this, &UHSIWidget::GetDeviation)))
        .ToFlag(TAttribute<bool>::Create(TAttribute<bool>::FGetter::CreateUObject(this, &UHSIWidget::GetToFlag)))
        .NoSignalFlag(TAttribute<bool>::Create(TAttribute<bool>::FGetter::CreateUObject(this, &UHSIWidget::GetNoSignalFlag)))
    ];   // Overlay image

  UpdateFont();
}

void UHSIWidget::SynchronizeProperties()
{
  Super::SynchronizeProperties();

  // Update the Slate widget's font if it exists
  if (!CompassWidget.IsValid() || !BearingPointerWidget.IsValid() || !CoursePointerWidget.IsValid())
    RebuildWidget();
  
  UpdateFont();

  CompassWidget.Get()->SetHeading(Heading);
  CompassWidget.Get()->SetFont(Font);

  CoursePointerWidget->SetDeviation(Deviation);
  CoursePointerWidget->SetToFlag(ToFlag);
  CoursePointerWidget->SetNoSignalFlag(NoSignalFlag);

}

void UHSIWidget::UpdateFont()
{
  if (!CompassWidget.IsValid() || !BearingPointerWidget.IsValid())
    RebuildWidget();

  if (!Font.HasValidFont())
    Font = FAppStyle::GetFontStyle("NormalFont");

  const TSharedRef<FSlateFontMeasure>& FontMeasure = FSlateApplication::Get().GetRenderer()->GetFontMeasureService();

  FString TextToMeasure = "00";
  TextHeight = FontMeasure->Measure(TextToMeasure, Font).Y;

  CompassWidget.Get()->SetInset(TextHeight);
  CompassWidget.Get()->SetTickSize(TextHeight);

  BearingPointerWidget.Get()->SetInset(2*TextHeight);
  BearingPointerWidget.Get()->SetTickSize(TextHeight);

  CoursePointerWidget.Get()->SetInset(2 * TextHeight);
  CoursePointerWidget.Get()->SetTickSize(TextHeight);
}
