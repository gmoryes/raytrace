#include "BaseStructures.h"
#include <cmath>

Pixel Pixel::Red = Pixel(255, 0, 0);
Pixel Pixel::Green = Pixel(0, 255, 0);
Pixel Pixel::Blue = Pixel(0, 0, 255);

Pixel Pixel::Black = Pixel(0, 0, 0);
Pixel Pixel::White = Pixel(255, 255, 255);
Pixel Pixel::Gray = Pixel(40, 40, 40);
Pixel Pixel::LightPink = Pixel(110, 0, 110);

Vector Vector::operator*(float k) const {
    return Vector(x * k, y * k, z * k);
}

Vector Vector::operator/(float k) const {
    if (std::abs(k) < 1e-20)
        throw std::runtime_error("Zero division in vector");

    return Vector(x / k, y / k, z / k);
}

Vector Vector::operator+(const Vector &v) const {
    return Vector(x + v.x, y + v.y, z + v.z);
}

Vector Vector::operator-(const Vector &v) const {
    return Vector(x - v.x, y - v.y, z - v.z);
}

void Vector::normalize() {
    float sum = std::sqrt(x * x + y * y + z * z);
    if (sum < 1e-20)
        return;
    x /= sum; y /= sum; z /= sum;
}

float Vector::GetCosAngleWith(const Vector &vector) const {
    float scalar_mult = x * vector.x + y * vector.y + z * vector.z;
    return scalar_mult / (this->length() * vector.length());
}

float Vector::length() const {
    return std::sqrt(x * x + y * y + z * z);
}

float Vector::dot(const Vector &a, const Vector &b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector Vector::reflect(const Vector &ray, const Vector &norm) {
    return -(-ray + norm * 2 * Vector::dot(ray, norm));
}

Vector Vector::operator-() const {
    return Vector(-x, -y, -z);
}

Vector Vector::refract(const Vector &ray, Vector norm, float eta) {
    bool need_reflect = false;
    float cos_alpha = ray.GetCosAngleWith(norm);
    if (cos_alpha > 0) {
        norm = -norm;
    }
    cos_alpha = std::abs(cos_alpha);

    float sin_alpha = std::sqrt(1 - cos_alpha * cos_alpha);

    float sin_beta = eta * sin_alpha;

    Vector a = ray * eta;
    Vector b = norm * (eta * cos_alpha - std::sqrt(1 - sin_beta * sin_beta));

    Vector a1 = (ray + norm * cos_alpha) * eta;
    Vector b1 = norm * -std::sqrt(1 - a1.length() * a1.length());

    return a1 + b1;
}

/* Pixel implementation */

Pixel::Pixel() {
    value = RGBApixel{0, 0, 0};
}

Pixel::Pixel(uint8_t r, uint8_t g, uint8_t b) {
    RGBApixel pixel{};
    pixel.Red = r;
    pixel.Green = g;
    pixel.Blue = b;

    value = pixel;
}

Pixel Pixel::operator*(float k) {
    uint8_t red = static_cast<uint8_t >(std::min(255, int(float(value.Red) * k)));
    uint8_t green = static_cast<uint8_t >(std::min(255, int(float(value.Green) * k)));
    uint8_t blue = static_cast<uint8_t >(std::min(255, int(float(value.Blue) * k)));

    return Pixel(red, green, blue);
}

Pixel &Pixel::operator+=(const Pixel &pixel) {
    uint8_t red = static_cast<uint8_t>(std::min(255, int(value.Red) + int(pixel.value.Red)));
    uint8_t green = static_cast<uint8_t>(std::min(255, int(value.Green) + int(pixel.value.Green)));
    uint8_t blue = static_cast<uint8_t>(std::min(255, int(value.Blue) + int(pixel.value.Blue)));

    value.Red = red;
    value.Green = green;
    value.Blue = blue;
}

bool Pixel::operator==(const Pixel &pixel) {
    return
        value.Red == pixel.value.Red &&
        value.Green == pixel.value.Green &&
        value.Blue == pixel.value.Blue;
}

bool Pixel::operator!=(const Pixel &pixel) {
    return !(*this == pixel);
}

Pixel Pixel::operator/(float k) {
    if (k < 1e-20)
        throw std::runtime_error("Zeri devision in Pixel");

    uint8_t red = static_cast<uint8_t >(std::min(255, int(float(value.Red) / k)));
    uint8_t green = static_cast<uint8_t >(std::min(255, int(float(value.Green) / k)));
    uint8_t blue = static_cast<uint8_t >(std::min(255, int(float(value.Blue) / k)));

    return Pixel(red, green, blue);
}
