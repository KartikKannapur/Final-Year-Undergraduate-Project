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
#include <math.h>

#define pi 3.1416
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
void calc_ang(int m, int n, int o,int param_local[14][3][1000],int gait_cycle_local[5])
{
        int a1,a2,b1,b2,c1,c2;
        float ang;
        int i,j;
        for(i=1; i<4;i++)
        {
            printf("\n cycle %d\n", i);
            for(j=gait_cycle_local[i];j<gait_cycle_local[i+1];j++)
            {
               a1 = param_local[m][0][j]- param_local[n][0][j];
               b1 = param_local[m][1][j]- param_local[n][1][j];
               c1 = (param_local[m][2][j]- param_local[n][2][j])/10;

               a2 = param_local[o][0][j]- param_local[n][0][j];
               b2 = param_local[o][1][j]- param_local[n][1][j];
               c2 = (param_local[o][2][j]- param_local[n][2][j])/10;

                float c, d, e;
                 c= (abs(a1)*abs(a1))+(abs(b1)*abs(b1))+(abs(c1)*abs(c1));
                 d= (abs(a2)*abs(a2)+(abs(b2)*abs(b2))+(abs(c2)*abs(c2)));
                 e= (a1*a2+b1*b2+c1*c2);

                 ang= e/(sqrt(c*d));

            //   ang = ((a1*a2+b1*b2+c1*c2)/(((abs(a1)^2+abs(b1)^2+abs(c1)^2)*(abs(a2)^2+abs(b2)^2+abs(c2)^2))));
          //     printf(" %d %d %d %d %d %d\n", a1, b1 ,c1, a2, b2, c2);
           //  printf(" this %f %f %f %f this\n" , c ,d ,e ,ang);
               ang = acos(ang);

               ang = (ang*180)/pi;

               printf ("%f degrees\n", ang);

            }
        }
}


int get_step_len(int param_local[14][3][1000],int h,int gait_cycle_L[5],int gait_cycle_R[5])
{
    int step_length=0;
    if(h==13)
    {
        for(int i=1;i<4;i++) // ignoring the last gait cycle just to make 3 cycles a common factor everywhere
        {

        step_length += (param_local[13][2][gait_cycle_L[i+1]])- (param_local[12][2][gait_cycle_R[i]]);
        }

        step_length = step_length/3;
    }
    else
    {
        for(int i=1;i<4;i++)
        {
            step_length += (param_local[12][2][gait_cycle_R[i]])- (param_local[13][2][gait_cycle_L[i]]);
        }
        step_length = step_length/3;
    }

    return step_length;
}



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
        stride_time+=(float)t_local[i];
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
            printf("%d %f\n",  ((stop.tv_sec-start.tv_sec)*1000)+(stop.tv_usec-start.tv_usec)/1000,  ((stop.tv_sec-start.tv_sec)*1000)+(stop.tv_usec-start.tv_usec)/1000);
            gait_time_R_temp[z]= ((stop.tv_sec-start.tv_sec)*1000)+(stop.tv_usec-start.tv_usec)/1000;
            gettimeofday(&start, NULL);
        }
        else if(!(next_byte=='1' || next_byte=='2'))
        {
            printf("Wrong value received from arduino\n");
            return 1;
        }


        if(left_flag==1 && right_flag==1)
        {
            z++;
            left_flag=0;
        }


        printf("Gait_time_L_temp is \n");
        for(int ijk=0;ijk<5;ijk++)
        {
              printf("%d\n", gait_time_L_temp[ijk]);
        }
          printf("Gait_time_R_temp is \n");
          for(int ijk=0;ijk<5;ijk++)
          {
              printf("%d\n", gait_time_R_temp[ijk]);
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

          printf("Gait_time_L is \n");
          for(int ij=0;ij<5;ij++)
          {
              printf("%d\n", gait_time_L[ij]);
          }
          printf("Gait_time_R is \n");
          for(int ij=0;ij<5;ij++)
          {
              printf("%d\n", gait_time_R[ij]);
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

    printf("Left stride length is %d in cm\n", stride_len_L/10);
    printf("Right stride length is %d in cm\n", stride_len_R/10);

    //To calculate stride_time

    float stride_time_L= param_stride_time(gait_time_L);
    float stride_time_R= param_stride_time(gait_time_R);


    printf("The left stride time is %f in seconds\n", stride_time_L);
    printf("The right stride time is %f in seconds\n", stride_time_R);

    //To calculate stride_freq

    int stride_freq_L=0;
    int stride_freq_R=0;

    stride_freq_L = (3*60*1000)/(gait_time_L[4]+gait_time_L[3]+gait_time_L[2]);
    stride_freq_R = (3*60*1000)/(gait_time_R[4]+gait_time_R[3]+gait_time_R[2]);


    printf("The left stride frequency is %d steps per min\n", stride_freq_L );
    printf("The right stride frequency is %d steps per min\n", stride_freq_R );

    //To calculate step_len

    int step_len_LR=0;
    int step_len_RL=0;

    step_len_LR = abs(get_step_len(param, 12, gait_cycle_L, gait_cycle_R));
    step_len_RL = abs(get_step_len(param, 13, gait_cycle_L, gait_cycle_R));

    printf ("The L-R step length is %d in cm\n", step_len_LR/10 );
    printf ("The R-L step length is %d in cm\n", step_len_RL/10 );


    //To calculate angles


    // 1.Elbow angles left - 2,3,6 right- 4,5,7
    printf ("The left elbow angles are\n");
    calc_ang(1,2,5,param,gait_cycle_L);
    printf ("The right elbow angles are\n");
    calc_ang(3,4,6,param,gait_cycle_R);


    // 2.Hip angles left- 2,9,12 right- 4,8,11
    printf("The left hip angles are\n");
    calc_ang(1,8,11,param,gait_cycle_L);
    printf ("The right hip angles are\n");
    calc_ang(3,7,10,param, gait_cycle_R);

    // 3.Knee angles left-9,12,14 right- 8,11,13
    printf("The left knee angles are\n");
    calc_ang(8,11,13,param,gait_cycle_L);
    printf ("The right knee angles are\n");
    calc_ang(7,10,12,param, gait_cycle_R);





    g_scriptNode.Release();
    g_UserGenerator.Release();
    g_Context.Release();
    return 0;
}
