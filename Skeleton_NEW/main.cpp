/*****************************************************************************
*                                                                            *
*  OpenNI 1.x Alpha                                                          *
*  Copyright (C) 2012 PrimeSense Ltd.                                        *
*                                                                            *
*  This file is part of OpenNI.                                              *
*                                                                            *
*  Licensed under the Apache License, Version 2.0 (the "License");           *
*  you may not use this file except in compliance with the License.          *
*  You may obtain a copy of the License at                                   *
*                                                                            *
*      http://www.apache.org/licenses/LICENSE-2.0                            *
*                                                                            *
*  Unless required by applicable law or agreed to in writing, software       *
*  distributed under the License is distributed on an "AS IS" BASIS,         *
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  *
*  See the License for the specific language governing permissions and       *
*  limitations under the License.                                            *
*                                                                            *
*****************************************************************************/
//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
#include <XnCppWrapper.h>
#include <iostream>
#include "SerialStream.h"
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
//---------------------------------------------------------------------------
// Defines
//---------------------------------------------------------------------------
#define SAMPLE_XML_PATH "SamplesConfig.xml"
#define SAMPLE_XML_PATH_LOCAL "SamplesConfig.xml"

//---------------------------------------------------------------------------
// Globals
//---------------------------------------------------------------------------
xn::Context g_Context;
xn::ScriptNode g_scriptNode;
xn::UserGenerator g_UserGenerator;

XnBool g_bNeedPose = FALSE;
XnChar g_strPose[20] = "";

#define MAX_NUM_USERS 15
//---------------------------------------------------------------------------
// Code
//---------------------------------------------------------------------------

int param_stride_len(int param_local[14][3][1000],int prm_num,int gait_cycle[5])
{

    int stride_len=0;

    for(int i=1;i<4;i++)        // only 3 loops(ignoring the first gait cycle)
    {

        stride_len+= (param_local[prm_num][2][gait_cycle[i]])-( param_local[prm_num][2][gait_cycle[i+1]-1]);

    }

    stride_len=stride_len/3;

    return stride_len;

}

float param_stride_time(int t_local[5])
{
    float stride_time=0;
    for (int i=1;i<5;i++)
    {
        stride_time+=t_local[i];
    }
    return stride_time/(4*1000);
}

XnBool fileExists(const char *fn)
{
	XnBool exists;
	xnOSDoesFileExist(fn, &exists);
	return exists;
}

// Callback: New user was detected
void XN_CALLBACK_TYPE User_NewUser(xn::UserGenerator& /*generator*/, XnUserID nId, void* /*pCookie*/)
{
    XnUInt32 epochTime = 0;
    xnOSGetEpochTime(&epochTime);
    printf("%d New User %d\n", epochTime, nId);
    // New user found
    if (g_bNeedPose)
    {
        g_UserGenerator.GetPoseDetectionCap().StartPoseDetection(g_strPose, nId);
    }
    else
    {
        g_UserGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
    }
}
// Callback: An existing user was lost
void XN_CALLBACK_TYPE User_LostUser(xn::UserGenerator& /*generator*/, XnUserID nId, void* /*pCookie*/)
{
    XnUInt32 epochTime = 0;
    xnOSGetEpochTime(&epochTime);
    printf("%d Lost user %d\n", epochTime, nId);
}
// Callback: Detected a pose
void XN_CALLBACK_TYPE UserPose_PoseDetected(xn::PoseDetectionCapability& /*capability*/, const XnChar* strPose, XnUserID nId, void* /*pCookie*/)
{
    XnUInt32 epochTime = 0;
    xnOSGetEpochTime(&epochTime);
    printf("%d Pose %s detected for user %d\n", epochTime, strPose, nId);
    g_UserGenerator.GetPoseDetectionCap().StopPoseDetection(nId);
    g_UserGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
}
// Callback: Started calibration
void XN_CALLBACK_TYPE UserCalibration_CalibrationStart(xn::SkeletonCapability& /*capability*/, XnUserID nId, void* /*pCookie*/)
{
    XnUInt32 epochTime = 0;
    xnOSGetEpochTime(&epochTime);
    printf("%d Calibration started for user %d\n", epochTime, nId);
}

