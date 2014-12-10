//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("server.res");
USEFORM("FormServer.cpp", fmServer);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TfmServer), &fmServer);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------
