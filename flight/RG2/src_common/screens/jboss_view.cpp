#include "jboss_view.h"
#include "bitmaps/jboss_logo_128x128_16.cpp"
#include <math.h>
#include "debug.h"

JBOSSView::JBOSSView(MainButtonHandler* bhandler, TFT_ILI9163C* lcd, ScreenView* parent)
: ScreenView(bhandler, lcd, 6)
{
    _parent = parent;
    //AddChild()
}


void JBOSSView::OnClick(Button* button)
{
    Done();
    _bhandler->SetActiveScreen(_parent);
}

void JBOSSView::Run()
{
    Buzzer* buzzer = _bhandler->getBuzzer();

    _lcd->clearScreen();
    _lcd->display();
    delay(1000);
    _lcd->setBgColor(0);
    _lcd->setTextColor(WHITE);
    _lcd->write("JBoss Bootstrap Env", 0, 1, 1);
    _lcd->write("JBOSS_HOME:", 0, 2, 1);
    _lcd->write("/symb/opt/jboss-eap-6.0", 0, 3, 1);
    _lcd->write("JAVA_OPTS:", 0, 4, 1);
    _lcd->write("-Xms32M -Xmx64M", 0, 5, 1);
    _lcd->write("----------------", 0, 6, 1);
    _lcd->display();
    delay(3000);
    buzzer->PlayTone(1000, 100);
    _lcd->write("Initializing...", 0, 7, 1);
    _lcd->display();
    delay(3000);

    _lcd->fillScreen(WHITE);
    _lcd->drawBmp(0, 0, jboss_logo_128x128_16);
    _lcd->display();
    delay(1000);
    buzzer->PlayTune(":b=400:c6,g6,4c7");
    _lcd->setTextColor(0);
    _lcd->write("JBoss Fuse", 3, 5, 2);
    _lcd->display();
    delay(1000);
    _lcd->write("loading...", 3, 6, 2);
    _lcd->display();
    delay(2000);

    _lcd->setTextColor(WHITE);
    _lcd->clearScreen();
    _lcd->write("ERR name=jboss:service=Naming", 0, 0, 1);
    _lcd->write("java.rmi.server.Exception:", 0, 2, 1);
    _lcd->write("Port already in use: 1098", 0, 4, 1);
    _lcd->write("Error initializing endpoint", 0, 6, 1);
    _lcd->write("java.net.BindException: Address already in use: JVM_Bind:8080",
            0, 8, 1);
    _lcd->display();
    const int n = 64;

    int spoints[n];
    for (int i = 0; i < n; ++i)
    {
        float a = 2. * M_PI / n * i;
        //spoints[i] = 10000 * sin(a)*sin(a);
        //spoints[i] = 5000  + 10000 * sin(a);
        spoints[i] = 5000. *(1.0 + sin (a - M_PI/2.));
    }

    TIMER_With_Channels* rt = (TIMER_With_Channels*)TIMER::Get_Instance(LED_R_TIM);
    rt->EnablePeripheralClock(true);

    GPIO_PIN* r = GPIO_Helper::GetPin(LED_R_PIN);
    r->SetAF(rt->GetAf());
    r->SetupGPIO_OutAltPP();

    TIMER_Channel* r_ch = rt->GetChannel(LED_R_CH);

    r_ch->EnableChannel(true);
    r_ch->SetupPWM1(10000, 0);
    rt->EnableCounter(true);

    int i = 0;
    while(1)
    {
        //g_ch->UpdatePWMDuty(spoints[i % n]);
        r_ch->UpdatePWMDuty((float)spoints[i % n]);
        buzzer->PlayTone(50 + spoints[i % n]/2);
        if ((i % n) == 0)
        {
            //delay(100);
        }
        ++i;
        delay(20);
    }

    while (!_done)
    {
        delay(50);
    }

    _done = false;

}
