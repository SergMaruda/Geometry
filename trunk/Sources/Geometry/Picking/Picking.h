#pragma once
#include <memory>
#include "..\Primitives\IUIObject.h"

class UIPoint;
class UISegment;
interface IUIObject;

//------------------------------------------------------------------------------------------------------
UIPoint*      GetPickedPoint  (const CPoint& i_pt, TIUIObjectPtr ip_root);
UISegment*    GetPickedSegment (const CPoint& i_pt, TIUIObjectPtr ip_root);
TIUIObjectPtr GetPickedObject  (const CPoint& i_pt, TIUIObjectPtr ip_root);
