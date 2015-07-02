#ifndef TARGET_H_INCLUDED
#define TARGET_H_INCLUDED

/**
  * @file target.h
  *
  * @brief Container class for storing information about
  *     identified targets in real-world measurements
  *     Adding PixelTargets consolidates their information
  *     into the Target
  *
  */

#include <opencv2/core/core.hpp>
#include <vector>

using cv::Mat, cv::Scalar, cv::Point2f, cv::Point;

class Target{
public:
    Target(string type);
   
    /**
     * @brief Getter for Target image
     *
     * @return The best image of the Target from one of the PixelTargets
     */
    Mat get_image();

    /**
     * @brief Getter for type field
     *
     * @return Type string associated with the Target
     */
    string get_type();

    /**
     * @brief Getter for centroid
     *
     * @return GPS co-ordinates of the Target
     */
    Point2f get_centroid();

    /**
     * @brief Getter for area
     *
     * @return Area of the Target in square metres
     */
    double get_area();

    /**
     * @brief Getter for perimeter
     *
     * @return Perimeter of the Target in metres
     */
    double get_perimeter();

    /**
     * @brief Getter for colour
     *
     * @return Average colour of the interiour of the Target
     */
    Scalar get_colour();

    /**
     * @brief Getter for error
     *
     * @return 2D error magnitude of the Target's location in metres
     */
    Point2f get_error();

    /**
     * @brief Getter for error angle
     *
     * @return Angle in radians between the direction of the error and North
     */
    double get_error_angle();

    /**
     * @brief Adds given PixelTarget to Target's storage
     *        and recalculate target information
     *
     * @param t PixelTarget to be added
     */
    void add_pixel_target(PixelTarget * t);

    /**
     * @brief Getter for pixel Targets
     *
     * @return Array containing all of the PixelTargets that were used to 
     *         create this instance of Target
     */
    PixelTarget * [] get_pixel_targets();
private:

    /**
     * @brief Cropped Image of the Target
     */
    Mat image;

    /**
     * @brief current quality of the Image
     */
    double imageQuality;

    /**
     * @brief PixelTarget type description 
     */
    string type;

    /**
     * @brief GPS co-ordinates of the centre of the Target
     */
    Point2f centroid;
    
    /**
     * @brief area of the target in square metres
     */
    double area;

    /**
     * @brief perimeter of the target in metres
     */
    double perimeter;
    
    /**
     * @brief Colour of the target in BGR (Blue, Green, Red) format
     */
    Scalar colour;
    
    /**
     * @brief Calculated location error of the target as a 2D rectangle in metres
     */
    Point2f error;
    
    /**
     * @brief Angle of the error as degrees clockwise from North
     */
    double error-angle;

    /**
     * @brief PixelTargets used to form this Target instance
     */
    vector<PixelTarget *> pixelTargets;
};


#endif // TARGET_H_INCLUDED
