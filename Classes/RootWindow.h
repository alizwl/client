

#ifndef __HelloCpp__RootWindow__
#define __HelloCpp__RootWindow__

#include <iostream>
#include "CrossApp.h"
#include "CDData.h"

USING_NS_CC;

class RootWindow: public CAWindow, public CAKeypadDelegate
{
    
public:

	static RootWindow* getInstance();
    
	RootWindow();
    
	virtual ~RootWindow();
    
    virtual bool init();
    
	CC_SYNTHESIZE_READONLY(CANavigationController*, m_pRootNavigationController, RootNavigationController);
	CC_SYNTHESIZE_READONLY(CADrawerController*, m_pRootDrawerController, DrawerController);
	virtual void keyBackClicked();

    int getUserId() { return m_userId; }
    void setUserId(int id) { m_userId = id; }
    
private:
    
    int m_userId;
};


#endif /* defined(__HelloCpp__ViewController__) */