void XN_CALLBACK_TYPE UserCalibration_CalibrationComplete(xn::SkeletonCapability& /*capability*/, XnUserID nId, XnCalibrationStatus eStatus, void* /*pCookie*/)
{
    XnUInt32 epochTime = 0;
    xnOSGetEpochTime(&epochTime);
    if (eStatus == XN_CALIBRATION_STATUS_OK)
    {
        // Calibration succeeded
        printf("%d Calibration complete, start tracking user %d\n", epochTime, nId);
        g_UserGenerator.GetSkeletonCap().StartTracking(nId);
    }
    else
    {
        // Calibration failed
        printf("%d Calibration failed for user %d\n", epochTime, nId);
        if(eStatus==XN_CALIBRATION_STATUS_MANUAL_ABORT)
        {
            printf("Manual abort occured, stop attempting to calibrate!");
            return;
        }
        if (g_bNeedPose)
        {
            g_UserGenerator.GetPoseDetectionCap().StartPoseDetection(g_strPose, nId);
        }
        else
        {
            g_UserGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
        }
    }
}


#define CHECK_RC(nRetVal, what)					    \
    if (nRetVal != XN_STATUS_OK)				    \
{								    \
    printf("%s failed: %s\n", what, xnGetStatusString(nRetVal));    \
    return nRetVal;						    \
}

