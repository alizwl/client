
#ifndef __SessionDetail__ViewController__
#define __SessionDetail__ViewController__

#include <iostream>
#include "CrossApp.h"
#include "RootWindow.h"

USING_NS_CC;

class SessionDetailViewController :
	public CAViewController
{
    
public:

	SessionDetailViewController(int sessionId);
    
	virtual ~SessionDetailViewController();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
    void initDetailView();

	void buttonCallBack(CAControl* btn, DPoint point);

	void showAlert();

	void requestMsg();

	void onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json);

    void onStoreRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json);
    
private:

	DSize													m_winSize;

    int                                                     m_sessionId;
	struct newsMsg											m_msg;
	CAView*													m_alertView;
	CAActivityIndicatorView*								m_pLoading;
    
    CALabel*								m_titleLabel;
    CALabel*								m_lectureLabel;
    CALabel*								m_trackLabel;
    CALabel*								m_formatLabel;
    CALabel*								m_startTimeLabel;
    CALabel*								m_durationLabel;
    CALabel*                                m_likeNumLabel;
    CAImageView*                            m_storeBtnImage;
    CommonUrlImageView*						m_urlImageView;

    bool									m_isStore;
    bool									m_canStore;
	
};


#endif /* defined(__HelloCpp__ViewController__) */
