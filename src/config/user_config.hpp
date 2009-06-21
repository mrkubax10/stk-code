// $Id$
//
//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2006 SuperTuxKart-Team
//  Modelled after Supertux's configfile.h
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 3
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#ifndef HEADER_USER_CONFIG_HPP
#define HEADER_USER_CONFIG_HPP

/* The following config versions are currently used:
   0: the 0.2 release config file, without config-version number
      (so that defaults to 0)
   1: Removed singleWindowMenu, newKeyboardStyle, oldStatusDisplay,
      added config-version number
      Version 1 can read version 0 without any problems, so 
      SUPPORTED_CONFIG_VERSION is 0.
   2: Changed to SDL keyboard bindings
   3: Added username (userid was used for ALL players)
   4: Added username per player
   5: Enabled jumping, which might cause a problem with old
      config files (which have an unused entry for jump defined
      --> if a kart control for (say) player 2 uses the same key as
      jump for player 1, this problem is not noticed in 0.3, but will
      cause an undefined game action now
   6: Added stick configurations.
*/
const int CURRENT_CONFIG_VERSION = 7;

#include <string>
#include <map>
#include <vector>
#include <fstream>

#include "config/player.hpp"
#include "input/input.hpp"
#include "lisp/lisp.hpp"
#include "lisp/parser.hpp"
#include "lisp/writer.hpp"

class XMLNode;

/**
  * The base of a set of small utilities to enable quickly adding/removing stuff to/from config painlessly.
  */
class UserConfigParam
{
protected:
    std::string paramName;
public:
    virtual ~UserConfigParam() {}
    virtual void write(std::ofstream& stream) const = 0;
    virtual void read(const XMLNode* node) = 0;

};

class IntUserConfigParam : public UserConfigParam
{
    int value;
public:
    IntUserConfigParam(int defaultValue, const char* paramName);
    void write(std::ofstream& stream) const;
    void read(const XMLNode* node);
    
    operator int() const { return value; }
    int& operator=(const int& v) { value = v; return value; }
};

class StringUserConfigParam : public UserConfigParam
{
    std::string value;
public:
    StringUserConfigParam(const char* defaultValue, const char* paramName);
    void write(std::ofstream& stream) const;
    void read(const XMLNode* node);
    
    operator std::string() const { return value; }
    std::string& operator=(const std::string& v) { value = v; return value; }
    const char* c_str() const { return value.c_str(); }
};

class BoolUserConfigParam : public UserConfigParam
{
    bool value;
public:
    BoolUserConfigParam(bool defaultValue, const char* paramName);
    void write(std::ofstream& stream) const;
    void read(const XMLNode* node);
    
    operator bool() const { return value; }
    bool& operator=(const bool& v) { value = v; return value; }
};

class FloatUserConfigParam : public UserConfigParam
{
    float value;
public:
    FloatUserConfigParam(bool defaultValue, const char* paramName);
    void write(std::ofstream& stream) const;
    void read(const XMLNode* node);
    
    operator float() const { return value; }
    float& operator=(const float& v) { value = v; return value; }
};


/**
  * Using X-macros for setting-possible values is not very pretty, but it's a no-maintenance case :
  * when you want to add a new parameter, just add one signle line below and everything else automagically works
  * (including default value, saving to file, loading from file)
  */

#ifndef PARAM_PREFIX
#define PARAM_PREFIX extern
#endif

#ifndef PARAM_DEFAULT
#define PARAM_DEFAULT(X)
#endif

namespace UserConfigParams
{
    PARAM_PREFIX BoolUserConfigParam         m_sfx              PARAM_DEFAULT( BoolUserConfigParam(true, "sfx_on") );
    PARAM_PREFIX BoolUserConfigParam         m_music            PARAM_DEFAULT(  BoolUserConfigParam(true, "music_on") );
    
    /** Default number of karts. */
    PARAM_PREFIX IntUserConfigParam          m_num_karts        PARAM_DEFAULT(  IntUserConfigParam(4, "numkarts") );
    
    /** Default number of laps. */
    PARAM_PREFIX IntUserConfigParam          m_num_laps         PARAM_DEFAULT(  IntUserConfigParam(4, "numlaps") );
    
    /** Default difficulty. */
    PARAM_PREFIX IntUserConfigParam          m_difficulty       PARAM_DEFAULT(  IntUserConfigParam(0, "difficulty") );
    
    /** Index of current background image. */ // TODO : make this a skin choice instead
    PARAM_PREFIX IntUserConfigParam         m_background_index  PARAM_DEFAULT(  IntUserConfigParam(0, "background_index") );
    
    // Attributes that are accessed directly.
    PARAM_PREFIX BoolUserConfigParam        m_gamepad_debug     PARAM_DEFAULT(  BoolUserConfigParam(false, "gamepad_debug") );
    PARAM_PREFIX BoolUserConfigParam        m_track_debug       PARAM_DEFAULT(  BoolUserConfigParam(false, "track_debug") );
    PARAM_PREFIX BoolUserConfigParam        m_bullet_debug      PARAM_DEFAULT(  BoolUserConfigParam(false, "bullet_debug") );
    PARAM_PREFIX BoolUserConfigParam        m_fullscreen        PARAM_DEFAULT(  BoolUserConfigParam(false, "fullscreen") );
    PARAM_PREFIX BoolUserConfigParam        m_no_start_screen   PARAM_DEFAULT(  BoolUserConfigParam(false, "no_start_screen") );
    
