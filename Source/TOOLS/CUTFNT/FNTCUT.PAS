{$M 8192,0,0}
uses dos;

const
  FWIDTH        = 'FONT_WIDTH';
  FHEIGHT       = 'FONT_HEIGHT';
  FIDN          = 'Fnt';

type
  Long  = Longint;
  PByte = ^Byte;


var
  BitPtr : Pointer;     { образ оригинального фонта (8x8/14/16) }
  OrgH   : Byte;        { высота оригинального фонта }

  FntName: String;      { имя выходного файла }
  FntPtr : Pointer;     { образ полученного фонта }
  FntW   : Byte;
  FntH   : Byte;
  FntDir : Byte;        { направление кодирования: 0 - построчно, 1 - по столбцам }
  FntType: Char;        { тип выходного файла: b - binary (def), p - pascal, a - assembler, c - c }
  ChrSize: Byte;        { размер одного символа в байтах }



procedure Help;
begin
  Writeln('Usage: FNTCUT.EXE SrcFont DstFont [/options]');
  Writeln('  Where:');
  Writeln('    SrcFont - filename of bit font 8x8, 8x14 or 8x16');
  Writeln('    DstFont - filename of produce font');
  Writeln('    func options:');
  Writeln('      /w=x - new width font, range in [1..8]');
  Writeln('      /h=x - new height font, range in [1..16]');
  Writeln('      /d=x - direction pack: 0 - horizontal (default), 1 - vertical');
  Writeln('      /o=x - produce file type:');
  Writeln('               b - binary (default)');
  Writeln('               p - pascal');
  Writeln('               a - assembler');
  Writeln('               c - c');
end;


function HexN(l: LongInt; n:Integer): String;
const
  Digits: array[0..15] of Char = '0123456789ABCDEF';
var
  s : String;
begin

  s := '';
  while n > 0 do begin
    s:=Digits[l and $F] + s;
    Dec(n);
    l := l shr 4;
  end;
  HexN := s;
end;

function Int(l: LongInt): String;
var
  s: String;
begin
  Str(l, s);
  Int := s;
end;


function dos_MaxAvail: long; assembler;
asm
        or      bx, -1          { заказываем 65535*16 байт памяти :) }
        mov     ah, 48h
        int     21h             { естественно получаем фигу }
        mov     ax, bx          { зато в BX макс. колличество свободной памяти }
        mov     dx, 16
        mul     dx              { переводим параграфы в байты }
end;

function dos_Alloc(Size: Long): Pointer; assembler;
asm
        mov     ax, word [Size]
        mov     dx, word [Size+2]
        mov     bx, 16
        div     bx              { переводим в параграфы }
        inc     ax
        mov     bx, ax
        mov     ah, 48h
        int     21h
        jnc     @done
        xor     ax, ax
    @done:
        mov     dx, ax          { dx - сегмент }
        xor     ax, ax          { смещение всегда нулевое :) }
end;

function dos_Free(MemPtr: Pointer): Pointer; assembler;
asm
        mov     ax, word [MemPtr+2]
        or      ax, ax
        jz      @exit
        mov     es, ax
        mov     ah, 49h
        int     21h
    @exit:
        xor     dx, dx
        xor     ax, ax
end;

function dos_IncPtr(p: Pointer; incr: Word): Pointer; assembler;
asm
        mov     dx, word [p+2]
        mov     ax, word [p]
        add     ax, [incr]
        jnc     @done
        add     dx, 1000h
    @done:
end;


function dos_DecPtr(p: Pointer; decr: Word): Pointer; assembler;
asm
        mov     dx, word [p+2]
        mov     ax, word [p]
        sub     ax, [decr]
        jnc     @done
        sub     dx, 1000h
    @done:
end;

function FileExists(Name: String): Long;
var
  f: File;
  s: Long;
begin
  Assign(f, Name);
  {$I-}
  Reset(f,1);
  {$I+}
  if (IOResult = 0) and (Name <> '') then
    begin
      s := FileSize(f);
      Close(f);
    end
  else
    s := 0;
  FileExists := s;
end;


function GetValue(s: String; var Result: Long): Boolean;
var
  Code  : Integer;
begin
  GetValue := False;
  Delete(s, 1, 1);
  if s[1] <> '=' then
    Exit;
  Delete(s, 1, 1);
  val(s, Result, Code);
  if (Code <> 0) then
    Exit;
  GetValue := True;
end;



function ParseCmd: Boolean;
var
  f     : File;
  size  : Long;
  p     : PByte;
  i     : Byte;
  rd    : Word;
  Param : String;
