// This application is provided as is, without any support

#include "ADIWidget.h"
#include "Slate/ADI.h"

TSharedRef<SWidget> UADIWidget::RebuildWidget()
{
  return SNew(SADI)
    .Roll(TAttribute<float>::Create(TAttribute<float>::FGetter::CreateUObject(this, &UADIWidget::GetRoll)))
    .Pitch(TAttribute<float>::Create(TAttribute<float>::FGetter::CreateUObject(this, &UADIWidget::GetPitch)))
    .Slip(TAttribute<float>::Create(TAttribute<float>::FGetter::CreateUObject(this, &UADIWidget::GetSlip)));
}

void UADIWidget::SynchronizeProperties()
{
  Super::SynchronizeProperties();
}