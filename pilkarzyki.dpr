// JEZU UFAM TOBIE...



{
  PROGRAM      : pilkarzyki
  WERSJA       : 0.0.1 Freeware
  AUTOR        : Matusz Janda - ulimo@op.pl
  UTWORZONY    : 27.04.2003
  MODYFIKOWANY : 12.05.2003,  22:49

  OPIS DZILANIA. Proagram pozwala na rozgrywanie prostych pojedykow miedzy,
   czlowiekiem, a programem reazlizowanym przez komputer w populranej grze
   pilkarzyki. Program traktuje boisko jako nie spojny graf nieskierowany
   wyszukuje najlepsza dorge po wierzcholkach nalezacych do podgrafu dozac do
   zwyciestwa, poprzez analize wag poszczegolnych wierzcholkow. Wagi dla
   poczegolych wierzcholkow zostaly wczesniej ustalone, a ich wartosci
   odpowiadaja dlugosci najkrotszej drogi prowadzacej z wierzcholka do bramki
   ( im blizej bramki nalezacej dogracza tym waga jest mniejsza ). Poszczegolne
   drogi sa znajdywane poprzez generowanie wszystkich mozliwych  pozycji
   wierzcholkow na ktore moze przeniesc sie "pilka" ( przez minimalna
   transpozycje ostatniego wierzcholka ). Przeniesienie nastepuje dopiero po
   sprawdzeniu warunku na to, ze nowo generowany wierzcholek nie jest incydentny
   z wierzcholkiem z ktorego "nastepujes" generacja. Po wygenerowaniu calej
   drogi tzn. takiej ktorej ostatni wierzcholek ma stopien 1, przekazywana jest
   ona do dalszej analizy. Dodatkow zawarto warunek, na to aby program nie
   generowal nastepnych pozycji wierzcholka z wierzcholow, ktorych waga jest
   wieksz lub rowna 7 tzn do takich albo sie da "wejsc" ale nie da sie "wyjsc",
   lub wogole nie da sie "wyjsc". Poniewaz analiza wygenerowanych drog bedzie
   coraz bardziej zlozona w miare optymalizowania programu postaram sie ja
   opisac dopiero w procedurze, ktora bedzie sie tym zjawiskiem zajmowac. Po
   wyborze "zdaniem komputera" najlepszej dorgi uzupelniane sa inforamcje o
   wierzcholka tzn. zwiekszane sa ich stopnie, dla wierzcholkow incydentnych
   dodawane sa wpisy do listy sasiedztw. W ten oto sposob nasz podgraf zostaje
   zweikszony o jeden wierzcholek, a dlasza jego modyfikacja tzn. wiekszenie
   jego "rozmiaru" przekazywane jest przeciwnikowi tak az do momentu osigniecia
   bramki.
}


program pilkarzyki;
{$APPTYPE CONSOLE}

uses
  SysUtils,
  windows;

{
const n=8;
      m=10;
}
type v = record                  { rekord ktorego elementy sa wierzcholkami }
       waga : shortint;
       deg : shortint;
    end;
    wspolrzedne = record         { rekord ktorego elementy sa punktami }
       x : shortint;
       y : shortint;
    end;

    stos = ^kom_stosu;
    kom_stosu = record           { stos na ktorym przechowywane sa wierzcholki }
       dl_stosu : integer;
       x : shortint;
       y : shortint;
       prev : stos;
    end;

var lista_sasiedztw : array[0..n,0..m] of array[1..8] of wspolrzedne;
    { lista sasiedztw danego wierzcholka }

    wierzcholek : array[0..n,0..m] of v;
    { wierzcholek do ktorego odowlujemy sie przez jego wspolrzedne }

    rozgrywka: shortint;
    { wybierany przez gracza tryb pracy z programem }

    pozycja_x, pozycja_y : shortint;
    { aktualna pozycja "pilki" }

    stos_komp, stos_hum : stos;
    { stosy na ktorych przechowywane sa wierzcholki }

    koniec_gry : boolean;
    { zmienna pozwalajaca stwierdzic kidey zostal strzelony gol }





{******************************************************************************
 Procedury GotoXY, ClrScr, TextAttribute ( czyli textcolor ) znane z Pascala,
 a nie osiagalne w kompilatorze Delphi. Uzyskane dzieki pomocy pana
 dr. Kubaszka z dokumentu "Delphi FAQs and TIs.chm"
}
{
procedure GotoXY(xx, yy :Smallint);
var P : TCoord;
 begin
  P.X:=xx; P.Y:=yy;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), P);
 end;
}

{
procedure ClrScr;
var sbi :TConsoleScreenBufferInfo; i :integer;
 begin
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), sbi);
  for i := 0 to sbi.dwSize.y do writeln;
  GotoXY(0,0);
 end;
}

{
procedure TextAttribute(wAttributes: Word);
}
{
wAttributes: za pomoc¹ "OR" mo¿na ³¹czyæ atrybuty:
FOREGROUND_BLUE, FOREGROUND_GREEN, FOREGROUND_RED, FOREGROUND_INTENSITY,
BACKGROUND_BLUE, BACKGROUND_GREEN, BACKGROUND_RED, BACKGROUND_INTENSITY.
}
{
 begin
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),wAttributes);
 end;
}



{******************************************************************************
 Procedura umozliwiajaca dodawnaie na sots nowego wierzchola ( jego wsporzedne
 i aktualna dlugosc stosu ), na wyjscie zwaracany jest nowy stos.
}
procedure na_stos(var ost_stos : stos; dl_stosu : integer; x, y : shortint);
var temp : stos;
 begin
  new(temp);
  temp^.dl_stosu := dl_stosu;
  temp^.x := x;
  temp^.y := y;
  temp^.prev := ost_stos;
  ost_stos := temp;
 end;




{******************************************************************************
 Procedura umozliwiajaca zdejmowanie ze stosu ostatniego elemetu, na wyjsciu
 zwracany jest nowy sots, jego aktulna dlugosc, wspolrzedne wierzchola
 znajdujacego sie na szczycie stosu ( our_x, our_y ), oraz wspolrzedne
 wierzcholka zdejmowanego ze stosu ( prev_gen_x, prev_gen_y ).
}
procedure ze_stosu(var ost_stos : stos; var dl_stosu : integer;
 var our_x, our_y, prev_gen_x, prev_gen_y : shortint );

