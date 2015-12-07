

#include "RootWindow.h"
#include "LoginViewController.h"

RootWindow* RootWindow::create()
{
    RootWindow* _window = new RootWindow();
    _window->init();
    _window->autorelease();
    return _window;
}

RootWindow::RootWindow()
{

}

RootWindow::~RootWindow()
{

}

bool RootWindow::init()
{
    if (!CAWindow::init())
    {
        return false;
    }
    
    LoginViewController* _viewController = new LoginViewController();
    _viewController->init();
    this->setRootViewController(_viewController);
    _viewController->release();
    
    return true;
}
