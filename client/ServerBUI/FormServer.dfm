object fmServer: TfmServer
  Left = 530
  Top = 263
  Width = 173
  Height = 118
  Caption = 'Server'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object lbOp: TLabel
    Left = 16
    Top = 24
    Width = 3
    Height = 13
  end
  object ServerSocket: TServerSocket
    Active = False
    Port = 0
    ServerType = stNonBlocking
    OnGetSocket = ServerSocketGetSocket
    Left = 112
    Top = 40
  end
end