var temp : stos;
 begin
  temp := ost_stos;
  dl_stosu :=  ost_stos^.dl_stosu;
  prev_gen_x := temp^.x;
  prev_gen_y := temp^.y;
  ost_stos := temp^.prev;
  dispose(temp);

  if ost_stos = nil then
  begin
   our_x := pozycja_x;
   our_y := pozycja_y;
  end else
  begin
   our_x := ost_stos^.x;
   our_y := ost_stos^.y;
  end;
 end;




{******************************************************************************
 Procedura umozliwiajaca odrocenie zawartosci stosu, niszczony jest stary stos
 a na wyjscie zwracany jest sots odwrocony.
}
procedure odwroc_stos( var ost_stos : stos);
var dl_stosu : integer;
    our_x, our_y, x, y : shortint;
    temp : stos;
 begin
  temp := ost_stos;
  ost_stos := nil;
  repeat
   ze_stosu(temp, dl_stosu, our_x, our_y, x, y);
   na_stos(ost_stos, dl_stosu, x, y);
  until temp = nil;
 end;



{******************************************************************************
 Procedura umozliwiajaca kopiowanie zawartosci stosu, na wyjsciu zwracany jest
 wskaznik na nowy stos.
}
procedure kopiuj_stos(ost_stos : stos; var tso_stos : stos);
var temp : stos;
 begin
  temp := ost_stos;
  tso_stos := nil;

  repeat
   na_stos(tso_stos, temp^.dl_stosu, temp^.x, temp^.y);
   temp := temp^.prev;
  until temp = nil;
  odwroc_stos(tso_stos);
 end;



{
procedure czysc_linie(var xx, yy : word);
 begin
  repeat
   gotoxy(xx, yy);
   write(' ');
   xx:= xx + 1;
  until xx = 80
 end;
 }


{******************************************************************************
 Procedura umozliwiajaca podladniecie elemuentow na stosie ( zawartosci ich
 komorek ).
}
{
procedure pokaz_stos(ost_stos : stos);
var temp: stos;
 begin
  temp := ost_stos;
  repeat
    write(temp^.dl_stosu,'|',temp^.x,',',temp^.y,' ');
    temp := temp^.prev;
    readln;
  until temp = nil;
  if temp = nil then writeln('nil ');
 end;
}






{******************************************************************************
 Procedura slozaca do testowanie, czy wlasciwie zostaly ustawione stopnie.
 Nie bierze udzilu we wlasciwym programie.
}
{
procedure pokaz_stopnie;
 var xx,yy : shortint;
 begin
  writeln('Stopnie (deg) wszystkich wierzcholkow:');
  for xx:=0 to n do
   for yy:=0 to m do
   begin
    readln;
    write('[',xx,',',yy,']= ');
    write(wierzcholek[xx,yy].deg);
   end;
   writeln;
   writeln('koniec zapisu nacisnij ENTER');
   readln;
 end;
}




{******************************************************************************
 Procedura slozaca do testowanie, czy wlasciwie zostaly ustawione wagi.
 Nie bierze udzilu we wlasciwym programie.
}
{
procedure pokaz_wagi;
 var xx,yy : shortint;
 begin
  writeln('Wagi wszystkich wierzcholkow:');
  for xx:=0 to n do
   for yy:=0 to m do
   begin
    readln;
    write('[',xx,',',yy,']= ');
    write(wierzcholek[xx,yy].waga);
   end;
   writeln;
   writeln('koniec zapisu nacisnij ENTER');
   readln;
 end;
}




{******************************************************************************
 Procedura slozaca do testowanie, czy wlasciwie zostala wypelniona lista
 sasiedztw. Nie bierze udzilu we wlasciwym programie.
}
{
procedure pokaz_sasiadow;
var xx,yy,i : shortint;
 begin
  writeln('Wszyscy sasiedzi danego wierzcholka:');
  for xx:=0 to n do
   for yy:=0 to m do
   begin
    readln;
    write('[',xx,',',yy,']= ');
    for i:=1 to 8 do
    write('(',lista_sasiedztw[xx,yy][i].x,',',lista_sasiedztw[xx,yy][i].y,')');
   end;
   writeln;
   writeln('koniec zapisu nacisnij ENTER');
   readln;
 end;
}



