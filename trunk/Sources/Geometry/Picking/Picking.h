#pragma once

class UIPoint;
class UISegment;
interface IUIObject;

//------------------------------------------------------------------------------------------------------
UIPoint*   GetPickedPoint  (const CPoint& i_pt, IUIObject* ip_root);
UISegment* GetPickedSegment(const CPoint& i_pt, IUIObject* ip_root);
IUIObject* GetPickedObject(const CPoint& i_pt, IUIObject* ip_root);
