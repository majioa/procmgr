//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "pmgrunit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
const void *WRONG_HANDLE = (void *)0xffffffff;

__fastcall TForm1::TForm1(TComponent* Owner)
  : TForm(Owner)
{
  MaxProcessCount = 128;
  Proc = new PROCESSENTRY32PLUS[MaxProcessCount];
  OldProc = new PROCESSENTRY32PLUS[MaxProcessCount];
  StringGrid1->Cells[0][0] = "Exe-файл";
  StringGrid1->Cells[1][0] = "ПИ";
  StringGrid1->Cells[2][0] = "Кол-во задач";
  StringGrid1->Cells[3][0] = "РПИ";
  StringGrid1->Cells[4][0] = "Версия";
  StringGrid1->Cells[5][0] = "Приоритет";
  StringGrid1->Cells[6][0] = "Исп. ЦПУ";
  StringGrid1->Cells[7][0] = "Время процесса";
//  StringGrid1->Cells[8][0] = "Исп. памяти";
  StringGrid1->ColWidths[0] = 100;
  StringGrid1->ColWidths[1] = 40;
  StringGrid1->ColWidths[2] = 40;
  StringGrid1->ColWidths[3] = 40;
  StringGrid1->ColWidths[4] = 30;
  StringGrid1->ColWidths[5] = 70;
  StringGrid1->ColWidths[6] = 30;
  StringGrid1->ColWidths[7] = 50;
//  StringGrid1->ColWidths[8] = 40;
  Timer1Timer(this);
  Timer1->Enabled = true;
 }
//---------------------------------------------------------------------------


void __fastcall TForm1::FormResize(TObject *Sender)
{
  int k = ClientWidth - 16;
  StringGrid1->Width = k;
  StringGrid1->Height = ClientHeight - StatusBar1->Height - 16;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
  memcpy(OldProc, Proc, sizeof(PROCESSENTRY32PLUS) * ProcessCount);
  OldProcessCount = ProcessCount; 
  PROCESSENTRY32PLUS proc;
  memset(&proc,0,sizeof(PROCESSENTRY32PLUS));
  proc.dwSize = sizeof(PROCESSENTRY32PLUS);
  int tflags = TH32CS_SNAPPROCESS	| TH32CS_SNAPTHREAD;
  HANDLE hsnap = CreateToolhelp32Snapshot(tflags,NULL);
  int i = 1, k, j;
  ProcessCount = 0;
  if(hsnap != WRONG_HANDLE)
    {
    k = Process32First(hsnap,&proc);
    while (k || GetLastError() != ERROR_NO_MORE_FILES)
      {
      AddProcess(proc);
      k = Process32Next(hsnap,&proc);
      }
    CloseHandle(hsnap);
    }
  SortProcesses();
  for(j = 0; j < ProcessCount; j++)
    {
    StringGrid1->Cells[0][j+1] = Proc[j].szExeFile;
    StringGrid1->Cells[1][j+1] = Proc[j].th32ProcessID;
    StringGrid1->Cells[2][j+1] = Proc[j].cntThreads;
    StringGrid1->Cells[3][j+1] = Proc[j].th32ParentProcessID;
    if(Proc[j].Version)
      StringGrid1->Cells[4][j+1] = AnsiString(HIWORD(Proc[j].Version)) + "," + AnsiString(LOWORD(Proc[j].Version));
    else
      StringGrid1->Cells[4][j+1] = "?";
    switch(Proc[j].Priority)
      {
      case HIGH_PRIORITY_CLASS:
        StringGrid1->Cells[5][j+1] = "High";
        break;
      case IDLE_PRIORITY_CLASS:
        StringGrid1->Cells[5][j+1] = "Idle";
        break;
      case NORMAL_PRIORITY_CLASS:
        StringGrid1->Cells[5][j+1] = "Normal";
        break;
      case REALTIME_PRIORITY_CLASS:
        StringGrid1->Cells[5][j+1] = "Realtime";
        break;
      default:
        StringGrid1->Cells[5][j+1] = "Unknown";
      }
    StringGrid1->Cells[6][j+1] = AnsiString(Proc[j].CPULoading) + "%";
//    __int64 l1 = Proc[j].UserTime / 86400;
    __int64 l1 = (Proc[j].SysTime + Proc[j].UserTime) / 86400;
    double l = (double)l1 / 10000000;
    TDateTime t(l);
    StringGrid1->Cells[7][j+1] = t.TimeString();
//    StringGrid1->Cells[8][j+1] = Proc[j].MemSize;
    }
  StringGrid1->RowCount = ProcessCount + 1;
  MEMORYSTATUS memstat;
  memstat.dwLength = sizeof(MEMORYSTATUS);
  GlobalMemoryStatus(&memstat);
  StatusBar1->Panels->Items[0]->Text = "Процессов: " + AnsiString(ProcessCount);
//  int free = memstat.dwAvailPhys + memstat.dwAvailPageFile, total = memstat.dwTotalPhys + memstat.dwTotalPageFile;
  int free = memstat.dwAvailPageFile, total = memstat.dwTotalPageFile;
//  int free = memstat.dwAvailPhys, total = memstat.dwTotalPhys;
  StatusBar1->Panels->Items[1]->Text = "Использование памяти: " + AnsiString((total - free)/1024) + "K / " + AnsiString(total/1024) + "K";
  k = 0;
  for(i = 0; i < ProcessCount; i++)
    k += Proc[i].CPULoading;
  StatusBar1->Panels->Items[2]->Text = "Загрузка процессора: " + AnsiString(k) + "%";
}
//---------------------------------------------------------------------------


