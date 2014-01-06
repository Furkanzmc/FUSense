FUSense
=======

A class to detect gestures and postures and it has some helper functions for depth sensors supporting OpenNI functions using OpenNI/NiTE.
This class only processes the input from the depth sensor camera, it DOES NOT get any input form depth sensor. You have to feed it to the functions.
I used sf::Image when using void FUSense::saveRGBImage(const RGB888Pixel *pixels, std::string path), feel free to change it to your needs. SFML is not needed for this class to work except the mentioned function. Delete it if you don't need it...
