/// @file	AP_LandingGear.h
/// @brief	Landing gear control library
#pragma once

#include <AP_Param/AP_Param.h>
#include <AP_Common/AP_Common.h>

#define AP_LANDINGGEAR_SERVO_RETRACT_PWM_DEFAULT    1250    // default PWM value to move servo to when landing gear is up
#define AP_LANDINGGEAR_SERVO_DEPLOY_PWM_DEFAULT     1750    // default PWM value to move servo to when landing gear is down

/// @class	AP_LandingGear
/// @brief	Class managing the control of landing gear
class AP_LandingGear {

public:

    // Gear command modes
    enum LandingGearCommand {
        LandingGear_Retract,
        LandingGear_Deploy,
        LandingGear_Deploy_And_Keep_Deployed,
    };

    /// Constructor
    AP_LandingGear()
    {
        // setup parameter defaults
        AP_Param::setup_object_defaults(this, var_info);
    }

    /// returns true if the landing gear is deployed
    bool deployed() const { return _deployed; }

    /// set landing gear position to retract, deploy or deploy-and-keep-deployed
    void set_position(LandingGearCommand cmd);

    static const struct AP_Param::GroupInfo        var_info[];

private:

    // Parameters
    AP_Int16    _servo_retract_pwm;     // PWM value to move servo to when gear is retracted
    AP_Int16    _servo_deploy_pwm;      // PWM value to move servo to when gear is deployed

    // internal variables
    bool        _deployed;              // true if the landing gear has been deployed, initialized false
    bool        _deploy_lock;           // used to force landing gear to remain deployed until another regular Deploy command is received to reduce accidental retraction
    
    /// retract - retract landing gear
    void retract();
    
    /// deploy - deploy the landing gear
    void deploy();
};