    // TODO : adapt to be more powerful with irrlicht
    PARAM_PREFIX BoolUserConfigParam        m_graphical_effects PARAM_DEFAULT(  BoolUserConfigParam(true, "gfx") );
    
    PARAM_PREFIX BoolUserConfigParam        m_display_fps       PARAM_DEFAULT(  BoolUserConfigParam(false, "show_fps") );
    
    // Positive number: time in seconds, neg: # laps. (used to profile AI)
    // 0 if no profiling. Never saved in config file!
    PARAM_PREFIX IntUserConfigParam         m_profile           PARAM_DEFAULT(  IntUserConfigParam(0, "profile") ); 
    
    PARAM_PREFIX BoolUserConfigParam        m_print_kart_sizes  PARAM_DEFAULT(  BoolUserConfigParam(false, "print_kart_sizes") ); // print all kart sizes
    
    PARAM_PREFIX FloatUserConfigParam       m_sfx_volume        PARAM_DEFAULT(  FloatUserConfigParam(1.0, "sfx_volume") );
    PARAM_PREFIX FloatUserConfigParam       m_music_volume      PARAM_DEFAULT(  FloatUserConfigParam(0.7f, "music_volume") );
    
    PARAM_PREFIX IntUserConfigParam         m_max_fps           PARAM_DEFAULT(  IntUserConfigParam(120, "max_fps") );
    
    PARAM_PREFIX StringUserConfigParam      m_item_style        PARAM_DEFAULT(  StringUserConfigParam("items", "item_style") );
    
    PARAM_PREFIX StringUserConfigParam      m_kart_group        PARAM_DEFAULT(  StringUserConfigParam("standard", "kart_group") );      /**< Kart group used last.        */
    PARAM_PREFIX StringUserConfigParam      m_track_group       PARAM_DEFAULT(  StringUserConfigParam("standard", "track_group") );     /**< Track group used last.       */
    PARAM_PREFIX StringUserConfigParam      m_last_track        PARAM_DEFAULT(  StringUserConfigParam("jungle", "last_track") );      /**< name of the last track used. */
    
    PARAM_PREFIX StringUserConfigParam      m_server_address    PARAM_DEFAULT(  StringUserConfigParam("localhost", "server_adress") );
    PARAM_PREFIX IntUserConfigParam         m_server_port       PARAM_DEFAULT(  IntUserConfigParam(2305, "server_port") );
    
    PARAM_PREFIX IntUserConfigParam         m_width             PARAM_DEFAULT(  IntUserConfigParam(800, "width") );
    PARAM_PREFIX IntUserConfigParam         m_height            PARAM_DEFAULT(  IntUserConfigParam(600, "height") );
    PARAM_PREFIX IntUserConfigParam         m_prev_width        PARAM_DEFAULT(  IntUserConfigParam(800, "prev_width") );
    PARAM_PREFIX IntUserConfigParam         m_prev_height       PARAM_DEFAULT(  IntUserConfigParam(600, "prev_height") );
    
    PARAM_PREFIX BoolUserConfigParam        m_prev_windowed     PARAM_DEFAULT(  BoolUserConfigParam(true, "prev_windowed") );
    PARAM_PREFIX BoolUserConfigParam        m_crashed           PARAM_DEFAULT(  BoolUserConfigParam(false, "crashed") ); // TODO : is this used with new code? does it still work?
    PARAM_PREFIX BoolUserConfigParam        m_log_errors        PARAM_DEFAULT(  BoolUserConfigParam(false, "log_errors") );
    
    // TODO? implement blacklist for new irrlicht device and GUI
    PARAM_PREFIX std::vector<std::string>   m_blacklist_res;
    
    // TODO : implement saving to config file
    PARAM_PREFIX std::vector<Player>        m_player;
    
}
#undef PARAM_PREFIX
#undef PARAM_SUFFIX

/** Class for managing general STK user configuration data. */
class UserConfig
{
private:
   
    /** Filename of the user config file. */
    std::string m_filename;

    void        setFilename      ();

public:

    std::string m_warning;
    int         CheckAndCreateDir();
    
         UserConfig();
         UserConfig(const std::string& filename);
        ~UserConfig();
    void setDefaults();

    void  loadConfig();
    void  loadConfig(const std::string& filename);
    void  saveConfig()                    { saveConfig(m_filename);     }
    void  saveConfig(const std::string& filename);

    const std::string
         &getWarning()                     { return m_warning;  }
    void  resetWarning()                   { m_warning="";      }
    void  setWarning(std::string& warning) { m_warning=warning; }
    
};


extern UserConfig *user_config;

#endif

/*EOF*/
