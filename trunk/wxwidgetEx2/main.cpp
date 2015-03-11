#include "main.h"
#include "Tetris.h"


IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
    srand(time(NULL));
    Tetris *tetris = new Tetris(wxT("Tetris"));
    tetris->Centre();
    tetris->Show(true);

    return true;
}