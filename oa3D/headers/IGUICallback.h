#ifndef IGUI_CALLBACK_H
#define IGUI_CALLBACK_H


class IGUICallback
{

public:
    virtual void onUpdatePositionX(float x) = 0;
    virtual void onUpdatePositionY(float y) = 0;
    virtual void onUpdatePositionZ(float z) = 0;

    virtual void onUpdateScaleX(float x) = 0;
    virtual void onUpdateScaleY(float y) = 0;
    virtual void onUpdateScaleZ(float z) = 0;
    virtual void onUpdateScale(float xyz) = 0;

    virtual void onUpdateRotationX(float x) = 0;
    virtual void onUpdateRotationY(float y) = 0;
    virtual void onUpdateRotationZ(float z) = 0;
    virtual void onUpdateRotationAngle(float angle) = 0;
};




#endif
