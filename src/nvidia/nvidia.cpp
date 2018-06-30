#include "nvidia.hpp"

int
NVIDIA::set_vibrance(int level, int chosen_dpy)
{
    int screen;
    NVCTRLAttributeValidValuesRec valid_values;
    int *data;
    int len;

    // don't segfault if shit goes wrong
    if(!dpy && !(dpy = XOpenDisplay(NULL)))
        return 0;
    
    // normalize percentages to <-100,100>
    if(level > 100)
    	level = 100;
    else if(level < -100)
    	level = -100;

    // get active nvidia screen
    screen = GetNvXScreen(dpy);

    // query nvidia NV_CTRL feature
    if(!XNVCTRLQueryTargetBinaryData(dpy,
        		NV_CTRL_TARGET_TYPE_X_SCREEN,
			screen,
			0,
			NV_CTRL_BINARY_DATA_DISPLAYS_ENABLED_ON_XSCREEN,
			(unsigned char **)&data,
			&len)
    ){
        cerr <<  "Unable to determine enabled display devices for" << endl;
        cerr << "screen " << screen << "of " << XDisplayName(NULL) << endl;
        return 1;
    }

    for (int i = 1; i <= data[0]; i++)
    {
    	int dpyId = data[i];
    	int setval = 0;

        // skip if we were given specific dpyId and this one doesn't match it)
        if(chosen_dpy != NVIDIA_APPLY_TO_ALL && chosen_dpy != dpyId)
            continue;

        if(!XNVCTRLQueryValidTargetAttributeValues( dpy,
                                                    NV_CTRL_TARGET_TYPE_DISPLAY,
                                                    dpyId,
                                                    0,
                                                    NV_CTRL_DIGITAL_VIBRANCE,
						    &valid_values)
        ) {
            cerr << "Unable to query the valid vibrance ranges on DPY-" << dpyId << " " << screen << " " << XDisplayName(NULL) << endl;
            return 0;
        }

        if (valid_values.type != ATTRIBUTE_TYPE_RANGE)
        {
            cerr << "NV_CTRL_DIGITAL_VIBRANCE is not of type RANGE" << endl;
            return 1;
        }

        // recalculate value depending on range and percentage value
        if(level > 0)
        	setval = level * ((double)valid_values.u.range.max / 100);
        else if (level < 0)
        	setval = level * (((double)valid_values.u.range.min / 100)*-1);

        // set DVC
        XNVCTRLSetTargetAttribute(dpy,
        		NV_CTRL_TARGET_TYPE_DISPLAY,
				dpyId,
				0,
				NV_CTRL_DIGITAL_VIBRANCE,
				setval);
        XFlush(dpy);

#ifdef _DEBUG
        cout << "Set NV_CTRL_DIGITAL_VIBRANCE to " << setval << " on display device " <<
        		"DPY-" << dpyId << " of screen " << screen << " of " << XDisplayName(NULL) << endl;
#endif
    }

	return level;
}

int
NVIDIA::get_vibrance(void)
{
	Bool ret;
        int screen, value;
	NVCTRLAttributeValidValuesRec valid_values;
	int *data;
	int len;

	if(!dpy)
        {
            dpy = XOpenDisplay(NULL);
            if(!dpy)
                return 0;
        }
	screen = GetNvXScreen(dpy);

	ret = XNVCTRLQueryTargetBinaryData(dpy,
			NV_CTRL_TARGET_TYPE_X_SCREEN,
			screen,
			0,
			NV_CTRL_BINARY_DATA_DISPLAYS_ENABLED_ON_XSCREEN,
			(unsigned char **)&data,
			&len);


	if (!ret) {
		cerr <<  "Unable to determine enabled display devices for" << endl;
		cerr << "screen " << screen << "of " << XDisplayName(NULL) << endl;
		return 0;
	}

	int dpyId = data[1];
	ret = XNVCTRLQueryValidTargetAttributeValues(dpy,
			NV_CTRL_TARGET_TYPE_DISPLAY,
			dpyId,
			0,
			NV_CTRL_DIGITAL_VIBRANCE,
			&valid_values);

	if(!ret)
	{
		fprintf(stderr, "Unable to query the valid values for "
				"NV_CTRL_DIGITAL_VIBRANCE on display device DPY-%d of "
				"screen %d of '%s'.\n",
				dpyId,
				screen, XDisplayName(NULL));
		return 0;
	}

	/* we assume that NV_CTRL_DIGITAL_VIBRANCE is a range type */

	if (valid_values.type != ATTRIBUTE_TYPE_RANGE) {
		fprintf(stderr, "NV_CTRL_DIGITAL_VIBRANCE is not of "
				"type RANGE.\n");
		return 0;
	}

	ret = XNVCTRLQueryTargetAttribute(dpy,
			NV_CTRL_TARGET_TYPE_DISPLAY,
			dpyId,
			0,
			NV_CTRL_DIGITAL_VIBRANCE,
            &value);

#ifdef _DEBUG
        cout << "The current value of NV_CTRL_DIGITAL_VIBRANCE is " << level << " on display device " <<
        		"DPY-" << dpyId << " of screen " << screen << " of " << XDisplayName(NULL) << endl;
#endif

    return value > 0 ?
            value / ((double)valid_values.u.range.max / 100) :
            value / ((double)valid_values.u.range.min / 100);

}

#ifdef _DEBUG
int main(int argc, char **argv)
{
    NVIDIA *nv = new NVIDIA;
    
    cout << nv->query_active_window_name() << "[" <<
        nv->query_active_window_pid() << "]\n";
    
    if(argc == 2)
    {
        int level = atoi(argv[1]);
        nv->set_vibrance(atoi(argv[1]));
    }
    else if(argc == 3)
    {
        int level = atoi(argv[1]);
        int dpyId = atoi(argv[2]);
        nv->set_vibrance(level, dpyId);
    }
    else
    {
        cout << "Usage" << endl;
        cout << "./test_nvidia <level> [dpyId]" << endl;
    }
    return 0;
}
#endif
