object Form1: TForm1
  Left = 195
  Top = 107
  BorderStyle = bsDialog
  Caption = 'Process manager'
  ClientHeight = 333
  ClientWidth = 445
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  ShowHint = True
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object StatusBar1: TStatusBar
    Left = 0
    Top = 314
    Width = 445
    Height = 19
    Panels = <
      item
        Width = 80
      end
      item
        Width = 230
      end
      item
        Width = 50
      end>
    SimplePanel = False
  end
  object ProgressBar1: TProgressBar
    Left = 0
    Top = 144
    Width = 150
    Height = 16
    Min = 0
    Max = 100
    TabOrder = 1
    Visible = False
  end
  object StringGrid1: TStringGrid
    Left = 8
    Top = 8
    Width = 429
    Height = 297
    ColCount = 8
    DefaultColWidth = 48
    DefaultRowHeight = 16
    FixedCols = 0
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goColSizing, goTabs, goRowSelect, goThumbTracking]
    PopupMenu = PopupMenu1
    TabOrder = 2
  end
  object Timer1: TTimer
    Enabled = False
    OnTimer = Timer1Timer
    Left = 64
    Top = 176
  end
  object PopupMenu1: TPopupMenu
    Left = 24
    Top = 168
    object TerminateProcess1: TMenuItem
      Caption = 'Terminate Process'
      OnClick = TerminateProcess1Click
    end
  end
end