int main()
{
    int left_flag=0, right_flag=1;
    int gait_time_L[5], gait_time_L_temp[5];
    int gait_time_R[5], gait_time_R_temp[5];
    int z=0;
    int gait_cycle_L[5];
    int gait_cycle_R[5];
    struct timeval stop, start;

    int param[14][3][1000];


    using namespace LibSerial ;
    XnStatus nRetVal = XN_STATUS_OK;
    xn::EnumerationErrors errors;

    const char *fn = NULL;
    if    (fileExists(SAMPLE_XML_PATH)) fn = SAMPLE_XML_PATH;
    else if (fileExists(SAMPLE_XML_PATH_LOCAL)) fn = SAMPLE_XML_PATH_LOCAL;
    else {
        printf("Could not find '%s' nor '%s'. Aborting.\n" , SAMPLE_XML_PATH, SAMPLE_XML_PATH_LOCAL);
        return XN_STATUS_ERROR;
    }
    printf("Reading config from: '%s'\n", fn);

    nRetVal = g_Context.InitFromXmlFile(fn, g_scriptNode, &errors);
    if (nRetVal == XN_STATUS_NO_NODE_PRESENT)
    {
        XnChar strError[1024];
        errors.ToString(strError, 1024);
        printf("%s\n", strError);
        return (nRetVal);
    }
    else if (nRetVal != XN_STATUS_OK)
    {
        printf("Open failed: %s\n", xnGetStatusString(nRetVal));
        return (nRetVal);
    }

    nRetVal = g_Context.FindExistingNode(XN_NODE_TYPE_USER, g_UserGenerator);
    if (nRetVal != XN_STATUS_OK)
    {
        nRetVal = g_UserGenerator.Create(g_Context);
        CHECK_RC(nRetVal, "Find user generator");
    }

    XnCallbackHandle hUserCallbacks, hCalibrationStart, hCalibrationComplete, hPoseDetected;
    if (!g_UserGenerator.IsCapabilitySupported(XN_CAPABILITY_SKELETON))
    {
        printf("Supplied user generator doesn't support skeleton\n");
        return 1;
    }
    nRetVal = g_UserGenerator.RegisterUserCallbacks(User_NewUser, User_LostUser, NULL, hUserCallbacks);
    CHECK_RC(nRetVal, "Register to user callbacks");
    nRetVal = g_UserGenerator.GetSkeletonCap().RegisterToCalibrationStart(UserCalibration_CalibrationStart, NULL, hCalibrationStart);
    CHECK_RC(nRetVal, "Register to calibration start");
    nRetVal = g_UserGenerator.GetSkeletonCap().RegisterToCalibrationComplete(UserCalibration_CalibrationComplete, NULL, hCalibrationComplete);
    CHECK_RC(nRetVal, "Register to calibration complete");

    if (g_UserGenerator.GetSkeletonCap().NeedPoseForCalibration())
    {
        g_bNeedPose = TRUE;
        if (!g_UserGenerator.IsCapabilitySupported(XN_CAPABILITY_POSE_DETECTION))
        {
            printf("Pose required, but not supported\n");
            return 1;
        }
        nRetVal = g_UserGenerator.GetPoseDetectionCap().RegisterToPoseDetected(UserPose_PoseDetected, NULL, hPoseDetected);
        CHECK_RC(nRetVal, "Register to Pose Detected");
        g_UserGenerator.GetSkeletonCap().GetCalibrationPose(g_strPose);
    }

    g_UserGenerator.GetSkeletonCap().SetSkeletonProfile(XN_SKEL_PROFILE_ALL);

    nRetVal = g_Context.StartGeneratingAll();
    CHECK_RC(nRetVal, "StartGenerating");

    XnUserID aUsers[MAX_NUM_USERS];
    XnUInt16 nUsers;
    XnSkeletonJointTransformation torsoJoint;

    printf("Starting to run\n");
    if(g_bNeedPose)
    {
        printf("Assume calibration pose\n");
    }


//******************** SERIAL Read

        SerialStream serial_port ;
    serial_port.Open( "/dev/ttyACM0" ) ;
    if ( ! serial_port.good() )
    {
        std::cerr << "[" << __FILE__ << ":" << __LINE__ << "] "
                  << "Error: Could not open serial port."
                  << std::endl ;
        exit(1) ;
    }
    //
    // Set the baud rate of the serial port.
    //
    serial_port.SetBaudRate( SerialStreamBuf::BAUD_9600 ) ;
    if ( ! serial_port.good() )
    {
        std::cerr << "Error: Could not set the baud rate." << std::endl ;
        exit(1) ;
    }
    //
    // Set the number of data bits.
    //
    serial_port.SetCharSize( SerialStreamBuf::CHAR_SIZE_8 ) ;
    if ( ! serial_port.good() )
    {
        std::cerr << "Error: Could not set the character size." << std::endl ;
        exit(1) ;
    }
    //
    // Disable parity.
    //
    serial_port.SetParity( SerialStreamBuf::PARITY_NONE ) ;
    if ( ! serial_port.good() )
    {
        std::cerr << "Error: Could not disable the parity." << std::endl ;
        exit(1) ;
    }
    //
    // Set the number of stop bits.
    //
    serial_port.SetNumOfStopBits( 1 ) ;
    if ( ! serial_port.good() )
    {
        std::cerr << "Error: Could not set the number of stop bits."
                  << std::endl ;
        exit(1) ;
    }
    //
    // Turn on hardware flow control.
    //
    serial_port.SetFlowControl( SerialStreamBuf::FLOW_CONTROL_NONE ) ;
    if ( ! serial_port.good() )
    {
        std::cerr << "Error: Could not use hardware flow control."
                  << std::endl ;
        exit(1) ;
    }


    //********************* END OF SERIAL READ

    printf("Waiting\n");
    sleep(3);   //waiting for arduino to ready itself
    printf("Started\n");

    int n=0, m=0, k=0;

	while (!xnOSWasKeyboardHit())
    {
     //   printf("Waifcfbfbghgfng\n");
         n=0;
         m=0;


        g_Context.WaitOneUpdateAll(g_UserGenerator);
        // print the torso information for the first user already tracking

        nUsers=MAX_NUM_USERS;

        g_UserGenerator.GetUsers(aUsers, nUsers);

        for(XnUInt16 i=0; i<nUsers; i++)
        {


            n=0;

            if(g_UserGenerator.GetSkeletonCap().IsTracking(aUsers[i])==FALSE)
                continue;

            g_UserGenerator.GetSkeletonCap().GetSkeletonJoint(aUsers[i],XN_SKEL_NECK,torsoJoint);
                printf("\n user %d: Neck at (%6.2f,%6.2f,%6.2f)\n",aUsers[i],
                                                                torsoJoint.position.position.X,
                                                                torsoJoint.position.position.Y,
                                                                torsoJoint.position.position.Z);
            param[n][m][k]= torsoJoint.position.position.X;
            param[n][m+1][k]= torsoJoint.position.position.Y;
            param[n][m+2][k]= torsoJoint.position.position.Z;
            n++;

             g_UserGenerator.GetSkeletonCap().GetSkeletonJoint(aUsers[i],XN_SKEL_LEFT_SHOULDER,torsoJoint);
                printf("\n user %d: Left Shoulder at (%6.2f,%6.2f,%6.2f)\n",aUsers[i],
                                                                torsoJoint.position.position.X,
                                                                torsoJoint.position.position.Y,
                                                                torsoJoint.position.position.Z);

            param[n][m][k]= torsoJoint.position.position.X;
            param[n][m+1][k]= torsoJoint.position.position.Y;
            param[n][m+2][k]= torsoJoint.position.position.Z;
            n++;

           g_UserGenerator.GetSkeletonCap().GetSkeletonJoint(aUsers[i],XN_SKEL_LEFT_ELBOW,torsoJoint);
                printf("\n user %d: Left Elbow at (%6.2f,%6.2f,%6.2f)\n",aUsers[i],
                                                                torsoJoint.position.position.X,
                                                                torsoJoint.position.position.Y,
                                                                torsoJoint.position.position.Z);

            param[n][m][k]= torsoJoint.position.position.X;
            param[n][m+1][k]= torsoJoint.position.position.Y;
            param[n][m+2][k]= torsoJoint.position.position.Z;
            n++;

            g_UserGenerator.GetSkeletonCap().GetSkeletonJoint(aUsers[i],XN_SKEL_RIGHT_SHOULDER,torsoJoint);
                printf("\n user %d: Right Shoulder at (%6.2f,%6.2f,%6.2f)\n",aUsers[i],
                                                                torsoJoint.position.position.X,
                                                                torsoJoint.position.position.Y,
                                                                torsoJoint.position.position.Z);

            param[n][m][k]= torsoJoint.position.position.X;
            param[n][m+1][k]= torsoJoint.position.position.Y;
            param[n][m+2][k]= torsoJoint.position.position.Z;
            n++;

            g_UserGenerator.GetSkeletonCap().GetSkeletonJoint(aUsers[i],XN_SKEL_RIGHT_ELBOW,torsoJoint);
                printf("\n user %d: Right Elbow at (%6.2f,%6.2f,%6.2f)\n",aUsers[i],
                                                                torsoJoint.position.position.X,
                                                                torsoJoint.position.position.Y,
                                                                torsoJoint.position.position.Z);


            param[n][m][k]= torsoJoint.position.position.X;
            param[n][m+1][k]= torsoJoint.position.position.Y;
            param[n][m+2][k]= torsoJoint.position.position.Z;
            n++;

           g_UserGenerator.GetSkeletonCap().GetSkeletonJoint(aUsers[i],XN_SKEL_LEFT_HAND,torsoJoint);
                printf("\n user %d: Left Hand (%6.2f,%6.2f,%6.2f)\n",aUsers[i],
                                                                torsoJoint.position.position.X,
                                                                torsoJoint.position.position.Y,
                                                                torsoJoint.position.position.Z);


            param[n][m][k]= torsoJoint.position.position.X;
            param[n][m+1][k]= torsoJoint.position.position.Y;
            param[n][m+2][k]= torsoJoint.position.position.Z;
            n++;

           g_UserGenerator.GetSkeletonCap().GetSkeletonJoint(aUsers[i],XN_SKEL_RIGHT_HAND,torsoJoint);
                printf("\n user %d: Right Hand at (%6.2f,%6.2f,%6.2f)\n",aUsers[i],
                                                                torsoJoint.position.position.X,
                                                                torsoJoint.position.position.Y,
                                                                torsoJoint.position.position.Z);

            param[n][m][k]= torsoJoint.position.position.X;
            param[n][m+1][k]= torsoJoint.position.position.Y;
            param[n][m+2][k]= torsoJoint.position.position.Z;
            n++;

          g_UserGenerator.GetSkeletonCap().GetSkeletonJoint(aUsers[i],XN_SKEL_RIGHT_HIP,torsoJoint);
                printf("\n user %d: Right Hip at (%6.2f,%6.2f,%6.2f)\n",aUsers[i],
                                                                torsoJoint.position.position.X,
                                                                torsoJoint.position.position.Y,
                                                                torsoJoint.position.position.Z);

            param[n][m][k]= torsoJoint.position.position.X;
            param[n][m+1][k]= torsoJoint.position.position.Y;
            param[n][m+2][k]= torsoJoint.position.position.Z;
            n++;

          g_UserGenerator.GetSkeletonCap().GetSkeletonJoint(aUsers[i],XN_SKEL_LEFT_HIP,torsoJoint);
                printf("\n user %d: Left Hip at (%6.2f,%6.2f,%6.2f)\n",aUsers[i],
                                                                torsoJoint.position.position.X,
                                                                torsoJoint.position.position.Y,
                                                                torsoJoint.position.position.Z);

            param[n][m][k]= torsoJoint.position.position.X;
            param[n][m+1][k]= torsoJoint.position.position.Y;
            param[n][m+2][k]= torsoJoint.position.position.Z;
            n++;

          g_UserGenerator.GetSkeletonCap().GetSkeletonJoint(aUsers[i],XN_SKEL_TORSO,torsoJoint);
                printf("\n user %d: Torso at (%6.2f,%6.2f,%6.2f)\n",aUsers[i],
                                                                torsoJoint.position.position.X,
                                                                torsoJoint.position.position.Y,
                                                                torsoJoint.position.position.Z);

            param[n][m][k]= torsoJoint.position.position.X;
            param[n][m+1][k]= torsoJoint.position.position.Y;
            param[n][m+2][k]= torsoJoint.position.position.Z;
            n++;

          g_UserGenerator.GetSkeletonCap().GetSkeletonJoint(aUsers[i],XN_SKEL_RIGHT_KNEE,torsoJoint);
                printf("\n user %d: Right Knee at (%6.2f,%6.2f,%6.2f)\n",aUsers[i],
                                                                torsoJoint.position.position.X,
                                                                torsoJoint.position.position.Y,
                                                                torsoJoint.position.position.Z);

          param[n][m][k]= torsoJoint.position.position.X;
            param[n][m+1][k]= torsoJoint.position.position.Y;
            param[n][m+2][k]= torsoJoint.position.position.Z;
            n++;

          g_UserGenerator.GetSkeletonCap().GetSkeletonJoint(aUsers[i],XN_SKEL_LEFT_KNEE,torsoJoint);
                printf("\n user %d: Left Knee at (%6.2f,%6.2f,%6.2f)\n",aUsers[i],
                                                                torsoJoint.position.position.X,
                                                                torsoJoint.position.position.Y,
                                                                torsoJoint.position.position.Z);

            param[n][m][k]= torsoJoint.position.position.X;
            param[n][m+1][k]= torsoJoint.position.position.Y;
            param[n][m+2][k]= torsoJoint.position.position.Z;
            n++;

          g_UserGenerator.GetSkeletonCap().GetSkeletonJoint(aUsers[i],XN_SKEL_RIGHT_FOOT,torsoJoint);
                printf("\n user %d: Right Foot at (%6.2f,%6.2f,%6.2f)\n",aUsers[i],
                                                                torsoJoint.position.position.X,
                                                                torsoJoint.position.position.Y,
                                                                torsoJoint.position.position.Z);

            param[n][m][k]= torsoJoint.position.position.X;
            param[n][m+1][k]= torsoJoint.position.position.Y;
            param[n][m+2][k]= torsoJoint.position.position.Z;
            n++;

          g_UserGenerator.GetSkeletonCap().GetSkeletonJoint(aUsers[i],XN_SKEL_LEFT_FOOT,torsoJoint);
                printf("\n user %d: Left Foot at (%6.2f,%6.2f,%6.2f)\n",aUsers[i],
                                                                torsoJoint.position.position.X,
                                                                torsoJoint.position.position.Y,
                                                                torsoJoint.position.position.Z);


            param[n][m][k]= torsoJoint.position.position.X;
            param[n][m+1][k]= torsoJoint.position.position.Y;
            param[n][m+2][k]= torsoJoint.position.position.Z;
            n++;


            k++;

//************************************** SERIAL READ




    // Do not skip whitespace characters while reading from the
    // serial port.
    //
    // serial_port.unsetf( std::ios_base::skipws ) ;
    //
    // Keep reading data from serial port and print it to the screen.
    gettimeofday(&start, NULL);




  if( serial_port.rdbuf()->in_avail() > 0 )
  {
        char next_byte;
        serial_port.get(next_byte);

        gettimeofday(&stop, NULL);
        //printf("took %ld seconds\n", stop.tv_sec-start.tv_sec);
        //printf("%ld milliseconds\n", (stop.tv_usec-start.tv_usec)/1000);


        if( next_byte=='1' && left_flag==0 )
        {
            right_flag=0; // right value can be taken now

            gait_cycle_L[z]=k;
            left_flag=1;
            gait_time_L_temp[z]= ( (stop.tv_sec-start.tv_sec)*1000)+(stop.tv_usec-start.tv_usec)/1000;
            gettimeofday(&start, NULL);
        }
        else if( next_byte=='2' && right_flag==0 )
        {
            gait_cycle_R[z]=k;
            right_flag=1;
            gait_time_R_temp[z]= ( (stop.tv_sec-start.tv_sec)*1000)+(stop.tv_usec-start.tv_usec)/1000;
            gettimeofday(&start, NULL);
        }
        else
        {
            return 1;
        }


        if(left_flag==1 && right_flag==1)
        {
            z++;
            left_flag=0;
        }


  }


    //************* END OF SERIAL READ
        if (z>=5)
      {
          // first elements in both left and right arrays are invalid data

          for(i=0;i<4;i++)
          {
              gait_time_L[i+1]=gait_time_L_temp[i+1]+gait_time_R_temp[i];
              gait_time_R[i+1]=gait_time_R_temp[i+1]+gait_time_L_temp[i+1];
          }
            printf("test\n");
            g_scriptNode.Release();
            g_UserGenerator.Release();
            g_Context.Release();
            break;
      }

    // ********************5 gait cycles

        }



           if (z>=5)
      {
           break;
      }



    }

    //To calculate stride_len

    int stride_len_L=0;
    int stride_len_R=0;


    stride_len_L = param_stride_len(param,12, gait_cycle_L);
    stride_len_R = param_stride_len(param,13, gait_cycle_R);

    printf("Left stride length is %d\n", stride_len_L);
    printf("Right stride length is %d\n", stride_len_R);

    //To calculate stride_time

    float stride_time_L= param_stride_time(gait_time_L);
    float stride_time_R= param_stride_time(gait_time_R);
    printf("The left stride time is %f in seconds\n", stride_time_L);
    printf("The right stride time is %f in seconds\n", stride_time_R);

    //To calculate stride_freq

    int stride_freq_L=0;
    int stride_freq_R=0;

    stride_freq_L = (3*60*1000)/(gait_time_L[4]-gait_time_L[1]);
    stride_freq_R = (3*60*1000)/(gait_time_R[4]-gait_time_R[1]);


    printf("The left stride frequency is %d steps per min", stride_freq_L );
    printf("The right stride frequency is %d steps per min", stride_freq_R );

    //To calculate step_len

    int step_len_LR=0;
    int step_len_RL=0;





    g_scriptNode.Release();
    g_UserGenerator.Release();
    g_Context.Release();
    return 0;
}
