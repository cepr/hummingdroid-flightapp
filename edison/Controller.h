#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Motors.h"
#include "Telemetry.h"
#include "Value.h"
#include "Object.h"

namespace org {
namespace hummingdroid {
namespace flightapp {

class FlightService;

/**
 * Flight controller.
 *
 * <p>
 * This class implements the closed-loop control of the vehicle attitude.
 * </p>
 */
class Controller : public Object {

private:
    Motors* motors;
    Telemetry* telemetry;

    // Config
    CommandPacket::ControllerConfig config;

    // Command
    Attitude command;

    // Altitude control
    Value altitude_error;
    PID altitude_control;

    // Roll control
    Value roll_error;
    PID roll_control;

    // Pitch control
    Value pitch_error;
    PID pitch_control;

    // Yaw rate control
    Value yaw_rate_error;
    PID yaw_rate_control;

    // Motors control
    MotorsControl output;

public:
    /**
     * Constructor.
     *
     * @param context
     *            FlightService instance.
     */
    Controller(FlightService *context);

    /**
     * Sets the controller configuration.
     *
     * @param config
     *            New configuration.
     */
    void setConfig(const CommandPacket::ControllerConfig & config);

    /**
     * Sets the new command.
     *
     * <p>
     * The command is the attitude we want to reach.
     * </p>
     *
     * @param command
     *            The new command.
     */
    void setCommand(const Attitude & command);

    /**
     * Sets the latest measured attitude.
     *
     * @param attitude
     *            The latest measured attitude.
     *
     * @param timestamp
     *            The time in nanosecond of this measure.
     */
    void setAttitude(const Attitude & attitude, const Timestamp & timestamp);
};

}
}
}

#endif // CONTROLLER_H
