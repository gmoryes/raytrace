#include "Figures.h"

/* Base figure implementation */

Vector FigureBaseImpl::normal(const Vector &point) {
    Vector x1 = point + Vector(GRADIENT_EPS, 0, 0);
    Vector x2 = point - Vector(GRADIENT_EPS, 0, 0);

    Vector y1 = point + Vector(0, GRADIENT_EPS, 0);
    Vector y2 = point - Vector(0, GRADIENT_EPS, 0);

    Vector z1 = point + Vector(0, 0, GRADIENT_EPS);
    Vector z2 = point - Vector(0, 0, GRADIENT_EPS);

    float dx = distance(x1) - distance(x2);
    float dy = distance(y1) - distance(y2);
    float dz = distance(z1) - distance(z2);

    Vector result = Vector(dx, dy, dz) / (2 * GRADIENT_EPS);
    result.normalize();

    return result;
}

void FigureBaseImpl::MakeReflectable(float k) {
    reflect = true;
    reflect_k = k;
}

FigureBaseImpl::FigureBaseImpl():
    reflect(false), default_color(Pixel::Gray), reflect_k(0), refract(false), refract_k(0), refract_eta(0) {}

void FigureBaseImpl::DefaultColor(const Pixel &pixel) {
    default_color = pixel;
}

Pixel FigureBaseImpl::DefaultColor() {
    return default_color;
}

bool FigureBaseImpl::IsReflectable() {
    return reflect;
}

float FigureBaseImpl::ReflectCoefficient() {
    return reflect_k;
}

void FigureBaseImpl::Refractable(float k, float eta) {
    refract_k = k;
    refract_eta = eta;
    refract = true;
}

bool FigureBaseImpl::Refractable() {
    return refract;
}

float FigureBaseImpl::RefractableEta() {
    return refract_eta;
}

float FigureBaseImpl::RefractableCoefficient() {
    return refract_k;
}

/* Sphere implementation */

Sphere::Sphere() : center(), radius(0) {}

Sphere::Sphere(const Vector &point, float radius) : center(point), radius(radius) {}

float Sphere::distance(const Vector &point) {
    return std::sqrt(
        (point.x - center.x) * (point.x - center.x) +
        (point.y - center.y) * (point.y - center.y) +
        (point.z - center.z) * (point.z - center.z)
    ) - radius;
}

/* Box implementation */

Box::Box() : center(), radius() {}

Box::Box(const Vector &point, const Vector &hights) : center(point), radius(hights) {}

float Box::distance(const Vector &point) {
    float x = std::max(
        point.x - center.x - radius.x,
        center.x - point.x - radius.x
    );

    float y = std::max(
        point.y - center.y - radius.y,
        center.y - point.y - radius.y
    );

    float z = std::max(
        point.z - center.z - radius.z,
        center.z - point.z - radius.z
    );

    float d = x;
    d = std::max(d, y);
    d = std::max(d, z);

    return d;
}

Vector Box::normal(const Vector &point) {
    float x = std::abs(std::max(
        point.x - center.x - radius.x,
        center.x - point.x - radius.x
    ));

    float y = std::abs(std::max(
        point.y - center.y - radius.y,
        center.y - point.y - radius.y
    ));

    float z = std::abs(std::max(
        point.z - center.z - radius.z,
        center.z - point.z - radius.z
    ));

    if (x < EPS)
        return Vector(1, 0, 0);
    if (y < EPS)
        return Vector(0, 1, 0);

    return Vector(0, 0, 1);
}

/* Torus implementation */

Torus::Torus(): center(), R(0), r(0) {}

Torus::Torus(const Vector &point, float R, float r): center(point), R(R), r(r) {}

float Torus::distance(const Vector &point) {
    float p_x = center.x - point.x;
    float p_y = center.y - point.y;
    float p_z = center.z - point.z;

    float k = std::sqrt(p_x * p_x + p_z * p_z);

    return std::sqrt((k - R) * (k - R) + p_y * p_y) - r;
}


