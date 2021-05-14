/**
 * @file DongleCfg.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-05-13
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

class DongleCfg
{
private:
    char *ssid;
    char *pass;    
public:
    DongleCfg() {}
    DongleCfg(char *ssid, char *pass){
        this->pass = pass;
        this->ssid = ssid;
    }

    char *getSsid(){
        return ssid;
    }

    char *getPass(){
        return pass;
    }
};