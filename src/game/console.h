#ifndef _paintown_console_h
#define _paintown_console_h

#include <string>
#include <vector>
#include <sstream>
#include <map>
#include "input/input-map.h"
#include "return_exception.h"

class Bitmap;

namespace Console{

/* equivalent of std::endl */
class ConsoleEnd{
private:
    friend class Console;
    ConsoleEnd(){
    }
};

class Command{
public:
    Command(){
    }

    virtual std::string act() = 0;

    virtual ~Command(){
    }
};

class Console{
public:
    Console(const int maxHeight, const std::string & font = Console::DEFAULT_FONT);
    virtual ~Console();

    virtual void act();
    virtual void draw(const Bitmap & work);
    virtual void toggle();
    
    virtual void clear();

    virtual bool doInput();
    
    inline int getTextHeight(){ return textHeight; };
    inline int getTextWidth(){ return textWidth; };
    
    inline void setTextHeight(int h){ textHeight = h; };
    inline void setTextWidth(int w){ textWidth = w; };

    inline const std::string & getFont() const {
        return font;
    }

    /* for arbitrary data */
    template<typename T> Console & operator<<(const T & x){
        textInput << x;
        return *this;
    }

    void addCommand(const std::string & name, Command * command);

    /* for end of line, always pass Console::endl */
    Console & operator<<(const ConsoleEnd & e);

    static const std::string DEFAULT_FONT;
    static ConsoleEnd endl;

protected:

    void process(const std::string & command);
    void backspace();
    void clearInput();
    void deleteLastWord();
    virtual std::stringstream & add();

    enum State{
        Closed,
        Open,
        Opening,
        Closing,
    } state;

    const int maxHeight;
    int height;

    std::string font;
    
    // Text height
    int textHeight;
    // Text width
    int textWidth;
    std::vector<std::string>lines;
    // Our text inputer
    std::stringstream textInput;
    std::stringstream currentCommand;
    unsigned int offset;
    InputMap<char> input;
    std::map<std::string, Command*> commands;
    
    void checkStream();
};

}

#endif
