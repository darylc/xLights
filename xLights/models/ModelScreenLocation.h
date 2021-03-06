#ifndef MODELSCREENLOCATION_H
#define MODELSCREENLOCATION_H


#define OVER_NO_HANDLE              -1
#define OVER_L_TOP_HANDLE           1
#define OVER_R_TOP_HANDLE           2
#define OVER_L_BOTTOM_HANDLE        3
#define OVER_R_BOTTOM_HANDLE        4
#define OVER_ROTATE_HANDLE          5

class wxXmlNode;
class ModelPreview;
class wxPropertyGridInterface;
class wxPropertyGridEvent;
class wxCursor;

#include <vector>
#include "Node.h"
#include <glm/mat3x3.hpp>

class ModelScreenLocation
{
public:
    
    virtual void Read(wxXmlNode *node) = 0;
    virtual void Write(wxXmlNode *node) = 0;
    
    virtual void SetPreviewSize(int w, int h, const std::vector<NodeBaseClassPtr> &Nodes) = 0;
    virtual void PrepareToDraw() const = 0;
    virtual void TranslatePoint(double &x, double &y) const = 0;

    virtual bool IsContained(int x1, int y1, int x2, int y2) const = 0;
    virtual bool HitTest(int x,int y) const = 0;
    virtual wxCursor CheckIfOverHandles(int &handle, int x, int y) const = 0;
    virtual void DrawHandles() const = 0;
    virtual int MoveHandle(ModelPreview* preview, int handle, bool ShiftKeyPressed, int mouseX, int mouseY) = 0;
    virtual wxCursor InitializeLocation(int &handle, int x, int y, const std::vector<NodeBaseClassPtr> &Nodes) = 0;
 
    virtual void AddSizeLocationProperties(wxPropertyGridInterface *grid) const = 0;
    virtual int OnPropertyGridChange(wxPropertyGridInterface *grid, wxPropertyGridEvent& event) = 0;
    virtual bool IsCenterBased() const = 0;
    virtual float GetVScaleFactor() const {return 1.0;}
    
    virtual void SetOffset(double xPct, double yPct) = 0;
    virtual void AddOffset(double xPct, double yPct) = 0;
    
    virtual float GetHcenterOffset() const = 0;
    virtual float GetVcenterOffset() const = 0;
    virtual void SetHcenterOffset(float f) = 0;
    virtual void SetVcenterOffset(float f) = 0;
    
    //in screen coordinates
    virtual int GetTop() const = 0;
    virtual int GetLeft() const = 0;
    virtual int GetRight() const = 0;
    virtual int GetBottom() const = 0;
    virtual void SetTop(int i) = 0;
    virtual void SetLeft(int i) = 0;
    virtual void SetRight(int i) = 0;
    virtual void SetBottom(int i) = 0;
    
    
    void SetRenderSize(float NewWi, float NewHt) {
        RenderHt=NewHt;
        RenderWi=NewWi;
    }
    float RenderHt,RenderWi;  // size of the rendered output
    int previewW,previewH;
    
protected:
    ModelScreenLocation(int points);
    virtual ~ModelScreenLocation() {};

    struct xlPoint {
        int x;
        int y;
    };
    mutable std::vector<xlPoint> mHandlePosition;
};

//Default location that uses a bounding box - 4 corners and a rotate handle
class BoxedScreenLocation : public ModelScreenLocation {
public:
    BoxedScreenLocation();
    virtual ~BoxedScreenLocation() {}
    
    virtual void Read(wxXmlNode *node) override;
    virtual void Write(wxXmlNode *node) override;
    
    virtual void SetPreviewSize(int w, int h, const std::vector<NodeBaseClassPtr> &Nodes) override;
    virtual void PrepareToDraw() const override;
    virtual void TranslatePoint(double &x, double &y) const override;
    
    virtual bool IsContained(int x1, int y1, int x2, int y2) const override;
    virtual bool HitTest(int x,int y) const override;
    virtual wxCursor CheckIfOverHandles(int &handle, int x, int y) const override;
    virtual void DrawHandles() const override;
    virtual int MoveHandle(ModelPreview* preview, int handle, bool ShiftKeyPressed, int mouseX, int mouseY) override;
    virtual wxCursor InitializeLocation(int &handle, int x, int y, const std::vector<NodeBaseClassPtr> &Nodes) override;
    
    virtual void AddSizeLocationProperties(wxPropertyGridInterface *grid) const override;
    virtual int OnPropertyGridChange(wxPropertyGridInterface *grid, wxPropertyGridEvent& event) override;
    virtual bool IsCenterBased() const override {return true;};

    virtual float GetHcenterOffset() const override {
        return offsetXpct;
    }
    virtual float GetVcenterOffset() const override {
        return offsetYpct;
    }
    virtual void SetHcenterOffset(float f) override {
        offsetXpct = f;
    }
    virtual void SetVcenterOffset(float f) override {
        offsetYpct = f;
    }