{******************************************************************************
 Procedura ustawia wszelkie dane dotyczace wierzcholkow, na poczatku rozgrywki
 takie jak aktualna pozycaj "pilki". Stopnie, wagi wierzcholkow a przede
 wszystkim liste sasiedztw wierzcholkow. Mimo ze nie jest to widocze prawdziewe
 boisko powino wygladac na poczatku rozgrywki nastepujaco ( po uwzglednieniu
 wszystkich wierzcholkow ze soba incydentnych ).


    ><|			|><	
   *--*			*--*
 ><|><|><|><|\   /|><|><|><|><
*--*--*--*--*     *--*--*--*--*
    ><|                 |><
   *--* 	        *--*
    ><|                 |><
   *--* 	        *--*
    ><|                 |><
   *--* 	        *--*
    ><|                 |><
   *--* 	        *--*
    ><|                 |><
   *--* 	        *--*
    ><|                 |><
   *--* 	        *--*
    ><|                 |><
   *--* 	        *--*
    ><|                 |><
*--*--*--*--*     *--*--*--*--*
 ><|><|><|><|\   /|><|><|><|><
   *--*			*--*
    ><|	                |><

}
procedure ustaw_dane;
var xx,yy,i : shortint;
 begin
  pozycja_x := n div 2;
  pozycja_y := m div 2;

  koniec_gry := false;

  for xx:=0 to n do
   for yy:=0 to m do
   begin
    wierzcholek[xx,yy].deg:=0;
    wierzcholek[1,yy].deg:=5;
    wierzcholek[n-1,yy].deg:=5;
    wierzcholek[xx,1].deg:=5;
    wierzcholek[xx,m-1].deg:=5;
    wierzcholek[0,yy].deg:=8;
    wierzcholek[n,yy].deg:=8;
    wierzcholek[xx,0].deg:=8;
    wierzcholek[xx,m].deg:=8;

    i:=0;
    repeat
     wierzcholek[xx,m-i].waga:=i;
     i:=i+1;
    until i>m;
   end;
   wierzcholek[(n div 2)-1,1].deg:=3;
   wierzcholek[(n div 2)-1,m-1].deg:=3;
   wierzcholek[(n div 2)+1,1].deg:=3;
   wierzcholek[(n div 2)+1,m-1].deg:=3;
   wierzcholek[(n div 2),0].deg:=0;
   wierzcholek[(n div 2),1].deg:=0;
   wierzcholek[(n div 2),m-1].deg:=0;
   wierzcholek[(n div 2),m].deg:=0;
   wierzcholek[1,1].deg:=7;
   wierzcholek[1,m-1].deg:=7;
   wierzcholek[n-1,1].deg:=7;
   wierzcholek[n-1,m-1].deg:=7;
   wierzcholek[n-1,m-1].deg:=7;
   wierzcholek[1,0].deg:=8;
   wierzcholek[1,m].deg:=8;
   wierzcholek[n,1].deg:=8;
   wierzcholek[n,m-1].deg:=8;

   wierzcholek[1,m-2].waga:=3;
   wierzcholek[n-1,m-2].waga:=3;

   for xx:=0 to n do
    for yy:=0 to m do
     for i:=1 to 8 do
     begin
      lista_sasiedztw[xx,yy][i].x := -1;
      lista_sasiedztw[xx,yy][i].y := -1;
     end;

   for xx:=0 to n do
    for yy:=0 to m do
    begin
     lista_sasiedztw[1,yy][1].x := 0;        {w ld}
     lista_sasiedztw[1,yy][1].y := yy - 1;
     lista_sasiedztw[n,yy][1].x := n - 1;
     lista_sasiedztw[n,yy][1].y := yy - 1;
     lista_sasiedztw[xx,1][1].x := xx - 1;
     lista_sasiedztw[xx,1][1].y := 0;
     lista_sasiedztw[xx,m][1].x := xx - 1;
     lista_sasiedztw[xx,m][1].y := m - 1;

     lista_sasiedztw[1,yy][2].x := 0;       {w lewo}
     lista_sasiedztw[1,yy][2].y := yy;
     lista_sasiedztw[n,yy][2].x := n - 1;
     lista_sasiedztw[n,yy][2].y := yy;
     lista_sasiedztw[xx,1][2].x := xx - 1;
     lista_sasiedztw[xx,1][2].y := 1;
     lista_sasiedztw[xx,m-1][2].x := xx - 1;
     lista_sasiedztw[xx,m-1][2].y := m - 1;

     lista_sasiedztw[1,yy][3].x := 0;       {w lg}
     lista_sasiedztw[1,yy][3].y := yy + 1;
     lista_sasiedztw[n,yy][3].x := n - 1;
     lista_sasiedztw[n,yy][3].y := yy + 1;
     lista_sasiedztw[xx,0][3].x := xx - 1;
     lista_sasiedztw[xx,0][3].y := 1;
     lista_sasiedztw[xx,m-1][3].x := xx - 1;
     lista_sasiedztw[xx,m-1][3].y := m;

     lista_sasiedztw[1,yy][4].x := 1;           {gora}
     lista_sasiedztw[1,yy][4].y := yy + 1;
     lista_sasiedztw[n-1,yy][4].x := n - 1;
     lista_sasiedztw[n-1,yy][4].y := yy + 1;
     lista_sasiedztw[xx,0][4].x := xx;
     lista_sasiedztw[xx,0][4].y := 1;
     lista_sasiedztw[xx,m-1][4].x := xx;
     lista_sasiedztw[xx,m-1][4].y := m;

     lista_sasiedztw[0,yy][5].x := 1;        {pg}
     lista_sasiedztw[0,yy][5].y := yy + 1;
     lista_sasiedztw[n-1,yy][5].x := n;
     lista_sasiedztw[n-1,yy][5].y := yy + 1;
     lista_sasiedztw[xx,0][5].x := xx + 1;
     lista_sasiedztw[xx,0][5].y := 1;
     lista_sasiedztw[xx,m-1][5].x := xx + 1;
     lista_sasiedztw[xx,m-1][5].y := m;

     lista_sasiedztw[0,yy][6].x := 1;       {w praowo}
     lista_sasiedztw[0,yy][6].y := yy;
     lista_sasiedztw[n-1,yy][6].x := n;
     lista_sasiedztw[n-1,yy][6].y := yy;
     lista_sasiedztw[xx,1][6].x := xx + 1;
     lista_sasiedztw[xx,1][6].y := 1;
     lista_sasiedztw[xx,m-1][6].x := xx + 1;
     lista_sasiedztw[xx,m-1][6].y := m - 1;

     lista_sasiedztw[0,yy][7].x := 1;        {pd}
     lista_sasiedztw[0,yy][7].y := yy - 1;
     lista_sasiedztw[n-1,yy][7].x := n;
     lista_sasiedztw[n-1,yy][7].y := yy - 1;
     lista_sasiedztw[xx,1][7].x := xx + 1 ;
     lista_sasiedztw[xx,1][7].y := 0;
     lista_sasiedztw[xx,m][7].x := xx + 1;
     lista_sasiedztw[xx,m][7].y := m - 1;

     lista_sasiedztw[1,yy][8].x := 1;      {w dol}
     lista_sasiedztw[1,yy][8].y := yy - 1;
     lista_sasiedztw[n-1,yy][8].x := n - 1;
     lista_sasiedztw[n-1,yy][8].y := yy - 1;
     lista_sasiedztw[xx,1][8].x := xx;
     lista_sasiedztw[xx,1][8].y := 0;
     lista_sasiedztw[xx,m][8].x := xx;
     lista_sasiedztw[xx,m][8].y := m - 1;
    end;

   lista_sasiedztw[(n div 2)-1,1][6].x := -1;
   lista_sasiedztw[(n div 2)-1,1][6].y := -1;
   lista_sasiedztw[(n div 2)-1,1][7].x := -1;
   lista_sasiedztw[(n div 2)-1,1][7].y := -1;

   lista_sasiedztw[(n div 2)+1,1][1].x := -1;
   lista_sasiedztw[(n div 2)+1,1][1].y := -1;
   lista_sasiedztw[(n div 2)+1,1][2].x := -1;
   lista_sasiedztw[(n div 2)+1,1][2].y := -1;

   lista_sasiedztw[(n div 2)-1,m-1][5].x := -1;
   lista_sasiedztw[(n div 2)-1,m-1][5].y := -1;
   lista_sasiedztw[(n div 2)-1,m-1][6].x := -1;
   lista_sasiedztw[(n div 2)-1,m-1][6].y := -1;

   lista_sasiedztw[(n div 2)+1,m-1][2].x := -1;
   lista_sasiedztw[(n div 2)+1,m-1][2].y := -1;
   lista_sasiedztw[(n div 2)+1,m-1][3].x := -1;
   lista_sasiedztw[(n div 2)+1,m-1][3].y := -1;

   for yy:=0 to m do
    for i:=1 to 8 do
    begin
     lista_sasiedztw[(n div 2),yy][i].x := -1;
     lista_sasiedztw[(n div 2),yy][i].y := -1;
    end;
 end;



