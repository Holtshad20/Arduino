object Form2: TForm2
  Left = 812
  Top = 195
  Width = 241
  Height = 314
  Caption = 'Ajustar Ganhos'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 16
    Top = 72
    Width = 14
    Height = 13
    Caption = 'KP'
  end
  object Label2: TLabel
    Left = 16
    Top = 112
    Width = 10
    Height = 13
    Caption = 'KI'
  end
  object Label3: TLabel
    Left = 16
    Top = 152
    Width = 15
    Height = 13
    Caption = 'KD'
  end
  object Edit1: TEdit
    Left = 56
    Top = 72
    Width = 65
    Height = 21
    TabOrder = 0
    Text = '0'
  end
  object Edit2: TEdit
    Left = 56
    Top = 112
    Width = 65
    Height = 21
    TabOrder = 1
    Text = '0'
  end
  object Edit3: TEdit
    Left = 56
    Top = 152
    Width = 65
    Height = 21
    TabOrder = 2
    Text = '0'
  end
  object Button1: TButton
    Left = 80
    Top = 200
    Width = 75
    Height = 17
    Caption = 'Enviar'
    TabOrder = 3
    OnClick = Button1Click
  end
  object Edit4: TEdit
    Left = 144
    Top = 72
    Width = 65
    Height = 21
    Enabled = False
    TabOrder = 4
    Text = 'Erro'
  end
  object Edit5: TEdit
    Left = 144
    Top = 112
    Width = 65
    Height = 21
    Enabled = False
    TabOrder = 5
    Text = 'Erro'
  end
  object Edit6: TEdit
    Left = 144
    Top = 152
    Width = 65
    Height = 21
    Enabled = False
    TabOrder = 6
    Text = 'Erro'
  end
end
