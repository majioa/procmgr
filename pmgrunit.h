//---------------------------------------------------------------------------

#ifndef pmgrunitH
#define pmgrunitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <mathvar.h>
#include <ComCtrls.hpp>
#include <Grids.hpp>
#include <tlhelp32.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <ADODB.hpp>
#include <Db.hpp>
#include <DBGrids.hpp>
//---------------------------------------------------------------------------
struct PROCESSENTRY32PLUS : public PROCESSENTRY32
{
  int Version;
  int Priority;
  __int64 SysTime;
  __int64 UserTime;
  __int64 Time;
  int CPULoading;
  int MemSize;
};

class TForm1 : public TForm
{
__published:	// IDE-managed Components
  TStatusBar *StatusBar1;
  TProgressBar *ProgressBar1;
  TStringGrid *StringGrid1;
  TTimer *Timer1;
  TPopupMenu *PopupMenu1;
  TMenuItem *TerminateProcess1;
  void __fastcall FormResize(TObject *Sender);
  void __fastcall Timer1Timer(TObject *Sender);
  void __fastcall TerminateProcess1Click(TObject *Sender);
private:	// User declarations
  HANDLE hSnap;
  PROCESSENTRY32PLUS *Proc;
  PROCESSENTRY32PLUS *OldProc;
  int ProcessCount;
  int OldProcessCount;
  int MaxProcessCount;
public:		// User declarations
  __fastcall TForm1(TComponent* Owner);
  void __fastcall AddProcess(PROCESSENTRY32PLUS &proc);
  void __fastcall RemoveProcess(HANDLE proc);
  void __fastcall ProcessTerminate(System::TObject* Sender);
  void __fastcall SortProcesses();
  void __fastcall ShowErrorMessage();
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
