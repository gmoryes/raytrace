#include <iostream>
#include "ArgumentsParser.h"
#include "Scene.h"
#include "Figures.h"
#include <omp.h>

using std::cout;
using std::endl;

int main(int argc, char **argv) {

    ArgumentsParser argumentsParser;

    argumentsParser.configure<std::string>("--save-to");
    argumentsParser.configure<int>("--distance-to-camera", 50);
    argumentsParser.configure<int>("--camera-position-z", 10);
    argumentsParser.configure<int>("--image-width", 512);
    argumentsParser.configure<int>("--image-height", 512);

    argumentsParser.configure<int>("--threads", 1);

    argumentsParser.configure<bool>("--antialiasing");
    argumentsParser.configure<bool>("--help");

    /* Parsing start */
    argumentsParser.Parse(argv, argc);

    if (argumentsParser.Get<bool>("--help")) {
        cout << "This is TraceRay Application" << endl;
        cout << "Possible arguments:" << endl;
        cout << "\t--save-to            - /path/to/result/image.bmp" << endl;
        cout << "\t--distance-to-camera - Distance from camera to projection screen (default: 50)" << endl;
        cout << "\t--camera-position-z  - Camera Z position (default: 10)" << endl;
        cout << "\t--image-width        - Width of result image (default: 512)" << endl;
        cout << "\t--image-height       - Height of result image (default: 512)" << endl;
        cout << "\t--threads            - Threads number (default: 1)" << endl;
        cout << "\t--antialiasing       - Enable antialiasing" << endl;
        cout << "\t--help               - Show this message" << endl;
        return 0;
    }

    argumentsParser.CheckArguments();

    /* Configure scene */
    Scene scene(Vector(-300, -300, -300), Vector(300, 300, 300));

    /* Configure camera */
    Vector camera_position(40, 50, argumentsParser.Get<int>("--camera-position-z"));
    Vector camera_direction(0, 0, 1);
    Vector camera_up(0, -1, 0);
    Vector camera_right(1, 0, 0);
    float distance_to_screen = argumentsParser.Get<int>("--distance-to-camera");
    Camera camera(camera_position, camera_direction, camera_up, camera_right, distance_to_screen);

    int image_width = argumentsParser.Get<int>("--image-width");
    int image_height = argumentsParser.Get<int>("--image-height");
    scene.ConfigureCamera(camera, image_width, image_height); // 512x512 - размер итоговой картинки

    /* Create and configure figures */
    Box *box2 = new Box(Vector(0, 20, 135), Vector(50, 10, 100));
    box2->MakeReflectable(0.1);
    box2->DefaultColor(Pixel::Red);

    Box *box3 = new Box(Vector(0, 50, 50), Vector(20, 20, 5));
    box3->Refractable(0.5, 1.03);

    Sphere *sphere1 = new Sphere(Vector(80, 70, 105), 40);
    sphere1->MakeReflectable(0.7);
    sphere1->DefaultColor(Pixel::Green / 4.0f);

    Sphere *sphere3 = new Sphere(Vector(0, 50, 90), 20);
    sphere3->DefaultColor(Pixel::LightPink);

    Torus *torus1 = new Torus(Vector(50, 40, 60), 15, 3);

    /* Add figures to scene */
    scene.AddFigure(box2);
    scene.AddFigure(sphere1);
    scene.AddFigure(sphere3);
    scene.AddFigure(box3);
    scene.AddFigure(torus1);

    /* Add light sources */
    scene.AddLight(Vector(200, 200, 200));
    scene.AddLight(Vector(-30, 30, 30));
    scene.AddLight(Vector(50, 130, 50));

    /* Start Trace Racing */
    scene.StartTraceRacing(argumentsParser);

    /* Save the result image */
    scene.SaveImage(argumentsParser.Get<std::string>("--save-to"));

    return 0;
}