begin
  ParseCmd := False;

  BitPtr := NIL;
  FntPtr := NIL;
  FntW   := 8;
  FntH   := 8;
  FntDir := 0;
  FntType:= 'B';

  if ParamCount < 2 then begin
    Help;
    Exit;
  end;
  { первым всегда идет имя исходного файла }
  size := FileExists(ParamStr(1));
  if (size = 0) then begin
    Writeln('ERROR: file "', ParamStr(1), '" not found!');
    Exit;
  end;
  case size of
    2048: OrgH := 8;
    3584: OrgH := 14;
    4096: OrgH := 16;
  else
    begin
      Writeln('ERROR: file "', ParamStr(1), '" is not bit font 8x8 or 8x14 or 8x16!');
      Exit;
    end;
  end;
  Writeln('Alloc memory: ', size, ' bytes');
  BitPtr := dos_Alloc(size);
  if (BitPtr = NIL) then begin
    Writeln('ERROR: not enought memory!');
    Exit;
  end;
  { далее имя получаемого файла }
  FntName := ParamStr(2);

  { парсим остальные параметры }
  for i := 3 to ParamCount do begin
    Param := ParamStr(i);
    if Param[1] in ['-', '/'] then begin
      Delete(Param, 1, 1);
      case UpCase(Param[1]) of
        'W': begin
               if not GetValue(Param, size) then begin
                 Writeln('ERROR: parametr ', ParamStr(i), ' must be number!');
                 Exit;
               end;
               FntW := size;
               if (FntW < 1) or (FntW > 8) then begin
                 Writeln('ERROR: font width must be range in [1..8]!');
                 Exit;
               end;
             end;
        'H': begin
               if not GetValue(Param, size) then begin
                 Writeln('ERROR: parametr ', ParamStr(i), ' must be number!');
                 Exit;
               end;
               FntH := size;
               if (FntH < 1) or (FntW > 16) then begin
                 Writeln('ERROR: font height must be range in [1..16]!');
                 Exit;
               end;
             end;
        'D': begin
               if not GetValue(Param, size) then begin
                 Writeln('ERROR: parametr ', ParamStr(i), ' must be number!');
                 Exit;
               end;
               FntDir := size;
               if (FntDir < 0) or (FntDir > 1) then begin
                 Writeln('ERROR: direction scan flag must be range in [0..1]!');
                 Exit;
               end;
             end;
        'O': begin
               Delete(Param, 1, 1);
               if Param[1] <> '=' then begin
                 Writeln('ERROR: bad parameter "', ParamStr(i),'", expected "="!');
                 Exit;
               end;
               Delete(Param, 1, 1);
               case UpCase(Param[1]) of
                 'B': FntType := 'B';
                 'P': FntType := 'P';
                 'A': FntType := 'A';
                 'C': FntType := 'C';
               else
                 begin
                   Writeln('ERROR: wrong output file type "',Param[1],'"!');
                   Exit;
                 end;
               end; { case }
             end;
        else
          begin
            Writeln('ERROR: unknown parameter "', ParamStr(i),'"!');
            Exit;
          end;
        end; { case }
    end else
      begin
        Writeln('ERROR: bad parameter: "', ParamStr(i),'"!');
        Exit;
      end;     { '-', '/' }
  end; {for}

  { подгружаем фонт и выделяем память под новый фонт }

  if (FntDir = 0) then
    begin
      ChrSize := FntH;
    end
  else begin
      ChrSize := FntW * ((FntH+7) shr 3);
    end;
  Writeln('Alloc memory: ', ChrSize * 256, ' bytes');
  FntPtr := dos_Alloc(ChrSize * 256);
  if (FntPtr = NIL) then begin
    Writeln('ERROR: not enought memory!');
    Exit;
  end;

  Param := ParamStr(1);
  Writeln('Open file: ', Param);
  size := FileExists(Param);
  Assign(f, Param);
  Reset(f, 1);
  p    := BitPtr;
  while (size > 0) and (not EOF(f)) do begin
    BlockRead(f, p^, 512, rd);
    Dec(size, rd);
    p := dos_IncPtr(p, rd);
  end;
  Close(f);
  if (size <> 0) then begin
    Writeln('ERROR: can`t block read from file "', Param, '"!');
    Exit;
  end;

  ParseCmd := TRUE;
end;


