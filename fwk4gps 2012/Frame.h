#ifndef _FRAME_H_
#define _FRAME_H_

/* Frame Definition - Modelling Layer
 *
 * Frame.h
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "iFrame.h"           // for the Frame Interface
#include "MathDeclarations.h" // for Matrix

//-------------------------------- Frame --------------------------------------
//
// The Frame class defines a reference frame in the Modelling Layer
//
class Frame : public iFrame {

    Matrix T;       // relative transformation wrt parent frame or world space
    iFrame* parent; // points to parent frame, if any
    Quaternion Q;

  public:
   Frame();
   void   scale(float sx, float sy, float sz)      { T.scale(sx, sy, sz); }
   void   rotatex(float rad)                       { rotate(Vector(1, 0, 0), rad); }
   void   rotatey(float rad)                       { rotate(Vector(0, 1, 0), rad); }
   void   rotatez(float rad)                       { rotate(Vector(0, 0, 1), rad); }
   void   rotate(const Vector& axis, float rad)    { Q = Q * Quaternion(axis, rad); }
   void   translate(float x, float y, float z)     { T.translate(x, y, z); }
   void   orient(const Matrix& rot)                { quaternion().getRotationMatrix().orient(rot); }
   Quaternion quaternion() const                   { return parent ? parent->quaternion() * Q : Q; }
   Vector position() const;
   Matrix rotation() const;
   Vector orientation(const Vector& v) const;
   Vector orientation(char c) const;
   Matrix world() const;
   void   attachTo(iFrame* newParent);

    virtual ~Frame() {}
};

//-------------------------------- Shape ----------------------------
//
// A Shape is a Frame that has a Boundary
//
class Shape : public Frame, public iShape {

    bool   sphere;
    bool   plane;
    bool   axisAligned;
    float  radius;
    Vector normal;
    Vector minimum;
    Vector maximum;

public:
    Shape() : sphere(false), plane(false), axisAligned(false),
     radius(0) {}
    void  setRadius(float r);
    void  setRadius(float x, float y, float z);
    float getRadius() const { return radius; }
    void  setPlane(Vector n, float d);
    void  setAxisAligned(Vector min, Vector max);
    friend bool collision(const Shape* f1, const Shape* f2, Vector& d);
};

#endif