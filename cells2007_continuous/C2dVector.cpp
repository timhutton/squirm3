// --- C2dVector.cpp -------------------------------------------

// local:
#include "C2dVector.h"

// stdlib:
#include <math.h>

// default constructor
C2dVector::C2dVector(float a,float b) 
: x(a), y(b) {}

// alternative constructors
C2dVector::C2dVector(int a,int b) 
: x((float)a), y((float)b) {}

// copy constructor
C2dVector::C2dVector(const C2dVector& v) 
: x(v.x), y(v.y) {}

// overloaded unary operators
C2dVector& C2dVector::operator=(const C2dVector& v) 
{ 
    x=v.x; 
    y=v.y;
    return(*this); 
}
C2dVector& C2dVector::operator+=(const C2dVector& v) 
{ 
    x+=v.x; 
    y+=v.y;
    return(*this); 
}
C2dVector& C2dVector::operator-=(const C2dVector& v) 
{ 
    x-=v.x; 
    y-=v.y;
    return(*this); 
}
C2dVector& C2dVector::operator*=(float f) 
{ 
    x*=f; 
    y*=f;
    return(*this); 
}
C2dVector& C2dVector::operator/=(float f) 
{ 
    x/=f; 
    y/=f;
    return(*this); 
}
C2dVector C2dVector::operator-()
{
    x*=-1; 
    y*=-1;
    return *this;
}

// overloaded binary operators
C2dVector operator*(const C2dVector& a,float f)
{ 
    return C2dVector(a.x*f,a.y*f); 
}
C2dVector operator*(float f,const C2dVector& a)
{ 
    return C2dVector(a*f);  // call other multiplication function
}
C2dVector operator/(const C2dVector& a,float f)
{ 
    return C2dVector(a.x/f,a.y/f); 
}
C2dVector operator/(float f,const C2dVector& a)
{ 
    return C2dVector(a/f);  // call other divide function
}
C2dVector operator+(const C2dVector& a,const C2dVector& b)
{ 
    return C2dVector(a.x+b.x,a.y+b.y); 
}
C2dVector operator-(const C2dVector& a,const C2dVector& b)
{ 
    return C2dVector(a.x-b.x,a.y-b.y); 
}
float Length(const C2dVector& v)
{
    return((float)sqrt(v.x*v.x+v.y*v.y));
}
float Length2(const C2dVector& v)
{
    return(v.x*v.x+v.y*v.y);
}
float DotProduct(const C2dVector& a,const C2dVector& b)
{
    return(a.x*b.x + a.y*b.y);
}
C2dVector C2dVector::Normalize()
{
    *this/=Length(*this);
    return *this;
}
void C2dVector::RotateAround(const C2dVector& c,float deg)
{
    // find sine and cosine of the angle
    float radians = deg*3.1415926535F/180.0F;
    float sine = (float)sin(radians);
    float cosine = (float)cos(radians);
    
    // rotate around the point
    C2dVector new_pos = *this - c;
    float temp = cosine*new_pos.x - sine*new_pos.y;
    new_pos.y = sine*new_pos.x + cosine*new_pos.y;
    new_pos.x = temp;
    *this = c + new_pos;
}
float C2dVector::Dist2(const C2dVector& a,const C2dVector& b)
{
    return (a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y);
}

//-----------------------------------------------------------
