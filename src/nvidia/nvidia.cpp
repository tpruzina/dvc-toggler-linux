#include "nvidia.hpp"

NVIDIA::NVIDIA() :
	dpy(XOpenDisplay(NULL))
{
	int *data;
	int len;

	// Query monitors (CRTCs on screen)
	if(!XNVCTRLQueryTargetBinaryData(dpy,
					 NV_CTRL_TARGET_TYPE_X_SCREEN,
					 GetNvXScreen(dpy),
					 0,
					 NV_CTRL_BINARY_DATA_DISPLAYS_ENABLED_ON_XSCREEN,
					 (unsigned char**) &data,
					 &len))
        {
            throw "No DVC enabeld monitors found\n";
        }
}

int
NVIDIA::set_vibrance(std::map<int,int> values)
{
	int *data;  // buffer for XNVCTRLQuery response
	int len;    // length of a respons

	if(!dpy && !(dpy = XOpenDisplay(NULL)))
		return -1;

	// get active nvidia screen
	int screen = GetNvXScreen(dpy);

	// query all dpys
	XNVCTRLQueryTargetBinaryData(dpy,
					 NV_CTRL_TARGET_TYPE_X_SCREEN,
					 screen,
					 0,
					 NV_CTRL_BINARY_DATA_DISPLAYS_ENABLED_ON_XSCREEN,
					 (unsigned char **)&data,
					 &len);
        
        // query each dpy
	for (int i = 1; i <= data[0]; i++)
	{
	        NVCTRLAttributeValidValuesRec valid_values;
		int dpyId = data[i];        // id of queried monitor
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
		int level = values[dpyId];  // get desired level from input vector
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
        return 0;
}

std::map<int, int>
NVIDIA::get_vibrance()
{
	int *data;
	int len;

        std::map<int,int> map;

	if(!dpy && !(dpy = XOpenDisplay(NULL)))
			return map;
	
        
        int screen = GetNvXScreen(dpy);

	if(!XNVCTRLQueryTargetBinaryData(dpy,
					   NV_CTRL_TARGET_TYPE_X_SCREEN,
					   screen,
					   0,
					   NV_CTRL_BINARY_DATA_DISPLAYS_ENABLED_ON_XSCREEN,
					   (unsigned char **)&data,
					   &len))
        {
		std::cerr <<  "Unable to determine enabled display devices for" << std::endl;
		std::cerr << "screen " << screen << "of " << XDisplayName(NULL) << std::endl;
	        return map;
        }

	std::cout << "num screens " << data[0] << std::endl;

        for (int i = 1; i <= data[0]; i++)
        {
	    std::cout << "dpyId[" <<  data[i];
            int dpyId = data[i];
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
                // zero on failure
                map[dpyId] = 0;
                continue;
            }
            
            XNVCTRLQueryTargetAttribute(dpy,
					  NV_CTRL_TARGET_TYPE_DISPLAY,
					  dpyId,
					  0,
					  NV_CTRL_DIGITAL_VIBRANCE,
					  &value);

	    std::cout << "]=" << value << std::endl;
	    map[dpyId] = value > 0 ?
				value / ((double)valid_values.u.range.max / 100) :
				value / ((double)valid_values.u.range.min / 100);
        }
        return map;
}

#ifdef _DEBUG
int main(int argc, char **argv)
{
	NVIDIA *nv = new NVIDIA;
        
        std::map<int,int> res = nv->get_vibrance();
        
        for (const auto &val : res)
            res[val.first] = val.second + 10;
        
        nv->set_vibrance(res);           
	return 0;
}
#endif
