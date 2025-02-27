//
// C++ Implementation: misc.cpp
//
// Description:
//
//
// Author: Sven Queisser <tincan@svenqueisser.de>, (C) 2001 - 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "misc.h"

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring> // need memset

double vToDb(double voltage, double reference) {
    // catch senseless cases
    if(voltage < 0 || reference < 0) {
        return 0;
    }
    // catch problematic cases and try to handle them in a sensible way...
    if(voltage == 0) {
        return -INFINITY;
    }
    if(reference == 0) {
        return INFINITY;
    }
    return 8.685889638 * log(voltage / reference); // 20 / log(10) = 8.685....
}

double dbToV(double dbs, double reference) {
    return pow(10, dbs/20) * reference;
}

double stringToNum(const std::string &text) {
    double value = 1.0;
    // check for unit suffixes
    if((signed)text.find('p') != -1) {
        value /= 1000000000000.0;
    } else if((signed)text.find('n') != -1) {
        value /= 1000000000.0;
    } else if((signed)text.find('u') != -1 || (signed)text.find("µ") != -1) {
        value /= 1000000.0;
    } else if((signed)text.find('m') != -1) {
        value /= 1000.0;
    } else if((signed)text.find('k') != -1) {
        value *= 1000.0;
    } else if((signed)text.find('M') != -1) {
        value *= 1000000.0;
    } else if((signed)text.find('G') != -1) {
        value *= 1000000000.0;
    }

    // multiply by the value of the string
    // remaining letters are ignored by atof :)
    value *= atof(text.c_str());
    return value;
}

std::string numToString(double value) {
    // limit number to +/- 1*10^21
    // everything else is treated as infinity...
    if(value > 1000000000000000000000.0) {
        return "inf ";
    }

    if(value < -1000000000000000000000.0) {
        return "-inf ";
    }

    std::string ret;
    if(value != 0.0) { // prevent prefix to a zero..
        if(fabs(value) > 1000000000.0) {
            value /= 1000000000.0;
            ret += "G";
        } else if(fabs(value) > 1000000.0) {
            value /= 1000000.0;
            ret += "M";
        } else if(fabs(value) > 1000.0) {
            value /= 1000.0;
            ret += "k";
        } else if(fabs(value) < 1.0/1000000000) {
            value *= 1000000000000.0;
            ret += "p";
        } else if(fabs(value) < 1.0/1000000) {
            value *= 1000000000.0;
            ret += "n";
        } else if(fabs(value) < 1.0/1000) {
            value *= 1000000.0;
            ret += "u";
        } else if(fabs(value) < 1.0) {
            value *= 1000.0;
            ret += "m";
        }
    }

    char num_temp[40];
    sprintf(num_temp, "%4f", value);

    // remove trailing zeros
    std::string num = num_temp;
    while(num[num.length() - 1] == '0') {
        num.resize(num.length() - 1);
    }
    // remove a now possible trailing dot (or comma)
    if(num[num.length() - 1] == '.' || num[num.length() - 1] == ',') {
        num.resize(num.length() - 1);
    }

    // prepend the converted number to the string
    ret.insert(0, " ");
    ret.insert(0, num);
    return ret;
}

void printMessage(const char *file, const char *function, int line, int level, const char *text) {
    std::string levelInfo;
    if(level < MSG_LEVEL) {
        return;
    }
    FILE *output;
    switch(level) {
    case MSG_NOTICE:
        levelInfo = "**notice**";
        output = stdout;
        break;
    case MSG_DEBUG:
        levelInfo = "**debug**";
        output = stderr;
        break;
    case MSG_INFO:
        levelInfo = "**info**";
        output = stdout;
        break;
    case MSG_WARN:
        levelInfo = "**warn**";
        output = stderr;
        break;
    case MSG_ERROR:
    default:
        levelInfo = "**error**";
        output = stderr;
        break;
    }
    fprintf(output, "%s %s:%d: %s: %s", levelInfo.c_str(), file, line, function, text);
    fflush(output);
}

// bool isNear(double, double)
// returns true, if both numbers differ less than 0.1 percent
// shall be used to compare existing values on the GUI
bool isNear(double a, double b){
    return fabs(a - b) / a < 0.001 && fabs(a - b) / b < 0.001;
}
