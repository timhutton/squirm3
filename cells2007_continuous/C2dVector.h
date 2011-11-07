// --- C2dVector.h ----------------------------------------------

#pragma once

#if !defined(C2DVECTOR)
#define C2DVECTOR 1

/// A simple class to manage a 2D point/vector, with coordinates (x,y)
class C2dVector
{
    public:

        // data members
        float x,y;

    public:

        // default constructor
        C2dVector(float a=0.0F,float b=0.0F);

        // alternative constructors
        C2dVector(int a,int b);

        // copy constructor
        C2dVector(const C2dVector& v);

        // overloaded unary operators
        C2dVector& operator=(const C2dVector& v);
        C2dVector& operator+=(const C2dVector& v);
        C2dVector& operator-=(const C2dVector& v);
        C2dVector& operator*=(float f);
        C2dVector& operator/=(float f);
        bool operator==(const C2dVector& v) const
            { return((x==v.x)&&(y==v.y)); }
        bool operator!=(const C2dVector& v) const
            { return((x!=v.x)||(y!=v.y)); }
        C2dVector operator-();

        void RotateAround(const C2dVector& c,float deg);
        C2dVector Normalize();

        // some operators encoded for speed
        void Mult(float f)
        {
            this->x *= f;
            this->y *= f;
        }
        void Add(const C2dVector& b)
        {
            this->x += b.x;
            this->y += b.y;
        }
        void Subtract(const C2dVector& b)
        {
            this->x -= b.x;
            this->y -= b.y;
        }
        static float Dist2(const C2dVector& a,const C2dVector& b);

        // overloaded binary operators
        friend C2dVector operator*(const C2dVector& a,float f);
        friend C2dVector operator*(float f,const C2dVector& a);
        friend C2dVector operator/(const C2dVector& a,float f);
        friend C2dVector operator/(float f,const C2dVector& a);
        friend C2dVector operator+(const C2dVector& a,const C2dVector& b);
        friend C2dVector operator-(const C2dVector& a,const C2dVector& b);
        friend float Length(const C2dVector& v);
        friend float Length2(const C2dVector& v);
        friend float DotProduct(const C2dVector& a,const C2dVector& b);
        friend C2dVector Normalized(const C2dVector& a);
};

#endif

//-----------------------------------------------------------
