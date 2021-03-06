#include "PotUI.h"
#include "Config.h"
#include "File.h"
#include "math.h"

PotUI::PotUI()
{
}

PotUI::~PotUI()
{
}

void PotUI::drawBall()
{
    if (alpha_ == 0)
    {
        return;
    }
    engine_->setTextureAlphaMod(square_, alpha_ / 2);
    engine_->setTextureAlphaMod(ball_, alpha_);
    int x, y;
    y = win_h_ - 15;
    engine_->renderCopy(square_, left_, y + d_ / 2 - 1, win_w_ - left_ - right_, 2);
    engine_->renderCopy(ball_, left_ + 1.0 * time_ / totoal_time_ * (win_w_ - left_ - right_) - d_ / 2, y, d_, d_);
    int one_square = BP_AUDIO_MIX_MAXVOLUME / 8;
    int v = volume_;
    x = win_w_ - 30;
    y = win_h_ - 5;
    for (int i = 0; i < 8; i++)
    {
        int h = (i + 1) * 2;
        v -= one_square;
        double r = 1;
        if (v < 0)
        {
            r = 1.0 * (one_square + v) / one_square;
        }
        int hc = r * h;
        engine_->renderCopy(square_, x + i * 3, y - hc, 2, hc);
        if (v < 0)
        {
            break;
        }
    }

    //x = _win_w - 10 - BP_AUDIO_MIX_MAXVOLUME / 2 - d;
    //y = 40;
    //engine_->renderCopy(_square, x - 2, y - 2, 6, 2);
    //engine_->renderCopy(_square, x - 2, y + BP_AUDIO_MIX_MAXVOLUME / 2 + d, 6, 2);
    //engine_->renderCopy(_square, x, y, BP_AUDIO_MIX_MAXVOLUME / 2 + d, 2);
    //engine_->renderCopy(_ball, x + _volume / 2, y - d / 2 + 1, d, d);
}

void PotUI::drawText(const std::string& text)
{
    engine_->drawText(fontname_.c_str(), text, 20, win_w_ - 20, 10, alpha_, BP_ALIGN_RIGHT);
    //engine_->drawText(_fontname.c_str(), std::to_string(_volume / 128.0)+"%", 20, _win_w - 10, 35, _alpha, BP_ALIGN_RIGHT);
}

void PotUI::drawUI(uint8_t alpha, int time, int totoalTime, int volume, bool pause)
{
    this->alpha_ = alpha;
    if (alpha == 0)
    {
        text_ = "";
        return;
    }
    //_win_w = engine_->getWindowsWidth();
    //_win_h = engine_->getWindowsHeight();
    engine_->getWindowSize(win_w_, win_h_);
    this->time_ = time;
    this->totoal_time_ = totoalTime;
    this->volume_ = volume;
    drawBall();
    if (text_ == "")
    {
        drawText(convertTimeToString(time) + " / " + convertTimeToString(totoalTime));
    }
    else
    {
        drawText(text_);
    }

    int button_x = button_x_;
    button_y_ = win_h_ - 15;

    engine_->renderCopy(triangle2_, button_x, button_y_, button_w_ / 2, button_h_);
    engine_->renderCopy(triangle2_, button_x + 5, button_y_, button_w_ / 2, button_h_);
    button_x += 15;
    if (!pause)
    {
        engine_->renderCopy(triangle1_, button_x, button_y_, button_w_, button_h_);
    }
    else
    {
        engine_->renderCopy(square_, button_x, button_y_, 4, button_h_);
        engine_->renderCopy(square_, button_x + 6, button_y_, 4, button_h_);
    }
    button_x += 15;
    engine_->renderCopy(triangle1_, button_x, button_y_, button_w_ / 2, button_h_);
    engine_->renderCopy(triangle1_, button_x + 5, button_y_, button_w_ / 2, button_h_);
}

std::string PotUI::convertTimeToString(int time)
{
    char s[256];
    sprintf(s, "%d:%02d:%02d", time / 3600000, time % 3600000 / 60000, time % 60000 / 1000);
    return s;
}

double PotUI::inProcess(int x, int y)
{
    if (y > win_h_ - 20)
    {
        double p = 1.0 * (x - left_) / (win_w_ - left_ - right_);
        if (p >= 0 && p <= 1)
        {
            return p;
        }
    }
    return -1;
}

int PotUI::inButton(int x, int y)
{
    if (y >= button_y_ && y <= button_y_ + button_h_)
    {
        int button_x = button_x_;
        if (x >= button_x && x <= button_x + button_w_)
        {
            return 0;
        }
        button_x += 15;
        if (x >= button_x && x <= button_x + button_w_)
        {
            return 1;
        }
        button_x += 15;
        if (x >= button_x && x <= button_x + button_w_)
        {
            return 2;
        }
    }
    return -1;
}

void PotUI::init()
{
    square_ = engine_->createSquareTexture(40);
    ball_ = engine_->createBallTexture(50);
    triangle1_ = engine_->createBallTexture(200, 1);
    triangle2_ = engine_->createBallTexture(200, 2);
    fontname_ = Config::getInstance()->getString("ui_font");
    if (!File::fileExist(fontname_))
    {
#ifdef _WIN32
        fontname_ = "C:\\Windows\\Fonts\\Cambria.ttc";
        if (!File::fileExist(fontname_))
        {
            fontname_ = "C:\\Windows\\Fonts\\Cambria.ttf";
        }
#else
        fontname_ = "/System/Library/Fonts/Palatino.ttc";
#endif
    }
}

void PotUI::destory()
{
    if (Config::getInstance()->getString("ui_font") == "")
    {
        Config::getInstance()->setString("ui_font", fontname_);
    }
    engine_->destroyTexture(square_);
    engine_->destroyTexture(ball_);
    engine_->destroyTexture(triangle1_);
    engine_->destroyTexture(triangle2_);
}
