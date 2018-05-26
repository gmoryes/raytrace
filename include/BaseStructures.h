#ifndef MASHGRAPH3_BASESTRUCTURES_H
#define MASHGRAPH3_BASESTRUCTURES_H

#include "EasyBMP.h"

#define INF 1e20
#define EPS 1e-4
#define GRADIENT_EPS 1
#define MAX_TRACE_STEPS_COUNT 20000
#define MAX_REFLECTIONS 5

/**
 * This file defines base structures at this project
 */

/**
 * The struct represent math Vector
 */
struct Vector {
    Vector(): x(0), y(0), z(0) {}
    Vector(float x, float y, float z): x(x), y(y), z(z) {}

    /**
     * Multiply vector by number
     * @param k - number
     * @return - new Vector
     */
    Vector operator* (float k) const;

    /**
     * Divide Vector by number
     * @param k - number
     * @return - new Vector
     */
    Vector operator/ (float k) const;

    /**
     * Sum of two Vectors
     * @param v - Vector, that we want to sum
     * @return - new Vector
     */
    Vector operator+ (const Vector &v) const;

    /**
     * Subtract Vector from Vector
     * @param v - Vector, that we want to subtract
     * @return - new Vector
     */
    Vector operator- (const Vector &v) const;

    /**
     * Vector opposite of current
     * @return new Vector
     */
    Vector operator- () const;

    /**
     * Normalize Vector
     * @return - new normalize Vector
     */
    void normalize();

    /**
     * Vector length
     */
    float length() const;

    /**
     * Cosinus of angle between two Vectors
     * @param vector - Second Vector
     * @return angle cos
     */
    float GetCosAngleWith(const Vector &vector) const;

    /**
     * Reflect Vector
     * @param ray - the Vector, that we want to reflect
     * @param norm - The normal about which we want to reflect
     * @return - new reflect Vector
     */
    static Vector reflect(const Vector &ray, const Vector &norm);

    /**
     * Refract Vector
     * @param ray - the Vector, that we want to refract
     * @param norm - The normal about which we want to refract
     * @param eta - refraction coefficient
     * @return - new refract Vector
     */
    static Vector refract(const Vector &ray, Vector norm, float eta);

    /**
     * Scalar multiplication of two Vectors
     * @param a - first Vector
     * @param b - second Vector
     * @return - scalar multiplication
     */
    static float dot(const Vector &a, const Vector &b);

    float x, y, z;
};

/**
 * Structure represent the object - Pixel
 */
struct Pixel {
    Pixel();
    Pixel (uint8_t r, uint8_t g, uint8_t b);

    /**
     * Multiply Pixel by number
     * @param k - number
     * @return - new Pixel
     */
    Pixel operator*(float k);

    /**
     * Divide Pixel by number
     * @param k - number
     * @return - new Pixel
     */
    Pixel operator/(float k);

    /**
     * Addition of Pixel
     * @param pixel - pixel to add
     * @return
     */
    Pixel &operator+=(const Pixel &pixel);

    bool operator==(const Pixel &pixel);

    bool operator!=(const Pixel &pixel);

    RGBApixel value;

    /* Some static colors */
    static Pixel Blue;
    static Pixel Black;
    static Pixel White;
    static Pixel Red;
    static Pixel Green;
    static Pixel Gray;
    static Pixel LightPink;
};

#endif //MASHGRAPH3_BASESTRUCTURES_H