{******************************************************************************
 Funkcja pozwalajaca na stweirdzenie czy dany ( wygenerowany ) wierzcholek jest
 z incydentny z wierzcholkie z ktorego nastepuje generacja. Dzila tak jak
 zapytanie "Czy wierzcholek o wspolrzednych (lokator_x, lokator_y)jest sasiedni
 z wierzcholkiem o wspolrzednych ( sasiad_x, sasiad_y ) ? Na wyjscie zwracana
 jest wartosc typu boolean.
}
function sprawdz_sasiada(lokator_x,lokator_y,sasiad_x,sasiad_y: shortint) : boolean;
var i: shortint;
 begin
  i:=0;
  repeat
  i:=i+1;
   if ( lista_sasiedztw[lokator_x,lokator_y][i].x = sasiad_x ) and
      ( lista_sasiedztw[lokator_x,lokator_y][i].y = sasiad_y ) then
      result:=true else
      result:=false;
  until (i=8) or (result=true);
 end;




{******************************************************************************
 Procedura pozwalajaca na dodoanie sasieada dla obu wierzcholkow do listy
 sasiedztw tych wierzcholkow.
}
procedure dodaj_sasiada(our_x, our_y, next_x, next_y : shortint);
var i : shortint;
    koniec_dodawania : boolean;
 begin
 i:=0;
 koniec_dodawania:=false;
  repeat
   i:=i+1;
   if ( lista_sasiedztw[our_x, our_y][i].x = -1 ) and
      ( lista_sasiedztw[our_x, our_y][i].y = -1 ) then
       begin
        lista_sasiedztw[our_x, our_y][i].x := next_x;
        lista_sasiedztw[our_x, our_y][i].y := next_y;
        koniec_dodawania := true;
       end;
  until (i=8) or (koniec_dodawania=true);

  i:=0;
  koniec_dodawania:=false;
  repeat
   i:=i+1;
   if ( lista_sasiedztw[next_x, next_y][i].x = -1 ) and
      ( lista_sasiedztw[next_x, next_y][i].y = -1 ) then
       begin
        lista_sasiedztw[next_x, next_y][i].x := our_x;
        lista_sasiedztw[next_x, next_y][i].y := our_y;
        koniec_dodawania := true;
       end;
  until (i=8) or (koniec_dodawania=true);
 end;




{******************************************************************************
 Procedura pozwalajaca na odjecie sasiada z lisyty sasiedztw dla obu
 wierzcholkow
}
procedure odejmij_sasiada(our_x, our_y, next_x, next_y : shortint);
var i : shortint;
    koniec_dodawania : boolean;
 begin
 i:=0;
 koniec_dodawania:=false;
  repeat
   i:=i+1;
   if ( lista_sasiedztw[our_x, our_y][i].x = next_x ) and
      ( lista_sasiedztw[our_x, our_y][i].y = next_y ) then
       begin
        lista_sasiedztw[our_x, our_y][i].x := -1;
        lista_sasiedztw[our_x, our_y][i].y := -1;
        koniec_dodawania := true;
       end;
  until (i=8) or (koniec_dodawania=true);

  i:=0;
  koniec_dodawania:=false;
  repeat
   i:=i+1;
   if ( lista_sasiedztw[next_x, next_y][i].x = our_x ) and
      ( lista_sasiedztw[next_x, next_y][i].y = our_y ) then
       begin
        lista_sasiedztw[next_x, next_y][i].x := -1;
        lista_sasiedztw[next_x, next_y][i].y := -1;
        koniec_dodawania := true;
       end;
  until (i=8) or (koniec_dodawania=true);
 end;




{******************************************************************************
 Procedura rysuje boisko do gry w srodowisku konsolowym.
}
procedure ustaw_pole_gry;
var i , j : shortint;
 begin
  clrscr;
  for i := 0 to (3*n)-6 do
   for j := 18 downto 22-(m*2) do
   begin
    gotoxy(i,j);
    if (i mod 3 = 0) and (j mod 2 = 0) then write('+');
    gotoxy(0,j);
    if (j mod 2 = 0) then write('*') else write('|');
    gotoxy((3*n)-6,j);

    if (j mod 2 = 0) then write('*') else write('|');
    gotoxy(i,18);
    if (i mod 3 = 0) then write('*') else write('-');
    gotoxy(i,22-(m*2));
    if (i mod 3 = 0) then write('*') else write('-');
   end;
   gotoxy(3*(n div 2)-6,18);
   write('+  +  +');
   gotoxy(3*(n div 2)-6,22-(m*2));
   write('+  +  +');
 end;