    virtual void SetOffset(double xPct, double yPct) override {
        offsetXpct=xPct;
        offsetYpct=yPct;
    }
    virtual void AddOffset(double xPct, double yPct) override {
        offsetXpct+=xPct;
        offsetYpct+=yPct;
    }
    void SetScale(double x, double y) {
        PreviewScaleX = x;
        PreviewScaleY = y;
        singleScale = false;
    }
    void SetRotation(int r) {
        PreviewRotation = r;
    }
    
    virtual int GetTop() const override {
        return mMaxScreenY;
    }
    virtual int GetLeft() const override {
        return mMinScreenX;
    }
    virtual int GetRight() const override {
        return mMaxScreenX;
    }
    virtual int GetBottom() const override {
        return mMinScreenY;
    }
    virtual void SetTop(int i) override;
    virtual void SetLeft(int i) override;
    virtual void SetRight(int i) override;
    virtual void SetBottom(int i) override;
    
private:
    double offsetXpct,offsetYpct;
    bool singleScale;
    double PreviewScaleX, PreviewScaleY;
    int PreviewRotation;
    
    mutable double radians;
    mutable double scalex;
    mutable double scaley;
    mutable double centerx;
    mutable double centery;
    
    int mMinScreenX;
    int mMinScreenY;
    int mMaxScreenX;
    int mMaxScreenY;
};


//Location that uses two points to define start/end
class TwoPointScreenLocation : public ModelScreenLocation {
public:
    TwoPointScreenLocation();
    virtual ~TwoPointScreenLocation();
    
    virtual void Read(wxXmlNode *node) override;
    virtual void Write(wxXmlNode *node) override;
    
    virtual void SetPreviewSize(int w, int h, const std::vector<NodeBaseClassPtr> &Nodes) override;
    virtual void PrepareToDraw() const override;
    virtual void TranslatePoint(double &x, double &y) const override;
    
    virtual bool IsContained(int x1, int y1, int x2, int y2) const override;
    virtual bool HitTest(int x,int y) const override;
    virtual wxCursor CheckIfOverHandles(int &handle, int x, int y) const override;
    virtual void DrawHandles() const override;
    virtual int MoveHandle(ModelPreview* preview, int handle, bool ShiftKeyPressed, int mouseX, int mouseY) override;
    virtual wxCursor InitializeLocation(int &handle, int x, int y, const std::vector<NodeBaseClassPtr> &Nodes) override;

    virtual void AddSizeLocationProperties(wxPropertyGridInterface *grid) const override;
    virtual int OnPropertyGridChange(wxPropertyGridInterface *grid, wxPropertyGridEvent& event) override;
    
    virtual float GetHcenterOffset() const override;
    virtual float GetVcenterOffset() const override;
    virtual void SetHcenterOffset(float f) override;
    virtual void SetVcenterOffset(float f) override;
    virtual bool IsCenterBased() const override {return false;};

    virtual void SetOffset(double xPct, double yPct) override;
    virtual void AddOffset(double xPct, double yPct) override;
    virtual int GetTop() const override;
    virtual int GetLeft() const override;
    virtual int GetRight() const override;
    virtual int GetBottom() const override;
    virtual void SetTop(int i) override;
    virtual void SetLeft(int i) override;
    virtual void SetRight(int i) override;
    virtual void SetBottom(int i) override;
    
    virtual float GetYShear() const {return 0.0;}
    void SetYMinMax(float min, float max) {
        minMaxSet = true;
        ymin = min;
        ymax = max;
    }
    void FlipCoords();

protected:
    virtual void ProcessOldNode(wxXmlNode *n);
    
    float x1, y1;
    float x2, y2;
    float ymin, ymax;
    bool minMaxSet;
    
    wxXmlNode *old;
    mutable glm::mat3 *matrix;
};


class ThreePointScreenLocation : public TwoPointScreenLocation {
public:
    ThreePointScreenLocation();
    virtual ~ThreePointScreenLocation();
    virtual void Read(wxXmlNode *node) override;
    virtual void Write(wxXmlNode *node) override;
    
    virtual void AddSizeLocationProperties(wxPropertyGridInterface *grid) const override;
    virtual int OnPropertyGridChange(wxPropertyGridInterface *grid, wxPropertyGridEvent& event) override;
    
    virtual void DrawHandles() const override;
    virtual int MoveHandle(ModelPreview* preview, int handle, bool ShiftKeyPressed, int mouseX, int mouseY) override;
    virtual float GetVScaleFactor() const override;
    virtual float GetYShear() const override;

    void SetHeight(float h) {
        height = h;
    }
    float GetHeight() const {
        return height;
    }
    void SetModelHandleHeight(bool b) {
        modelHandlesHeight = b;
    }
    void SetSupportsShear(bool b) {
        supportsShear = b;
    }
    void SetSupportsAngle(bool b) {
        supportsAngle = b;
    }
    void SetAngle(int a) {
        angle = a;
    }
    int GetAngle() const {
        return angle;
    }
protected:
    virtual void ProcessOldNode(wxXmlNode *n) override;
private:
    bool modelHandlesHeight;
    bool supportsAngle;
    bool supportsShear;
    float height;
    int angle;
    float shear;
};

#endif // MODELSCREENLOCATION_H

