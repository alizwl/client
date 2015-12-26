
#ifndef __MyStatus__ViewController__
#define __MyStatus__ViewController__

#include <iostream>
#include "CrossApp.h"
#include "RootWindow.h"

USING_NS_CC;

class MyStatusViewController :
	public CAViewController
{
    
public:

	MyStatusViewController();
    
	virtual ~MyStatusViewController();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();

	void buttonCallBack(CAControl* btn, DPoint point);

	void showAlert();

	void requestMsg();

	void onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json);

    
private:

	DSize													m_winSize;

	struct newsMsg											m_msg;
	CAView*													m_alertView;
	CAActivityIndicatorView*								m_pLoading;
	
};


#endif /* defined(__HelloCpp__ViewController__) */