{******************************************************************************
 Ta procedura zajmuje sie narysowaniem krawedzie miedzy wierzcholkami na boisku,
 w srodowisku konsolowym.
}
procedure rysuj_pozycje(var dawne_x, dawne_y, xx, yy : shortint);
var r_x, r_y : shortint;
 begin
  r_x := 3*(xx - 1);
  r_y := 18 - 2*(yy - 1);
  if (xx - dawne_x = -1) and (yy - dawne_y = -1) then       { w lewy dolny }
  begin
   gotoxy(r_x + 1, r_y - 1);
   if (sprawdz_sasiada(xx, yy+1, xx+1, yy) = false) then write(' /')
   else write('><');
  end;

  if (xx - dawne_x = -1) and (yy - dawne_y = 0) then        { w lewo }
  begin
   gotoxy(r_x + 1, r_y);
   write('--');
  end;

  if (xx - dawne_x = -1) and (yy - dawne_y = 1) then        { w lewy gorny }
  begin
   gotoxy(r_x + 1, r_y + 1);
   if (sprawdz_sasiada(xx, yy-1, xx+1, yy) = false) then write('\ ')
   else write('><');
  end;

  if (xx - dawne_x = 0) and (yy - dawne_y = 1) then         { w gore }
  begin
   gotoxy(r_x, r_y + 1);
   write('|');
  end;

  if (xx - dawne_x = 1) and (yy - dawne_y = 1) then         { w prawy gorny }
  begin
   gotoxy(r_x - 2, r_y + 1);
   if (sprawdz_sasiada(xx-1, yy, xx, yy-1) = false) then write(' /')
   else write('><');
  end;

  if (xx - dawne_x = 1) and (yy - dawne_y = 0) then        { w prawo }
  begin
   gotoxy(r_x - 2, r_y);
   write('--');
  end;

  if (xx - dawne_x = 1) and (yy - dawne_y = -1) then       {w prawy dolny }
  begin
   gotoxy(r_x - 2, r_y - 1);
   if (sprawdz_sasiada(xx-1, yy, xx, yy+1) = false) then write('\ ')
   else write('><');
  end;

  if (xx - dawne_x = 0) and (yy - dawne_y = -1) then       { w dol }
  begin
   gotoxy(r_x, r_y - 1);
   write('|');
  end;

  gotoxy(3*(dawne_x-1),18-2*(dawne_y-1));
  write('*');

  textattribute(FOREGROUND_INTENSITY OR
               FOREGROUND_RED OR
               FOREGROUND_GREEN);
  gotoxy(r_x, r_y);
  write('*');

  textattribute(FOREGROUND_RED OR
               FOREGROUND_GREEN OR
               FOREGROUND_BLUE);
 end;

{******************************************************************************
 Ta procedura jest MOZDZKIEM programu generuje ona wszystkie wierzcholki ktore
 sasiaduja lub moga sasiadowac z naczym wierzcholkiem ( our_x, our_y ) lub
 dla uproszczenia pozycje na ktore MOZE byc przeniesiona pilka. Poniewaz program
 zwraca sie do tej procedury w roznych momentach np.: generujac stos ( budujac
 droge ) lub usuwajac elementy ze stosu ( cofajac sie po zbudowanej drodze ),
 aby uniknoac generowania ponownie wierzcholka ktory byl juz kiedys z tej
 pozycji wygenerowany, program uwzglednia poprzednio generowana pozycje czyli
 wspolrzedne ( prev_gen_x, prev_gen_y ). Sa to wspolrzedne wierzcholka
 zdejmowanego ze stosu. Do kolejnych warunkow mozna sie dostac jedynie, gdy
 poprzednio wygenerowana pozycja maja wspolrzedne takie jak mozliwe
 do wygenerowania pozycje we wczesnijszym kroku. Dla ulatweina nalezy pamietac,
 ze program sprawdza wszystkie wierzcholki zaczynajac od lewgo dolego nastepnie
 lewego itd. zgodnie z ruchem wskazowek zegara. Jezeli wpolrzedne prev_gen_x,
 prev_gen_y maja wartosc -1 ( jest to specjalnie wymyslony wierzcholek ktory nie
 bierze udzilu w grze ) tzn. ze z tej pozycji nie zostaly jeszcze wygenerowane
 zadene wspolrzedne nastepnego wierzcholka. Jezeli na wyjscie zostaja zwrocone
 wspolrzedne next_x, next_y rowne -1 to oznacz ze z tego wierzcholka nie da
 sie juz generowaca nastepnych wirzcholkow i nalezy ten element zdjac ze
 stosu ( cofnac sie jeden element na zbudowanej drodze ). Numery przy petlach
 warunkowych odpowiadaja wcisnietym klawiszom na klawiaturz w bloku klawiszy
 numerycznych.
}
procedure generuj_pozycje( our_x, our_y, prev_gen_x, prev_gen_y : shortint;
 var next_x, next_y: shortint );

 begin

   {1}
   if ( prev_gen_x = -1 ) or ( prev_gen_y = -1 ) then
   begin
    next_x:=our_x-1;
    next_y:=our_y-1;
   end;

   {4}
   if ( prev_gen_x = our_x - 1 ) and  ( prev_gen_y = our_y - 1 ) then
   begin
    next_x:=our_x-1;
    next_y:=our_y;
   end;

   {7}
   if ( prev_gen_x = our_x - 1 ) and ( prev_gen_y = our_y ) then
   begin
    next_x:=our_x-1;
    next_y:=our_y+1;
   end;

   {8}
   if  ( prev_gen_x = our_x - 1 ) and ( prev_gen_y = our_y + 1 ) then
   begin
    next_x:=our_x;
    next_y:=our_y+1;
   end;

   {9}
   if ( prev_gen_x = our_x ) and ( prev_gen_y = our_y + 1 ) then
   begin
    next_x:=our_x+1;
    next_y:=our_y+1;
   end;

   {6}
   if ( prev_gen_x = our_x + 1) and ( prev_gen_y = our_y + 1 ) then
   begin
    next_x:=our_x+1;
    next_y:=our_y;
   end;

   {3}
   if ( prev_gen_x = our_x + 1) and ( prev_gen_y = our_y ) then
   begin
    next_x:=our_x+1;
    next_y:=our_y-1;
   end;

   {2}
   if ( prev_gen_x = our_x + 1) and ( prev_gen_y = our_y - 1 ) then
   begin
    next_x:=our_x;
    next_y:=our_y-1;
   end;

   {gdy nie mozna wiecej generowac}
   if ( prev_gen_x = our_x ) and ( prev_gen_y = our_y - 1 ) then
   begin
    next_x := -1;
    next_y := -1;
   end;

 end;

