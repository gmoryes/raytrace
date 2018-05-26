#ifndef MASHGRAPH3_SCENE_H
#define MASHGRAPH3_SCENE_H

#include <vector>
#include "BaseStructures.h"
#include "Figures.h"
#include "EasyBMP.h"
#include "ArgumentsParser.h"

/**
 * This file defines the Scene class
 */

/**
 * Struct represent to light source
 */
struct Light {
    Light(): source() {}
    explicit Light(const Vector &point): source(point) {}

    Vector source;
};

/**
 * Struct represent the Camera
 */
struct Camera {
    Camera(): position(), direction(), up(), right(), distance(0) {}

    /**
     * The camera given by next params
     * @param position - position of camera
     * @param direction - the direction of it
     * @param up - Vector, that talks us where is Up
     * @param right - Vector, that talks us where is Right
     * @param distance - The distance from camera to projection screen
     */
    Camera(const Vector &position,
           const Vector &direction,
           const Vector &up,
           const Vector &right,
           float distance): position(position), direction(direction), up(up), right(right), distance(distance) {}

    Vector position;
    
    Vector direction;
    Vector up;
    Vector right;

    float distance;
};

/**
 * Class, that represents Scene
 */
class Scene {
public:
    Scene(const Vector &left_border, const Vector &right_border);

    ~Scene() = default;

    /**
     * Add light
     * @param point - Point, the position of light
     */
    void AddLight(const Vector &point);

    /**
     * Add new figure
     * @param d - new figure
     */
    void AddFigure(Figure *d);

    /**
     * Initialize camera
     * @param camera
     */
    void ConfigureCamera(Camera& camera, int pixel_height, int pixel_width);

    /**
     * Start Trace Racing
     */
    void StartTraceRacing(ArgumentsParser &argumentsParser);

    /**
     * Save image
     */
    void SaveImage(std::string &&filename);
    
    void SaveImage(std::string &filename) {
        SaveImage(std::move(filename));
    }
    
    void SaveImage(const char* filename) {
        SaveImage(std::string(filename));
    }
private:
    std::vector<Light> lights;
    std::vector<Figure*> figures;

    /* The borders of scene (left down corner, right up corner) */
    Vector left_border, right_border;

    /* Camera, and result Image resolution */
    Camera camera;
    int image_height, image_width;

    /* Pixel matrix */
    using PixelMatrix = std::vector<std::vector<Pixel>>;
    PixelMatrix pixel_matrix;

    /**
     * Find figure, that intersect ray
     * @param source - ray source
     * @param direction - ray direction
     * @param intersect_point - reference to the point, in which the intersection will be written
     * @param intersect_figure - Figure, that we intersect (nullptr if not)
     * @param save - should we save the result to two upper arguments
     * @return - true, if intersect
     */
    bool FigureIntersectWith(const Vector &source,
                             const Vector &direction,
                             Vector &intersect_point,
                             Figure *&intersect_figure,
                             bool save = true);

    /**
     * Like upper function, but without saving intersection
     * @param source - ray source
     * @param direction - ray direction
     * @return 
     */
    bool FigureIntersectWith(const Vector &source,
                             const Vector &direction);
    /**
     * Check, that point is in scene
     * @param point - checking point
     * @return - true, if in, false - otherwise
     */
    bool IsPointIntoScene(const Vector &point);

    /*
     * Return the Color of ray, that run from source in direction
     */
    Pixel GetColorOfRay(const Vector &source, const Vector &direction, int reflect_count = MAX_REFLECTIONS);

};

#endif //MASHGRAPH3_SCENE_H
