//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("procmgr.res");
USEFORM("pmgrunit.cpp", Form1);
USELIB("C:\Program Files\Borland\CBuilder5\Projects\Lib\mathvar.lib");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
  try
  {
     Application->Initialize();
     Application->Title = "Process manager";
     Application->CreateForm(__classid(TForm1), &Form1);
     Application->Run();
  }
  catch (Exception &exception)
  {
     Application->ShowException(&exception);
  }
  return 0;
}
//---------------------------------------------------------------------------
