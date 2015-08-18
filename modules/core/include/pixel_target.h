#ifndef PIXEL_TARGET_H_INCLUDED
#define PIXEL_TARGET_H_INCLUDED

/**
 * @file pixel_target.h
 *
 * @brief Container class for storing information about
 *        targets located in photos or video frames using pixels as measurement
 *
 */

#include <opencv2/core/core.hpp>
#include <vector>

class Frame;

class PixelTarget{
public:
    PixelTarget(std::string type, cv::Point2d centroid, double area, double perimeter, cv::Scalar colour, cv::Point2d error, double errorAngle);
    
    /**
     * @brief Getter for type field
     *
     * @return Type std::string associated with the PixelTarget
     */
    std::string get_type();

    /**
     * @brief Getter for centroid
     *
     * @return Pixel location of the PixelTarget in its frame
     */
    cv::Point2d get_centroid();

    /**
     * @brief Getter for area
     *
     * @return Area of the PixelTarget in pixels as it appears in its frame
     */
    double get_area();

    /**
     * @brief Getter for perimeter
     *
     * @return Perimeter of the PixelTarget in pixels as it appears in its frame
     */
    double get_perimeter();

    /**
     * @brief Getter for colour
     *
     * @return Average colour of the interiour of the PixelTarget's contour
     */
    cv::Scalar get_colour();

    /**
     * @brief Getter for error
     *
     * @return 2D error magnitude of the PixelTarget's location
     */
    cv::Point2d get_error();

    /**
     * @brief Getter for error angle
     *
     * @return Angle in radians between the direction of the error and the vertical
     */
    double get_error_angle();

    /**
     * @brief Getter for image
     *
     * @return Frame associated with this PixelTarget
     */
    Frame * get_image();
private:
    /**
     * @brief PixelTarget type description 
     */
    std::string type;

    /**
     * @brief Pixel locations of the vertices of the PixelTarget
     */
    std::vector<cv::Point> contour;
    
    /**
     * @brief Pixel location of the centre of the PixelTarget
     */
    cv::Point2d centroid;
    
    /**
     * @brief area of the target in pixels
     */
    double area;

    /**
     * @brief perimeter of the target in pixels 
     */
    double perimeter;
    
    /**
     * @brief Colour of the target in BGR (Blue, Green, Red) format
     */
    cv::Scalar colour;
    
    /**
     * @brief Calculated location error of the target as a 2D rectangle in pixels
     */
    cv::Point2d error;
    
    /**
     * @brief Angle of the error as radians clockwise from vertical
     */
    double errorAngle;

    /**
     * @brief Frame described by this PixelTarget
     */
    Frame * image;
};


#endif // PIXEL_TARGET_H_INCLUDED
