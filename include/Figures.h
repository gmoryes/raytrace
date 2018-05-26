#ifndef MASHGRAPH3_FIGURESLIST_H
#define MASHGRAPH3_FIGURESLIST_H

#include "BaseStructures.h"
#include <cmath>
#include <algorithm>

/*
 * This file defines the list of Figures, that could be in application
 * To create new Figure, we must inherite from FigureBaseImpl, and implement
 * the distance() method - return the distance from point to Figure. In some case we must implement own
 * normal() method (the case of not continuous Figures, such as Box)
 */

/**
 * Abstract class, represent the Figure
 */
class Figure {
public:
    /**
     * Must return the distance from point to Figure (if distance is 0, it means the point lies at Figure)
     * @param point - Point
     * @return distance
     */
    virtual float distance(const Vector &point) = 0;

    /**
     * Calculate the normal to the surface
     * @param point - Point
     * @return normal Vector
     */
    virtual Vector normal(const Vector &point) = 0;

    /**
     * Make figure reflectable
     * @param k - the coefficient with which color sums after reflect
     */
    virtual void MakeReflectable(float k) = 0;
    virtual bool IsReflectable() = 0;
    virtual float ReflectCoefficient() = 0;

    /* Set the default color of Figure */
    virtual void DefaultColor(const Pixel &pixel) = 0;
    virtual Pixel DefaultColor() = 0;

    /*
     * Make figure refractable
     * @params k - like in MakeReflectable(float k)
     * @params eta - coefficient of refracraction
     */
    virtual void Refractable(float k, float eta) = 0;
    virtual bool Refractable() = 0;
    virtual float RefractableCoefficient() = 0;
    virtual float RefractableEta() = 0;
private:
};

/* The base figure implementation */
class FigureBaseImpl : public Figure {
public:
    FigureBaseImpl();

    /*
     * Get normal to surface, just get numerical gradient of distance function, may not work
     * with not continuous Figures, such as Box
     */
    virtual Vector normal(const Vector &point) override;

    /* Initialize reflect data */
    void MakeReflectable(float k) override;
    bool IsReflectable() override;
    float ReflectCoefficient() override;

    /* Initialize default color */
    void DefaultColor(const Pixel &pixel) override;
    Pixel DefaultColor() override;

    /* Initialize refract data */
    void Refractable(float k, float eta) override;
    bool Refractable() override;
    float RefractableCoefficient() override;
    float RefractableEta() override;

    bool reflect;
    float reflect_k;

    bool refract;
    float refract_eta;
    float refract_k;

    Pixel default_color;
};

/* Sphere - given by radius and center */
class Sphere : public FigureBaseImpl {
public:
    Sphere();
    Sphere(const Vector &point, float radius);

    float distance(const Vector &point) override;

private:
    Vector center;
    float radius;
};

/* Box - given by center of box and the Vector of distances from the center to it sides */
class Box : public FigureBaseImpl {
public:
    Box();
    Box(const Vector &point, const Vector &hights);

    float distance(const Vector &point) override;
    Vector normal(const Vector &point) override;
private:
    Vector center;
    Vector radius;
};

/* Torus - given by center and two radiuses */
class Torus : public FigureBaseImpl {
public:
    Torus();
    Torus(const Vector &point, float R, float r);

    float distance(const Vector &point) override;
private:
    Vector center;
    float R, r;
};

#endif //MASHGRAPH3_FIGURESLIST_H
