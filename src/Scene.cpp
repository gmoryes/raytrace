#include "Scene.h"
#include <sstream>
#include <cassert>

Scene::Scene(const Vector &left_border, const Vector &right_border) :
    left_border(left_border),
    right_border(right_border),
    image_height(0),
    image_width(0)
{}

void Scene::AddLight(const Vector &point) {
    lights.emplace_back(point);
}

void Scene::AddFigure(Figure *d) {
    figures.push_back(d);
}

void Scene::ConfigureCamera(Camera &camera, int pixel_height, int pixel_width) {
    this->camera = camera;

    image_width = pixel_width;
    image_height = pixel_height;

    pixel_matrix.resize(pixel_height);
    for (int i = 0; i < pixel_height; i++) {
        pixel_matrix[i].resize(pixel_width);
    }
}

/*
 +-------------------+
 |         |         |
 |       height      |
 |         |         |
 |--width--+--width--|
 |         |         |
 |       height      |
 |         |         |
 |-------------------|
 */
void Scene::StartTraceRacing(ArgumentsParser &argumentsParser) {

    std::cout << "Start Trace" << std::endl;

    /* Look at the picture */
    int width = image_width / 2;
    int height = image_height / 2;

    float gip_y = camera.distance / float(sin(M_PI / 4.0f));
    float gip_x = camera.distance / float(sin(M_PI / 4.0f));

    float right_length = gip_x * float(cos(M_PI / 4.0f));
    float up_length = gip_y * float(cos(M_PI / 4.0f));

    Vector true_up = camera.up * up_length;
    Vector true_right = camera.right * right_length;

    Vector vector_to_screen = camera.direction * camera.distance;
    Vector direction;

    float shift_x[4] = {-0.5f, 0.5f, 0.5f, -0.5f};
    float shift_y[4] = {0.5f, 0.5f, -0.5f, -0.5f};

    Vector dir_y;
    Vector dir_x;

    Vector color;

    int threads_number = static_cast<int>(argumentsParser.Get<int>("--threads"));
    std::cout << "Threads number: " << threads_number << std::endl;
    if (argumentsParser.Get<bool>("--antialiasing"))
        std::cout << "Antialiasing enable" << std::endl;
    else
        std::cout << "Antialiasing disabled" << std::endl;

#pragma omp parallel num_threads(threads_number)
    {
    #pragma omp for private(direction, dir_x, dir_y, color) schedule(dynamic, height / 4)
    for (int y = -height; y < height; y++) {
        for (int x = -width; x < width; x++) {

            color = Vector(0, 0, 0);

            int antialiasing_side_number = (argumentsParser.Get<bool>("--antialiasing")) ? 4 : 1;

            for (int i = 0; i < antialiasing_side_number; i++) {
                dir_y = true_up    * ((y + shift_y[i]) / float(height));
                dir_x = true_right * ((x + shift_x[i]) / float(width));

                direction = vector_to_screen + dir_x + dir_y;
                direction.normalize();

                Pixel pixel = Scene::GetColorOfRay(camera.position, direction);

                color.x += pixel.value.Red; color.y += pixel.value.Green; color.z += pixel.value.Blue;
            }

            color = color / float(antialiasing_side_number);

            pixel_matrix[y + height][x + width] = Pixel(color.x, color.y, color.z);
        }
    }
    }

    std::cout << "End Trace" << std::endl;
}

Pixel Scene::GetColorOfRay(const Vector &source, const Vector &direction, int reflect_count) {
    Vector intersect_point;
    Figure *intersect_figure = nullptr;
    Pixel pixel;

    bool is_intersect = Scene::FigureIntersectWith(source, direction, intersect_point, intersect_figure);
    if (not is_intersect) {
        /* If we intersect nothing, set Blue color of ray */
        pixel = Pixel::Blue;
    } else {
        assert(intersect_figure != nullptr);

        pixel = intersect_figure->DefaultColor();
        Vector norm = intersect_figure->normal(intersect_point);

        /* Get normal and look for light sources */

        for (auto &light : lights) {
            Vector dir_to_light = light.source - intersect_point;
            dir_to_light.normalize();

            is_intersect = Scene::FigureIntersectWith(intersect_point, dir_to_light);
            if (is_intersect) // If we intersect something, it means no light here
                continue;

            float angle = dir_to_light.GetCosAngleWith(norm);
            pixel += Pixel::White * (std::abs(angle) / 1.20);
        }

        /* Check if intersect figure is reflectable */
        if (intersect_figure->IsReflectable() && reflect_count != 0) {

            Vector reflect = Vector::reflect(direction, norm);
            reflect.normalize();

            float k = intersect_figure->ReflectCoefficient();

            Pixel after_reflect = GetColorOfRay(intersect_point, reflect, reflect_count - 1);
            if (after_reflect != Pixel::Blue) {
                pixel += after_reflect * k;
            }
        }

        /* Check if intersect figure is reflractable */
        if (intersect_figure->Refractable() && reflect_count != 0) {

            float eta = intersect_figure->RefractableEta();
            float k = intersect_figure->RefractableCoefficient();

            Vector refract = Vector::refract(direction, norm, eta);
            refract.normalize();

            Pixel after_reflect = GetColorOfRay(intersect_point, refract, reflect_count - 1);
            if (after_reflect != Pixel::Blue) {
                pixel += after_reflect * k;
            }
        }
    }

    return pixel;
}

void Scene::SaveImage(std::string &&filename) {
    std::cout << "Start Draw" << std::endl;

    BMP out;
    out.SetSize(image_width, image_height);

    for (int y = 0; y < image_height; y++) {
        for (int x = 0; x < image_width; x++) {
            out.SetPixel(x, y, pixel_matrix[y][x].value);
        }
    }

    if (!out.WriteToFile(filename.c_str())) {
        std::stringstream ss;
        ss << "Error write to file: " << filename;
        throw std::runtime_error(ss.str());
    }

    std::cout << "End Draw" << std::endl;
}

bool Scene::FigureIntersectWith(const Vector &source,
                                const Vector &direction,
                                Vector &intersect_point,
                                Figure *&intersect_figure,
                                bool save) {
    float current_distance = INF;
    Vector current_position = source;
    Figure *result = nullptr;
    int step_count = 0;

    do {
        float dist = current_distance;
        for (auto figure : figures) {
            float d = std::abs(figure->distance(current_position));
            if (dist > d) {
                dist = d;
                result = figure;
            }
        }

        if (dist < EPS)
            dist = 0.1;

        current_position = current_position + direction * dist;
        if (not IsPointIntoScene(current_position)) {
            result = nullptr;
            break;
        }

        current_distance = std::abs(result->distance(current_position));

        step_count++;
        if (step_count >= MAX_TRACE_STEPS_COUNT) {
            result = nullptr;
            break;
        }

    } while (current_distance > EPS);

    if (save) {
        intersect_point = current_position;
        intersect_figure = result;
    }

    return result != nullptr;
}

bool Scene::FigureIntersectWith(const Vector &source, const Vector &direction) {
    Vector a;
    Figure *b = nullptr;
    FigureIntersectWith(source, direction, a, b, false);
}

bool Scene::IsPointIntoScene(const Vector &point) {
    return
        left_border.x <= point.x && point.x <= right_border.x &&
        left_border.y <= point.y && point.y <= right_border.y &&
        left_border.z <= point.z && point.z <= right_border.z;

}
