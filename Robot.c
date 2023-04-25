#include "WPILib.h"
#include "Timer.h"

class Robot: public SampleRobot
{
    VictorSP rightDrive;
    VictorSP intake;
    VictorSP leftDrive;
    VictorSP arm;
    Joystick stickR,stickL;// only joystick
    Timer t1, t2;
    //int KC;
    float powerL,powerR;
    bool powerintake;
    IMAQdxSession session;
        Image *frame;
        IMAQdxError imaqError;

public:
    Robot():
            //myRobot(0, 1),    // initialize the RobotDrive to use motor controllers on ports 0 and 1
        // these are the PWM slots on the Roborio-- so the arm pwm is in port 0
        rightDrive(0),
        intake(2),
        leftDrive(1),
        arm(4),
        stickR(0),
        stickL(1)

    {
        //myRobot.SetExpiration(0.1);
    }

    /**
    * Runs the motors with arcade steering.
    */
    void Autonomous(){  //auto starts
        /*KC=0;
        while(KC < 1000)
        {
            KC++;
        }*/
        t2.Reset();
        t2.Start();

        t1.Reset();
        t1.Start();
        while(IsAutonomous() && IsEnabled())
        {
            while(t2.Get()<3)
                    {
                         SmartDashboard::PutNumber("t2", t2.Get());  //puts "t2" to smartdashboard for monitoring
                    }


            /*if(t1.Get()>4 && t1.Get()<4.2)  //optional arm code, unrem for low bar and portcullis
            {
                arm.Set(.25);
            }
            else
            {
                arm.Set(0);
            }*/


            if(t1.Get()>5 && t1.Get()<7)
            {
                //rightDrive.Set(-1);  //full speed, use in case of emergency
                //leftDrive.Set(1);

                rightDrive.Set(-.6);  //3/4 speed, use for moat, consider for rockwall, and ramparts
                leftDrive.Set(.6);

                //rightDrive.Set(-.5);  //1/2 speed, use for rough terrain
                //leftDrive.Set(.5);

                //rightDrive.Set(-.25);  //1/4 speed, use for portcullis, and lowbar
                //leftDrive.Set(.25);

                //rightDrive.Set(-.2);
                //leftDrive.Set(.2);
                SmartDashboard::PutNumber("t1", t1.Get());
            }
            else
            {
                rightDrive.Set(0);
                leftDrive.Set(0);
            }
        }
    }  //auto ends


        void RobotInit()
        override{
        // create an image  //begining
        frame = imaqCreateImage(IMAQ_IMAGE_RGB, 0);
        //the camera name (ex "cam0") can be found through the roborio web interface
        imaqError = IMAQdxOpenCamera("cam2", IMAQdxCameraControlModeController, &session);
        if(imaqError != IMAQdxErrorSuccess) {
            DriverStation::ReportError("IMAQdxOpenCamera error: " + std::to_string((long)imaqError) + "\n");
        }
        imaqError = IMAQdxConfigureGrab(session);
        if(imaqError != IMAQdxErrorSuccess) {
            DriverStation::ReportError("IMAQdxConfigureGrab error: " + std::to_string((long)imaqError) + "\n");
        }
    }
    void OperatorControl()

    {
        IMAQdxStartAcquisition(session);

        while (IsOperatorControl() && IsEnabled())
        {
            if(stickR.GetRawButton(6)==true)
            {
                intake.Set(.5);//in
            }
            else if(stickR.GetRawButton(4)==true)
            {
                intake.Set(-1);//out
            }
            else
            {
                intake.Set(0);
            }



            if(stickL.GetRawButton(9)==true) //down
            {
                arm.Set(1);
            }


            if(stickL.GetRawButton(11)==true) //up
            {
                arm.Set(-.3);
            }

            else
            {
                arm.Set(0);
            }


            IMAQdxGrab(session, frame, true, NULL);
                        if(imaqError != IMAQdxErrorSuccess) {
                            DriverStation::ReportError("IMAQdxGrab error: " + std::to_string((long)imaqError) + "\n");
                        } else {
                            imaqDrawShapeOnImage(frame, frame, { 10, 10, 100, 100 }, DrawMode::IMAQ_DRAW_VALUE, ShapeMode::IMAQ_SHAPE_OVAL, 0.0f);
                            CameraServer::GetInstance()->SetImage(frame);
                        }
                        Wait(0.005);                // wait for a motor update time
            powerL=-stickL.GetRawAxis(1);
            powerR=stickR.GetRawAxis(1);



            //myRobot.ArcadeDrive(stick); // drive with arcade style (use right stick)
            leftDrive.Set(powerL);
            rightDrive.Set(powerR);
            Wait(0.005);                // wait for a motor update time
        }
        IMAQdxStopAcquisition(session);
    }



};

START_ROBOT_CLASS(Robot);