{******************************************************************************
 Jest to PRZYSADKA MOZGOWA programu. Jego zadaniem jest znalezienie najgorszej
 z punktu widzenia programu droge, ktora moze wygenerowac czlowiek. Na wejscie
 podawana jest obecna pozycja, ktora sprawdza komputer, natomiast na wyjscie
 zwracana jest waga wierzcholka, najbardziej oddalonego od obecnej pozycji
 wygenerowanej przez komputer. 
}
function przewiduj_ruch(var badane_x, badane_y : shortint ) : shortint;
var our_x, our_y, prev_gen_x, prev_gen_y, next_x, next_y : shortint;
    o_x, o_y, pg_x, pg_y : shortint;
    dl_stosu : integer;
    pierwsza_droga : boolean;
    stos_gen, stos_temp : stos;

 begin
  our_x := badane_x;
  our_y := badane_y;
  prev_gen_x := -1;
  prev_gen_y := -1;
  pierwsza_droga := true;
  stos_gen := nil;
  stos_temp := nil;
  repeat
{ ta czes petli zajmuje sie generowaniem i wybieraniem kolejnych wierzcholkow
    - zajmuje sie budowaniem stosu ( dorgi )                                 }
   repeat
    generuj_pozycje(our_x, our_y, prev_gen_x, prev_gen_y, next_x, next_y);

{ kontrola bledow         }
{   gotoxy(0,22);
   write('                              ');
   gotoxy(0,22);
   write(our_x,',',our_y,' ',prev_gen_x,',',prev_gen_y,' ',next_x,',',next_y);
   readln;
 }

{ jezli droga jest wciaz budowna to uzupelniane sa dane o wierzcholku poniewaz
  moze sie zdarzyc ze w wyniku budowania drogi program utworzy cykl          }
    if ( next_x <> -1 ) and ( next_y <> -1 ) then
    begin
     na_stos( stos_gen, dl_stosu, next_x, next_y);


{ budowanie dorgi zostaje wstrzymane jezeli wierzcholke ktory chcielismy
  dodac juz istnieje na liscie sasiedztw lyb gdy nie da sie z niego
  generowac nastepnych wierzcholkow poniewaz jego stopien jest >= 7           }
     if (( sprawdz_sasiada(our_x, our_y, next_x, next_y ))= false ) and
        ( wierzcholek[next_x, next_y].deg < 7 ) then
     begin
      dodaj_sasiada( our_x, our_y, next_x, next_y );
      wierzcholek[our_x, our_y].deg := wierzcholek[our_x, our_y].deg + 1;
      wierzcholek[next_x, next_y].deg := wierzcholek[next_x, next_y].deg + 1;
      our_x := next_x;
      our_y := next_y;
      prev_gen_x := -1;
      prev_gen_y := -1;
     end else
     ze_stosu( stos_gen, dl_stosu, our_x, our_y, prev_gen_x, prev_gen_y );


    end;
   until (( next_x = -1 ) and ( next_y = -1 )) or
         ( wierzcholek[next_x,next_y].deg = 1 );