{/////////////////////////////////////////////////////////////////////////////
///////////////////////      ////////////////////////
//////////////////////////////////////////////////////////////////////////////}

{ конвертация в обычный битовый фонт, с построчным порядком }
procedure ConvertHor;
var
  p: PByte;
  d: PByte;
  t: PByte;
  b: Byte;
  i: Integer;
  j: Integer;
begin
  p := BitPtr;
  d := FntPtr;
  for i := 0 to 255 do begin
    t := p;
    for j := 0 to FntH-1 do begin
      d^ := t^;
      t := dos_IncPtr(t, 1);
      d := dos_IncPtr(d, 1);
    end;
    p := dos_IncPtr(p, OrgH);
  end;
end;

{ конвертация в битовый фонт с расположением по столбцам }
procedure ConvertVer;
begin

end;

procedure SaveBinary;
var
  f: File;
begin
  Assign(f, FntName);
  ReWrite(f, 1);
  BlockWrite(f, FntPtr^, 256*ChrSize);
  Close(f);
end;

procedure SaveAssembler;
var
  f: Text;
  i, j: Integer;
  p: PByte;
begin
  Assign(f, FntName);
  ReWrite(f);
  Writeln(f, FWIDTH, ' = ', Int(FntW));
  Writeln(f, FHEIGHT, ' = ', Int(FntH));
  Writeln(f, FIDN, Int(FntW), 'x', Int(FntH), ':');
  p := FntPtr;
  for i := 0 to 255 do begin
    Write(f, '  db ');
    for j := 0 to ChrSize-1 do begin
      Write(f, '0', HexN(p^, 2), 'h');
      if j < (ChrSize-1) then
        Write(f, ',');
      p := dos_IncPtr(p, 1);
    end;
    if i < 32 then
      Writeln(f, '    ; code 0x', HexN(i,2))
    else
      Writeln(f, '    ; code 0x', HexN(i,2), ' for char ',chr(i));
  end;
  Close(f);
end;

procedure SaveC;
var
  f: Text;
  i, j: Integer;
  p: PByte;
begin
  Assign(f, FntName);
  ReWrite(f);
  Writeln(f, '#define ',FWIDTH, '  ', Int(FntW));
  Writeln(f, '#define ',FHEIGHT, ' ', Int(FntH));
  Writeln(f, 'const unsigned char ',FIDN, Int(FntW), 'x', Int(FntH), '[] = {');
  p := FntPtr;
  for i := 0 to 255 do begin
    Write(f, '    ');
    for j := 0 to ChrSize-1 do begin
      Write(f, '0x', HexN(p^, 2));
      if (i = 255) and (j = ChrSize-1) then
        Write(f, ' ')
      else
        Write(f, ',');
      p := dos_IncPtr(p, 1);
    end;
    if i < 32 then
      Writeln(f, '    // code 0x', HexN(i,2))
    else
      Writeln(f, '    // code 0x', HexN(i,2), ' for char ',chr(i));
  end;
  Writeln(f, '};');
  Close(f);
end;

procedure SavePascal;
var
  f: Text;
  i, j: Integer;
  p: PByte;
begin
  Assign(f, FntName);
  ReWrite(f);
  Writeln(f, 'const');
  Writeln(f, '  ',FWIDTH, '  = ', Int(FntW), ';');
  Writeln(f, '  ',FHEIGHT, ' = ', Int(FntH), ';');
  Writeln(f, '  ',FIDN, Int(FntW), 'x', Int(FntH), ': array [0..',Int(ChrSize*256-1),'] of Byte = (');
  p := FntPtr;
  for i := 0 to 255 do begin
    Write(f, '    ');
    for j := 0 to ChrSize-1 do begin
      Write(f, '$', HexN(p^, 2));
      if (i = 255) and (j = ChrSize-1) then
        Write(f, ' ')
      else
        Write(f, ',');
      p := dos_IncPtr(p, 1);
    end;
    if i < 32 then
      Writeln(f, '    { code 0x', HexN(i,2), '}')
    else
      Writeln(f, '    { code 0x', HexN(i,2),' for char ',chr(i), '}');
  end;
  Writeln(f, ');');
  Close(f);
end;


begin
  Writeln('Bit font tool.');
  if ParseCmd then
    begin
      if FntDir = 0 then
        ConvertHor
      else
        ConvertVer;

      case FntType of
        'B': SaveBinary;
        'P': SavePascal;
        'C': SaveC;
        'A': SaveAssembler;
      end;

      Writeln('Done.');
    end;
  if BitPtr <> NIL then
    dos_Free(BitPtr);
  if FntPtr <> NIL then
    dos_Free(FntPtr);
  Writeln('Good bye!');
end.