void __fastcall TForm1::AddProcess(PROCESSENTRY32PLUS &proc)
{
  if(ProcessCount == MaxProcessCount)
    {
    MaxProcessCount *= 2;
    PROCESSENTRY32PLUS *p1 = new PROCESSENTRY32PLUS[MaxProcessCount];
    PROCESSENTRY32PLUS *p2 = new PROCESSENTRY32PLUS[MaxProcessCount];
    memcpy(p1, Proc, sizeof(PROCESSENTRY32PLUS) * ProcessCount);
    memcpy(p2, OldProc, sizeof(PROCESSENTRY32PLUS) * OldProcessCount);
    delete Proc;
    delete OldProc;
    Proc = p1;
    OldProc = p2;
    }
  int i;
  for(i = 0; i < OldProcessCount; i++)
    if(OldProc[i].th32ProcessID == proc.th32ProcessID)
      break;
  if(i == OldProcessCount)
    memset(((char *)&Proc[ProcessCount])+sizeof(PROCESSENTRY32),0,sizeof(PROCESSENTRY32PLUS) - sizeof(PROCESSENTRY32));
  else
    memcpy(&Proc[ProcessCount],&OldProc[i],sizeof(PROCESSENTRY32PLUS));
  i = ProcessCount;
  memcpy(&Proc[i],&proc,sizeof(PROCESSENTRY32));
  //GetStartupInfo
  //GetEnvironmentStrings
  //GetFullPathName
  Proc[i].Version = GetProcessVersion(Proc[i].th32ProcessID);
  HANDLE hproc = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, Proc[i].th32ProcessID);
  if(hproc)
    {
    Proc[i].Priority = GetPriorityClass(hproc);
    FILETIME ftime[5];
    __int64 *time = (__int64 *)ftime;
    __int64 t, z;
    GetProcessTimes(hproc, &ftime[1], &ftime[2], &ftime[3], &ftime[4]);
    GetSystemTimeAsFileTime(&ftime[0]);
    if(Proc[i].SysTime == 0)
      Proc[i].CPULoading = 0;
    else
      Proc[i].CPULoading = (time[3] - Proc[i].SysTime) * 100 / (time[0] - Proc[i].Time);
    Proc[i].UserTime = time[4];
    Proc[i].SysTime = time[3];
    Proc[i].Time = time[0];
//    Proc[i].MemSize = 0;
    }
  else
    {
//    ShowErrorMessage();
    Proc[i].Priority = -1;
    Proc[i].CPULoading = 0;
    Proc[i].SysTime = 0;
    Proc[i].Time = 0;
//    Proc[i].MemSize = 0;
    }
  ProcessCount++;
}

void __fastcall TForm1::SortProcesses()
{
  PROCESSENTRY32PLUS proc;
  int i, j;
  for(i = 1; i < ProcessCount; i++)
    for(j = 0; j < ProcessCount - i; j++)
      if(Proc[j+1].th32ProcessID < Proc[j].th32ProcessID)
        {
        memcpy(&proc,&Proc[j],sizeof(PROCESSENTRY32PLUS));
        memcpy(&Proc[j],&Proc[j+1],sizeof(PROCESSENTRY32PLUS));
        memcpy(&Proc[j+1],&proc,sizeof(PROCESSENTRY32PLUS));
        }
}


void __fastcall TForm1::TerminateProcess1Click(TObject *Sender)
{
  int k = StringGrid1->Selection.Top, l = StringGrid1->Cells[1][k].ToInt();
  HANDLE hproc = OpenProcess(PROCESS_TERMINATE, TRUE, l);
  if(!(hproc && TerminateProcess(hproc,0)))
    ShowErrorMessage();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ShowErrorMessage()
{
  char *z;
  int k = GetLastError();
  FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,NULL,k,0,(char *)&z,0,NULL);
  Application->MessageBox(z,"qqq",MB_OK);
//  StatusBar1->Panels->Items[2]->Text = AnsiString(z);
  LocalFree(z);
}

