#include "nvidia.hpp"

#include <inttypes.h>
#include <iostream>

#include "libXNVCtrl/NVCtrl.h"
#include "libXNVCtrl/NVCtrlLib.h"
#include "libXNVCtrl/nv-control-screen.h"

auto NVIDIA::isAvailable() noexcept -> bool
{
        auto dpy = XOpenDisplay(NULL);
        if(!dpy)
                return false;
        else
        {
                XCloseDisplay(dpy);
                return true;
        }
}

auto static inline _GetNvXScreen(Display *dpy) -> int
{
        auto defscr = DefaultScreen(dpy);

        if(XNVCTRLIsNvScreen(dpy, defscr))
                return defscr;

        for(auto screen = 0; screen < ScreenCount(dpy); screen++)
                if (XNVCTRLIsNvScreen(dpy, screen))
                        return screen;
        
        return -1;
}

auto NVIDIA::setVibrance(std::map<int,int> &values) noexcept -> void
{
        int *query_data;  // buffer for XNVCTRLQuery response
        int len;    // length of a respons

        auto dpy = XOpenDisplay(NULL);
        auto screen = _GetNvXScreen(dpy);

        // query all dpys
        XNVCTRLQueryTargetBinaryData(dpy,
                                     NV_CTRL_TARGET_TYPE_X_SCREEN,
                                     screen,
                                     0,
                                     NV_CTRL_BINARY_DATA_DISPLAYS_ENABLED_ON_XSCREEN,
                                     (unsigned char **)&query_data,
                                     &len);

        // query each dpy
        for (int i = 1; i <= query_data[0]; i++)
        {
                NVCTRLAttributeValidValuesRec valid_values;
                int dpyId = query_data[i];        // id of queried monitor
                int setval = 0;

                // get DVC range values
                if(!XNVCTRLQueryValidTargetAttributeValues( dpy,
                                                            NV_CTRL_TARGET_TYPE_DISPLAY,
                                                            dpyId,
                                                            0,
                                                            NV_CTRL_DIGITAL_VIBRANCE,
                                                            &valid_values))
                {
                        std::cerr << "Unable to query the valid vibrance ranges on DPY-" << dpyId << " " << screen << " " << XDisplayName(NULL) << std::endl;
                        continue;
                }

                // check if response has right type
                if (valid_values.type != ATTRIBUTE_TYPE_RANGE)
                {
                        std::cerr << "NV_CTRL_DIGITAL_VIBRANCE is not of type RANGE" << std::endl;
                        continue;
                }

                // normalize input values to <-100,100> and map them to true range (typically <-1024,1023>)
                int level = (values)[dpyId];  // get desired level from input vector
                if(level > 0)
                {
                        if(level > 100)
                                level = 100;
                        setval = level * ((double)valid_values.u.range.max / 100);
                }
                else if (level < 0)
                {
                        if(level < -100)
                                level = -100;
                        setval = level * (((double)valid_values.u.range.min / 100)*-1);
                }

                // set DVC
                XNVCTRLSetTargetAttribute(dpy,
                                          NV_CTRL_TARGET_TYPE_DISPLAY,
                                          dpyId,
                                          0,
                                          NV_CTRL_DIGITAL_VIBRANCE,
                                          setval);
                // Flush display
                XFlush(dpy);
        }
        if(query_data)
                free(query_data);
        if(dpy)
                XCloseDisplay(dpy);
}

// returns std::map of <dpyId, dvc_level> for current Nvidia screen
auto NVIDIA::getVibrance() noexcept -> std::map<int, int>
{
        int *query_data;
        int len;
        std::map<int,int> map;

        static Display *dpy = 0L;
        if(!dpy && !(dpy = XOpenDisplay(NULL)))
                return map;
        
        auto screen = _GetNvXScreen(dpy);

        // Query displays (CRTCs) on screen
        if(!XNVCTRLQueryTargetBinaryData(dpy,
                                         NV_CTRL_TARGET_TYPE_X_SCREEN,
                                         screen,
                                         0,
                                         NV_CTRL_BINARY_DATA_DISPLAYS_ENABLED_ON_XSCREEN,
                                         (unsigned char **)&query_data,
                                         &len))
        {
                std::cerr <<  "Unable to determine enabled display devices for" << std::endl;
                std::cerr << "screen " << screen << "of " << XDisplayName(NULL) << std::endl;
                return map;
        }

        //        std::cout << "num screens " << data[0] << std::endl;

        // @query_data[0]  - num screens
        // @query_data[i] - dpyId
        for (int i = 1; i <= query_data[0]; i++)
        {
                //            std::cout << "dpyId[" <<  data[i];
                int dpyId = query_data[i];
                int value;

                // query valid ranges
                NVCTRLAttributeValidValuesRec valid_values;
                if(!XNVCTRLQueryValidTargetAttributeValues(dpy,
                                                           NV_CTRL_TARGET_TYPE_DISPLAY,
                                                           dpyId,
                                                           0,
                                                           NV_CTRL_DIGITAL_VIBRANCE,
                                                           &valid_values))
                {
                        // set zero on failure
                        map[dpyId] = 0;
                        continue;
                }

                // query dvc level for specified dpyId, return in @value
                XNVCTRLQueryTargetAttribute(dpy,
                                            NV_CTRL_TARGET_TYPE_DISPLAY,
                                            dpyId,
                                            0,
                                            NV_CTRL_DIGITAL_VIBRANCE,
                                            &value);

                //            std::cout << "]=" << value << std::endl;
                // normalize value to <-100, 100> according to list of valid ranges
                map[dpyId] = value > 0 ?
                                        value / ((double)valid_values.u.range.max / 100) :
                                        value / ((double)valid_values.u.range.min / 100);
        }
        if(query_data)
                free(query_data);
        if(dpy)
                XCloseDisplay(dpy);

        return map;
}

