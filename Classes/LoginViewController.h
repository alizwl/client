
#ifndef __Login__ViewController__
#define __Login__ViewController__

#include <iostream>
#include "CrossApp.h"

USING_NS_CC;

class LoginViewController: public CAViewController
{
    
public:

	LoginViewController();
    
	virtual ~LoginViewController();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
};


#endif /* defined(__Login__ViewController__) */
