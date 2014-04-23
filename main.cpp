// Stephen Wells Project 1: Introduction to Animation with SDL
	//Based upon the Basic Animation Framework by Brian Malloy

#include "manager.h"

int main(int, char*[]) {
   Manager gameManager;
   while(1) {
        try {
            if(!gameManager.play())
            {
                break;
            }
            gameManager.reinit();
        }
        catch (const std::string& msg) { std::cout << msg << std::endl; }
        catch (...) {
            std::cout << "Oops, someone threw an exception!" << std::endl;
        }
   }
   return 0;
}
