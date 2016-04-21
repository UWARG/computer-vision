#ifndef CSV_READING_H_INCLUDED
#define CSV_READING_H_INCLUDED

#include "metadata.h"
#include <vector>
#include <string>

/**
* @brief this is for reading telemetry logs from the plane and store them into a vector. prints a fatal log "error: failed to open the csv file" on failure.
*
*
* @param filename the relative path and name of the file
* @return return the vector on success and return an empty vector on failure
*/

struct DataRead{
    unsigned int sys_time;
    float time;
    double lon;
    double lat;
    float pitch;
    float pitch_rate;
    float pitch_setpoint;
    float pitch_rate_setpoint;
    double pitch_kd;
    double pitch_kp;
    double pitch_ki;
    float roll;
    float roll_rate;
    float roll_setpoint;
    float roll_rate_setpoint;
    double roll_kd;
    double roll_kp;
    double roll_ki;
    float yaw;
    float yaw_rate;
    float yaw_rate_setpoint;
    double yaw_kd;
    double yaw_kp;
    double yaw_ki;
    float airspeed;
    float heading;
    float heading_setpoint;
    double heading_kd;
    double heading_kp;
    double heading_ki;
    float altitude;
    float altitude_setpoint;
    double altitude_kd;
    double altitude_kp;
    double altitude_ki;
    float flap_setpoint;
    double flap_kd;
    double flap_kp;
    double flap_ki;
    float ground_speed;
    float throttle_setpoint;
    double throttle_kd;
    double throttle_kp;
    double throttle_ki;
    int wireless_connection;
    int last_command_sent0;
    int last_command_sent1;
    int last_command_sent2;
    int last_command_sent3;
    unsigned int camera_status;
    char waypointindex;
    char path_gain;
    char orbit_gain;
    char battery_level1;
    char battery_level2;
    char ch1in,ch2in,ch3in,ch4in,ch5in,ch6in,ch7in,ch8in,ch1out,ch2out,ch3out,ch4out,ch5out,ch6out,ch7out,ch8out;
    int autopilot_active;
    char gps_status;
    int path_checksum;
    int waypoint_count;
    int waypoint_index;
    int path_following;
    int autonomousLevel;
    int startup_error_codes;
    int startupSettings;
    int RSSI;
};

class MetadataInput{
    public:
        MetadataInput(std::string filename);

        ~MetadataInput();

        void push_back(DataRead newEntry);

        Metadata bisearcher(double timestamp,int begin,int end);

        Metadata get_metadata(double timestamp);

        Metadata next_metadata();
    private:
        int output_cameraStatus=0;
        std::string heads[80];
        std::vector<DataRead> log;
        std::vector<Metadata> md;
};

#endif // CSV_READING_H_INCLUDED