{ ta czes petli repeat zajmuje sie zdejmowaniem elementu ze stosu ( cofnieciem
 sie z drogi o jeden element ), a takze wyborem najlepszej z drog            }

 { kontrola bledow       }
 {  gotoxy(0,22);
   write('                      <<<====');
   gotoxy(0,22);
   write(our_x,',',our_y,' ',prev_gen_x,',',prev_gen_y,' ',next_x,',',next_y);
   write(' ', wierzcholek[4, 6].deg );
   write(' ', wierzcholek[next_x,next_y].deg );
   write( ' ',next_x, next_y );
      ( wierzcholek[next_x,next_y].deg = 1 ) or
         ( wierzcholek[next_x,next_y].waga = 0 );
   readln;
 {
{ do anlizy wymagana jest jakakolwiek nie pusta droga, zawsze na poczatku
 bedzie to droga pierwsza                                                     }
   if ( pierwsza_droga = true ) then
   begin
    kopiuj_stos(stos_gen, stos_temp);
    pierwsza_droga := false;
   end;



   if ( wierzcholek[next_x,next_y].deg = 1 ) then
    if ( wierzcholek[stos_gen^.x, stos_gen^.y].waga ) >
       ( wierzcholek[stos_temp^.x, stos_temp^.y].waga ) then
       begin
        while stos_komp <> nil do
        ze_stosu( stos_temp, dl_stosu, o_x, o_y, pg_x, pg_y );
        kopiuj_stos(stos_gen, stos_temp);
       end;


{ Zdejmowanie elementu ze stosu i odpowiedznia modyfikacja danych }
   if ( stos_gen <> nil ) then
   begin
    ze_stosu( stos_gen, dl_stosu, our_x, our_y, prev_gen_x, prev_gen_y );
    odejmij_sasiada( our_x, our_y, prev_gen_x, prev_gen_y );
    wierzcholek[our_x, our_y].deg := wierzcholek[our_x, our_y].deg - 1;
    wierzcholek[prev_gen_x, prev_gen_y].deg := wierzcholek[prev_gen_x, prev_gen_y].deg - 1;
   end;

 { kontrola bledow
   gotoxy(0,22);
   write('                  <<<<***');
   gotoxy(0,22);
   write(our_x,',',our_y,' ',prev_gen_x,',',prev_gen_y);
   readln;
}


  until (( stos_gen = nil ) and ( next_x = -1 ) and ( next_y = -1 ) and
        ( our_x = badane_x ) and  ( our_y = badane_y ) and
        ( prev_gen_x = our_x) and ( prev_gen_y = our_y - 1 ));

  result := wierzcholek[stos_temp^.x, stos_temp^.y].waga;

{ likwidowanie tymczasowego stosu gdy, droga poprowadzila do bramki }
  while stos_temp <> nil do
  ze_stosu( stos_gen, dl_stosu, our_x, our_y, prev_gen_x, prev_gen_y );


 end;






{******************************************************************************
 Jest to MOZG programu. Jego zadaniem jest wybor najlepszej drogi sposrod
 wszystkich mozliwych. Ta czesc programu ma na wzgledzie dobor takich
 wierzcholkow tworzacych droge, aby nie powstal zaden nielegalny ruch np.:
 przejscie na wierzcholek, ktory byl wczesniej zapisany jako jego sasiad, dalsze
 generowanie pozycji jezeli stopien aktulanego wierzcholka ( na ktorym
 znajduje sie pilka ) jest wiekszy od jeden itd. Dodatkowo porgram sugeruje sie
 tym aby waga ostatenigo wierzcholka byla jak nalblizsz zeru tzn. wierzcholek
 byl jak najblizej baramki. Dla utrudnienia razgrywki procedura wybiera sposrod
 drog o tej samej ( jak njmniejszej wadze ) droge o najwiekszej dlugosci.
 Warunki te sa nie wystarczajace do tego aby komputer realizujacy porgram mial
 wyzsze szanse na zwyciestwo, do tego celu konieczne jest zbudowanie funkcji
 opartej calkowiecie na tej procedurze, ktorej zadanim byloby pesymistyczne
 przewidzenie nastepnego ruchy przeciwnika. Na wyjsciu tej zwracana byla waga
 tak wygenerowanej drogi. Bedzie to mozliwie dopiero, gdy w tej chwili omawiana
 procedura bedzie w pelni dzialajaca.
}
procedure komp;
var our_x, our_y, prev_gen_x, prev_gen_y, next_x, next_y : shortint;
    o_x, o_y, pg_x, pg_y : shortint;
    dl_stosu : integer;
    pierwsza_droga : boolean;
    stos_gen, stos_temp : stos;

 begin
  our_x := pozycja_x;
  our_y := pozycja_y;
  prev_gen_x := -1;
  prev_gen_y := -1;
  dl_stosu := 0;
  pierwsza_droga := true;
  stos_komp := nil;
  stos_gen := nil;
  repeat
{ ta czes petli zajmuje sie generowaniem i wybieraniem kolejnych wierzcholkow
    - zajmuje sie budowaniem stosu ( dorgi )                                 }
   repeat
    generuj_pozycje(our_x, our_y, prev_gen_x, prev_gen_y, next_x, next_y);

{ jezli droga jest wciaz budowna to uzupelniane sa dane o wierzcholku poniewaz
  moze sie zdarzyc ze w wyniku budowania drogi program utworzy cykl          }
    if ( next_x <> -1 ) and ( next_y <> -1 ) then
    begin
     dl_stosu := dl_stosu + 1;
     na_stos( stos_gen, dl_stosu, next_x, next_y);


{ budowanie dorgi zostaje wstrzymane jezeli wierzcholke ktory chcielismy
  dodac juz istnieje na liscie sasiedztw lyb gdy nie da sie z niego
  generowac nastepnych wierzcholkow poniewaz jego stopien jest >= 7           }
     if (( sprawdz_sasiada(our_x, our_y, next_x, next_y ))= false ) and
        ( wierzcholek[next_x, next_y].deg < 7 ) then
     begin
      dodaj_sasiada( our_x, our_y, next_x, next_y );
      wierzcholek[our_x, our_y].deg := wierzcholek[our_x, our_y].deg + 1;
      wierzcholek[next_x, next_y].deg := wierzcholek[next_x, next_y].deg + 1;
      our_x := next_x;
      our_y := next_y;
      prev_gen_x := -1;
      prev_gen_y := -1;
     end else
     ze_stosu( stos_gen, dl_stosu, our_x, our_y, prev_gen_x, prev_gen_y );


    end;
   until (( next_x = -1 ) and ( next_y = -1 )) or
         ( wierzcholek[next_x,next_y].deg = 1 ) or
         ( wierzcholek[our_x,our_y].waga = 0 );

{ ta czes petli repeat zajmuje sie zdejmowaniem elementu ze stosu ( cofnieciem
 sie z drogi o jeden element ), a takze wyborem najlepszej z drog            }

{ do anlizy wymagana jest jakakolwiek nie pusta droga, zawsze na poczatku
 bedzie to droga pierwsza                                                     }
   if ( pierwsza_droga = true ) then
   begin
    kopiuj_stos(stos_gen, stos_komp);
    pierwsza_droga := false;
   end;

{ anlizy i wybory najlepszej, czyli drogri ktorej ostatni wierzcholek ma jak
 najmnijsza wage, a sama dorga jest jak najdluzsza                   }
   if ( next_x <> -1 ) and ( next_y <> -1 ) then
   begin

    if ( wierzcholek[stos_gen^.x, stos_gen^.y].waga ) <
       ( wierzcholek[stos_komp^.x, stos_komp^.y].waga ) then
       begin
        while stos_komp <> nil do
        ze_stosu( stos_komp, dl_stosu, o_x, o_y, pg_x, pg_y );
        kopiuj_stos(stos_gen, stos_komp);
       end;

    if ( wierzcholek[stos_gen^.x, stos_gen^.y].waga ) =
       ( wierzcholek[stos_komp^.x, stos_komp^.y].waga ) then
       begin
        if stos_gen^.dl_stosu = stos_komp^.dl_stosu then
        begin
         while stos_komp <> nil do
         ze_stosu( stos_komp, dl_stosu, o_x, o_y, pg_x, pg_y );
         kopiuj_stos(stos_gen, stos_komp);
        end;
       end;
   end;

{ Zdejmowanie elementu ze stosu i odpowiedznia modyfikacja danych }
   if ( stos_gen <> nil ) then
   begin
    ze_stosu( stos_gen, dl_stosu, our_x, our_y, prev_gen_x, prev_gen_y );
    dl_stosu := dl_stosu - 1;
    odejmij_sasiada( our_x, our_y, prev_gen_x, prev_gen_y );
    wierzcholek[our_x, our_y].deg := wierzcholek[our_x, our_y].deg - 1;
    wierzcholek[prev_gen_x, prev_gen_y].deg := wierzcholek[prev_gen_x, prev_gen_y].deg - 1;
   end;


  until (( stos_gen = nil ) and ( next_x = -1 ) and ( next_y = -1 ) and
        ( our_x = pozycja_x ) and  ( our_y = pozycja_y ) and
        ( prev_gen_x = our_x) and ( prev_gen_y = our_y - 1 ))
        or ( wierzcholek[our_x,our_y].waga = 0 );


{ likwidowanie tymczasowego stosu gdy, droga poprowadzila do bramki }
  while stos_gen <> nil do
  ze_stosu( stos_gen, dl_stosu, our_x, our_y, prev_gen_x, prev_gen_y );

{ odwrocenie stosu jest konieczne do tego aby narysowac dorge na boisku
 poniewaz na szczycie znajduje sie ostatni element drogi a rysowanie musimy
 zaczac od pierwszego                                                         }
  odwroc_stos( stos_komp );


{ samo rysowanie, modyfikacja danych wierzcholka i ustawienie zmiennych
 globalnych                                                                  }
  stos_temp := stos_komp;
  repeat
   dodaj_sasiada( pozycja_x, pozycja_y, stos_temp^.x, stos_temp^.y );
   wierzcholek[pozycja_x, pozycja_y].deg := wierzcholek[pozycja_x, pozycja_y].deg + 1;
   wierzcholek[stos_temp^.x, stos_temp^.y].deg := wierzcholek[stos_temp^.x, stos_temp^.y].deg + 1;
   rysuj_pozycje(pozycja_x , pozycja_y, stos_temp^.x, stos_temp^.y);
   pozycja_x := stos_temp^.x;
   pozycja_y := stos_temp^.y;
   stos_temp := stos_temp^.prev;
  until stos_temp = nil;


  if (wierzcholek[pozycja_x, pozycja_y].waga = 0) then koniec_gry := true;
 end;




{******************************************************************************
 Procedura pozwalajaca na wygenerowanie kolejnych ruchow czlowiekowi tak aby
 nie zlamal on zasad gry, oraz zabraniajaca mu strzal w kant boiska. Sterowanie
 odbywa sie dzieki klawiszom znajdujacym sie w blku klawiszy numerycznych
 klawiatury
}
procedure hum;
var znak : string;
    hum_x, hum_y : shortint;
    dl_stosu : integer;
    koniec_hum : boolean;

 begin
  dl_stosu := 0;
  stos_hum := nil;
  repeat
   repeat
    koniec_hum := false;
    gotoxy(0,23);
    write('                      ');
    gotoxy(0,23);
    write('Twoj ruch: ');
    readln(znak);
    if znak = '1' then
    begin
     hum_x := pozycja_x - 1;
     hum_y := pozycja_y - 1;
     if (sprawdz_sasiada(pozycja_x, pozycja_y, hum_x, hum_y) = false ) then
      koniec_hum := true;
    end;
    if znak = '4' then
    begin
     hum_x := pozycja_x - 1;
     hum_y := pozycja_y;
     if (sprawdz_sasiada(pozycja_x, pozycja_y, hum_x, hum_y) = false ) then
      koniec_hum := true;
    end;
    if znak = '7' then
    begin
     hum_x := pozycja_x - 1;
     hum_y := pozycja_y + 1;
     if (sprawdz_sasiada(pozycja_x,pozycja_y,hum_x,hum_y) = false ) then
      koniec_hum := true;
    end;
    if znak = '8' then
    begin
     hum_x := pozycja_x;
     hum_y := pozycja_y + 1;
     if (sprawdz_sasiada(pozycja_x,pozycja_y,hum_x,hum_y) = false ) then
      koniec_hum := true;
    end;
    if znak = '9' then
    begin
     hum_x := pozycja_x + 1;
     hum_y := pozycja_y + 1;
     if (sprawdz_sasiada(pozycja_x,pozycja_y,hum_x,hum_y) = false ) then
      koniec_hum := true;
    end;
    if znak = '6' then
    begin
     hum_x := pozycja_x + 1;
     hum_y := pozycja_y;
     if (sprawdz_sasiada(pozycja_x,pozycja_y,hum_x,hum_y) = false ) then
      koniec_hum := true;
    end;
    if znak = '3' then
    begin
     hum_x := pozycja_x + 1;
     hum_y := pozycja_y - 1;
     if (sprawdz_sasiada(pozycja_x,pozycja_y,hum_x,hum_y) = false ) then
      koniec_hum := true;
    end;
    if znak = '2' then
    begin
     hum_x := pozycja_x;
     hum_y := pozycja_y - 1;
     if (sprawdz_sasiada(pozycja_x,pozycja_y,hum_x,hum_y) = false ) then
      koniec_hum := true;
    end;

   until ( koniec_hum = true ) and ( wierzcholek[hum_x, hum_y].deg < 7 );
   dodaj_sasiada(pozycja_x, pozycja_y, hum_x, hum_y);
   wierzcholek[pozycja_x, pozycja_y].deg := wierzcholek[pozycja_x, pozycja_y].deg + 1;
   wierzcholek[hum_x, hum_y].deg := wierzcholek[hum_x, hum_y].deg + 1;
   rysuj_pozycje(pozycja_x , pozycja_y, hum_x, hum_y);
   dl_stosu := dl_stosu + 1;

   pozycja_x := hum_x;
   pozycja_y := hum_y;
   na_stos( stos_hum, dl_stosu, hum_x, hum_y);
  until wierzcholek[pozycja_x, pozycja_y].deg = 1;

  if (wierzcholek[pozycja_x, pozycja_y].waga = 0) then koniec_gry := true;
  odwroc_stos( stos_hum );
 end;



{******************************************************************************
 Procedura sterujaca meczem, dodatkowo zapisujaca kolejno generowane wierzcholki
 do pliku.
}
procedure komp_vs_hum;
var plik : text;
    dl_stosu : integer;
    our_x, our_y, prev_gen_x, prev_gen_y : shortint;
 begin
  ustaw_dane;
  ustaw_pole_gry;
  assignfile(plik, 'mecz.txt');
  rewrite(plik);
  writeln(plik, rozgrywka);
  if (rozgrywka = 1) then
  begin
  komp;
   repeat
    ze_stosu( stos_komp, dl_stosu, our_x, our_y, prev_gen_x, prev_gen_y );
    write(plik, our_x,',', our_y,' ');
   until stos_komp = nil;
  end;

  repeat
   if (koniec_gry = false) then
   begin
    hum;
    repeat
     ze_stosu( stos_hum, dl_stosu, our_x, our_y, prev_gen_x, prev_gen_y );
     write(plik, our_x,',', our_y,' ');
    until stos_hum = nil;
   end;


   if (koniec_gry = false) then
   begin
    komp;
    repeat
     ze_stosu( stos_komp, dl_stosu, our_x, our_y, prev_gen_x, prev_gen_y );
     write(plik, our_x,',', our_y,' ');
    until stos_komp = nil;
  end;
  until koniec_gry = true;

  closefile(plik);
  readln;
 end;


{******************************************************************************
 Wywolanie programu, na razie aktywna jest jedynie rozgrywka nr. 1, lecz
 ma to ulec zmianie w miare dalszego optymalizowania programu.
}
begin

 writeln('wybierz rodzaj rozgrywki:');
 writeln('1. komputer - czlowiek');
{ writeln('2. czlowiek - komputer');     }
 readln(rozgrywka);
 if (rozgrywka = 1) or (rozgrywka = 2) then komp_vs_hum
 else
  begin
  end;
end.




